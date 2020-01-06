#include "notetaker.h"
#include "pdfdocument.h"
#include "ui_notetaker.h"

NoteTaker::NoteTaker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteTaker)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->centralwidget);

    scene = new QGraphicsScene(this);
}

NoteTaker::~NoteTaker()
{
    delete document;
    delete pages;
    delete ui;
}

void NoteTaker::on_actionNew_triggered()
{
    this->currentFile.clear();
    ui->mainTextEdit->setPlainText(QString());
    currentFile = "";
}

void NoteTaker::on_actionOpen_triggered()
{
    currentFile = QFileDialog::getOpenFileName(this, "Open the file","/home/taylor/Doc");

    QTextStream(stdout) << "Opening file: " << currentFile;
    QFile file{currentFile};

    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    setWindowTitle(currentFile);
    QTextStream in(&file);
    ui->mainTextEdit->setPlainText(in.readAll());

    //Load the pdfTaylor J Wood
    document = loadDocument(currentFile.left(currentFile.lastIndexOf('.')) + ".pdf");
    if (document != nullptr){
        pages = loadPagesAsPixmap(document);

        //Display the pdf
        setViewPixmap((*pages)[0]);
    }

    file.close();

}

void NoteTaker::on_actionSave_As_triggered()
{
    // Open a dialog for users to select file location
    QString filename = QFileDialog::getSaveFileName(this, "Save this file");

    // Set the current file to the location selected
    currentFile = filename;
    this->setWindowTitle(currentFile);

    // Open a file in that location
    QFile file{filename};

    //Check to see if file opened correctly
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning","Cannot save file : " + file.errorString());
        return;
    }

    // Create a text stream to write to the file with
    QTextStream out{&file};

    // Write the contents of the notepad to the file
    out << ui->mainTextEdit->toPlainText();

    //Close the file
    file.close();
}

void NoteTaker::on_actionExit_triggered()
{
    QApplication::quit();
}

void NoteTaker::on_actionSave_triggered()
{

    //Open a text file to write editor contents to
    QFile file{currentFile};

    // Make sure that it opened correctly
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Save file : " + file.errorString());
        return;
    }

    // Create a text stream to write to the file
    QTextStream out{&file};

    //Write the contents of the editor to the file
    out << ui->mainTextEdit->toPlainText();

    //Close the file after writing to it
    file.close();



    //If a pdf document already is open close it
    if (document != nullptr){
        delete document;
        document = nullptr;
    }

    //Clear the pdf's pages from memory aswell
    if (pages != nullptr){
        delete pages;
        pages = nullptr;
    }

    QTextStream(stdout) << '\n' <<currentFile.left(currentFile.lastIndexOf('.')) << '\n';
    //Compile the .tex file into a pdf
    std::system(
                "latexmk --pdf --cd --interaction=batchmode "
                + currentFile.toUtf8()
                );

    //Load the pdfTaylor J Wood
    document = loadDocument(currentFile.left(currentFile.lastIndexOf('.')) + ".pdf");
    pages = loadPagesAsPixmap(document);

    //Display the pdf
    setViewPixmap((*pages)[0]);
}

void NoteTaker::setViewPixmap(QPixmap pixmap){
    /*
     * Sets the pixmap image for the graphics view widget.
     * Used to display the pdf pages.
     */

    scene->addPixmap(pixmap);
    scene->setSceneRect(pixmap.rect());
    ui->pdfView->setScene(scene);

}

void NoteTaker::keyPressEvent(QKeyEvent *event){

    if (event->key() == Qt::Key_F5){
        if(ui->pdfView->isVisible()){
            ui->pdfView->setVisible(false);
        }else{
            ui->pdfView->setVisible(true);
        }
    }
}
