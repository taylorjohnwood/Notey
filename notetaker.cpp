#include "notetaker.h"
#include "fileutilities.h"
#include "stringutilities.h"
#include "ui_notetaker.h"
#include "pdfview.h"

NoteTaker::NoteTaker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteTaker)
{
    initGUI();


}

NoteTaker::~NoteTaker()
{
    delete ui;
    delete highlighter;
    delete envInput;
}

void NoteTaker::initGUI(){
    //Setup the Qt UI
     ui->setupUi(this);
     setCentralWidget(ui->centralwidget);

     setupSignalsAndSlots();


    //Syntax Highlighter object
    highlighter = new Highlighter(ui->mainTextEdit->document());

    //Hide the file explorer by default
    ui->console->setVisible(false);

    //Create a graphic scene for the pdf document
    scene.reset(new QGraphicsScene);

    //Initialise the environment input dialogue
    initEnvironmentInput();

    zoom = 240;
}

void NoteTaker::setupSignalsAndSlots(){
    QObject::connect(ui->pdfView, &PdfView::focusToggled,
                     this, &NoteTaker::focusMainTextEdit);
}
void NoteTaker::initEnvironmentInput(){

    //Create the input widget, which user types environment into
    envInput = new QInputDialog(this);
    envInput->setLabelText("Environment");
    envInput->setInputMode(QInputDialog::TextInput);

    //Create a list of the environment options
    QStringList environments = QStringList();
    environments << "display" << "enumerate"
                 <<"itemize"  << "template";

    //Create the autocompleter object for the input widget
    envCompleter = new QCompleter(environments,this);
    envCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

    //Find the text box in the input dialogue
    QLineEdit * textInput = envInput->findChild<QLineEdit*>();

    //Attatch the autocompleter object to it
    textInput->setCompleter(envCompleter);
}
void NoteTaker::focusMainTextEdit(){
    ui->mainTextEdit->setFocus();
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
    document = loadPdf((currentFile.left(currentFile.length()-3)+"pdf"));

    if (document != nullptr){
        pages = loadPagesAsPixmap(document,zoom);
        setPdfView(pages->at(0));
    }
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
     document = loadPdf(currentFile.left(currentFile.length()-3)+"pdf");

     if (document != nullptr){
         pages = loadPagesAsPixmap(document,zoom);
         setPdfView(pages->at(0));
     }
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
    document = loadPdf(currentFile.left(currentFile.length()-3)+"pdf");

    if (document != nullptr){
        pages = loadPagesAsPixmap(document,zoom);
        setPdfView(pages->at(0));
    }
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

void NoteTaker::setPdfView(QPixmap pixmap)
{
    /*
     * Sets the pixmap image for the graphics view widget.
     * Used to display the pdf pages.
     */


    scene->addPixmap(pixmap);
    scene->setSceneRect(pixmap.rect());
    ui->pdfView->setScene(scene.get());
}

void NoteTaker::on_actionEnvironment_Dialogue_triggered()
{
    QString text = "";

    if(envInput->exec()){
        text = envInput->textValue();
        envInput->setTextValue("");
    }

    if (text=="display"){
        insertSnippet("\\[\n{$1}\n\\]",0);

    } else if(text=="enumerate"){
        insertSnippet("""\\begin{enumerate}\n"
                        "\t\\item\n"
                        "\t\t{$1}\n"
                        "\\end{enumerate}""",0);

    } else if (text=="itemize"){
        insertSnippet("""\\begin{itemize}\n"
                                "\t\\item\n"
                                    "\t\t{$1}\n"
                              "\\end{itemize}""",0);

    } else if (text=="template"){
        insertSnippet("""\\documentclass{article}\n"
                      "\\begin{document}\n"
                      "{$1}\n"
                      "\\end{document}""",0);
    }

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

void NoteTaker::on_mainTextEdit_textChanged()
{
    //Find the word (only behind) under the cursor
    QString word{""};
    QTextCursor cursor = ui->mainTextEdit->textCursor();
    int i = cursor.position();
    QString text = ui->mainTextEdit->toPlainText();

    do{
        i--;
        if(i<0){
            break;
        }else if((text[i]==' ') || (text[i]=='\n')){
            break;
        }
        word.prepend(text[i]);

    }while(true);


    //Check the word to see if it matches an environment
    if (word == "dm"){
        insertSnippet("\\[\n{$1}\n\\]",2);

    } else if (word == "//"){
        insertSnippet("\\frac{{$1}}{}",2);

    }else if (word=="bgg"){
        insertSnippet("""\\begin{{$1}}\n"
                      "\t\n"
                      "\\end{}""",3);
    }

}

void NoteTaker::insertSnippet(QString snippet, int triggerLength){
    // Get a copy of the text cursor
    QTextCursor cursor = ui->mainTextEdit->textCursor();

    //Save its position before inserting snippet
    int currentPos = cursor.position();

    //Get the desired position of the cursor after inserting
    int posOffset = snippet.indexOf("{$1}");



    //Remove the position indicators from the text
    snippet = snippet.remove(QRegularExpression("\\{\\$\\d\\}"));

    //Remove the snippet trigger
    ui->mainTextEdit->setPlainText(ui->mainTextEdit->toPlainText().remove(currentPos-triggerLength,triggerLength));

    //Insert the snippet
    cursor.setPosition(currentPos-triggerLength);
    cursor.insertText(snippet);

    //Update the cursor position
    //Have to minus at end due to deleted trigger word
    cursor.setPosition(currentPos + posOffset - triggerLength);

    //Apply the changed cursor to the actual text cursor
    ui->mainTextEdit->setTextCursor(cursor);
}
