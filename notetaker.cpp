#include "notetaker.h"


NoteTaker::NoteTaker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteTaker)
{
    QTextStream(stdout) << "Initialising GUI\n";
    initGUI();
    QTextStream(stdout) << "GUI initialised, now setting up "
                           "signals and slots...\n";
    setupSignalsAndSlots();
    QTextStream(stdout) << "Signals and slots successfully setup\n";

}

NoteTaker::~NoteTaker()
{
    delete ui;
    delete highlighter;

}

void NoteTaker::initGUI(){
    //Setup the Qt UI
     ui->setupUi(this);
     setCentralWidget(ui->centralwidget);

    //Syntax Highlighter object
    highlighter = new Highlighter(ui->mainTextEdit->document());

    //Hide the file explorer by default
    ui->console->setVisible(false);

    //Create a graphic scene for the pdf document
    scene.reset(new QGraphicsScene);

    zoom = 240;

    on_actionNew_triggered();
}

void NoteTaker::setupSignalsAndSlots(){

    //Allow latex text editor to give up focus to the pdfView
    connect(ui->mainTextEdit, &LatexTextEditor::focusToggled,
                     ui->pdfView, QOverload<>::of(&PdfView::setFocus));

    //Allow pdfView to give up focus to the latex text editor
    connect(ui->pdfView, &PdfView::focusToggled,
                     ui->mainTextEdit, QOverload<>::of(&LatexTextEditor::setFocus));


}

void NoteTaker::on_actionNew_triggered()
{
    //Clear the filename
    this->currentFile.clear();

    //Reset the text box
    ui->mainTextEdit->setPlainText(QString());
    scene->clear();

    //Change the filename label
    ui->filenameTextBrowser->setText(currentFile);

    ui->mainTextEdit->previousPos = 0;
    ui->mainTextEdit->previousBufferSize = 0;

    QTextStream(stdout) << "\nOpening: " << currentFile <<"\n"
                        << "Starting pos: " << ui->mainTextEdit->previousPos
                        << "\nBuffer size: " << ui->mainTextEdit->previousBufferSize <<'\n';
}

void NoteTaker::on_actionOpen_triggered()
{
    // Get the user to select the file to open
    currentFile = QFileDialog::getOpenFileName(this,
                                               "Open file",
                                               "/home/taylor/Doc",
                                               "*.tex");

    //Create a file object from the file they chose
    QFile file{currentFile};

    //Create an error warning if the file doesn't open
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }


    //Change the filename label
    ui->filenameTextBrowser->setText(currentFile);

    //Create a text input stream to read from the file
    QTextStream in(&file);

    //Write the file contents to the text edit widget
    ui->mainTextEdit->setPlainText(in.readAll());

    //Close the file, as it's no longer needed
    file.close();


    //Compile the tex file
    createPdfFromTex(currentFile);


    //Load the corresponding pdf file for the .tex file
    std::unique_ptr<Poppler::Document> document = loadPdf((currentFile.left(currentFile.length()-3)+"pdf"));

    ui->pdfView->setDocument(document);
    ui->pdfView->displayPage(ui->pdfView->currentPage);

    ui->mainTextEdit->previousPos = ui->mainTextEdit->textCursor().position();
    ui->mainTextEdit->previousBufferSize = ui->mainTextEdit->toPlainText().length();

    QTextStream(stdout) << "\nOpening: " << currentFile <<"\n"
                        << "Starting pos: " << ui->mainTextEdit->previousPos
                        << "Buffer size: " << ui->mainTextEdit->previousBufferSize;
}

void NoteTaker::on_actionSave_As_triggered()
{
    // Open a dialog for users to select file location
    currentFile = QFileDialog::getSaveFileName(this, "Save this file");


    //Save the text to the chosen file
    if(!saveTextToFile(ui->mainTextEdit->toPlainText(), currentFile)){
        QMessageBox::warning(this, "Warning", "Cannot save file");
    }

     ui->filenameTextBrowser->setText(currentFile);

     //Compile the tex file
     createPdfFromTex(currentFile);

     //Load the corresponding pdf file for the .tex file
     std::unique_ptr<Poppler::Document> document = loadPdf((currentFile.left(currentFile.length()-3)+"pdf"));

     ui->pdfView->setDocument(document);
     ui->pdfView->displayPage(ui->pdfView->currentPage);
}

void NoteTaker::on_actionExit_triggered()
{
    QApplication::quit();
}

void NoteTaker::on_actionSave_triggered()
{

    //Save the text to the chosen file
    if(!saveTextToFile(ui->mainTextEdit->toPlainText(), currentFile)){
        QMessageBox::warning(this, "Warning", "Cannot save file");
    }

    ui->filenameTextBrowser->setText(currentFile);

    //Compile the tex file
    createPdfFromTex(currentFile);

    //Load the corresponding pdf file for the .tex file
    std::unique_ptr<Poppler::Document> document = loadPdf((currentFile.left(currentFile.length()-3)+"pdf"));

    ui->pdfView->setDocument(document);
    ui->pdfView->displayPage(ui->pdfView->currentPage);
}

void NoteTaker::on_actionToggle_PDF_View_triggered()
{
    ui->pdfView->setVisible(!ui->pdfView->isVisible());
}

void NoteTaker::on_actionFile_Tree_triggered()
{
    ui->subjectTree->setVisible(!ui->subjectTree->isVisible());
}

void NoteTaker::on_actionToggle_Console_triggered()
{
    ui->console->setVisible(!ui->console->isVisible());
}


void NoteTaker::on_actionAdd_Subject_triggered()
{
    QString subjectName = QInputDialog::getText(this,"Subject Name","Subject Name: ");

    ui->subjectTree->addTopLevelItem(new QTreeWidgetItem((QTreeWidget*)0,QStringList(subjectName)));

}

void NoteTaker::on_actionAdd_Chapter_triggered()
{
    QStringList items;
    for(int i=0; i < ui->subjectTree->topLevelItemCount(); i++){
        items << ui->subjectTree->topLevelItem(i)->text(0);
    }
    QString subjectName = QInputDialog::getItem(this,"Choose Subject", "Subject Name: ", items);
    QString chapterName = QInputDialog::getText(this,"Chapter Name","Chapter Name: ");
    QTreeWidgetItem* subjectWidget = ui->subjectTree->findItems(subjectName,0,0)[0];
    new QTreeWidgetItem(subjectWidget,QStringList(chapterName));

}

