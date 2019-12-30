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

    document = loadDocument("/home/taylor/Projects/NoteTaker/Calculus_Cheat_Sheet.pdf");

    pages = loadPagesAsPixmap(document);

    setViewPixmap((*pages)[0]);
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
}

void NoteTaker::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file");
    currentFile = filename;
    QFile file{filename};

    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    setWindowTitle(currentFile);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->mainTextEdit->setPlainText(text);

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
    QFile file{currentFile};

    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Save file : " + file.errorString());
        return;
    }
    QTextStream out{&file};

    out << ui->mainTextEdit->toPlainText();

    delete document;
    delete pages;

    document = loadDocument(currentFile.left(currentFile.size()-3) + "pdf");
    pages = loadPagesAsPixmap(document);
    setViewPixmap((*pages)[0]);
    file.close();

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
