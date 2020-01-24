#include "notetaker.h"
#include "fileutilities.h"
#include "stringutilities.h"
#include "ui_notetaker.h"

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

    //Syntax Highlighter object
    highlighter = new Highlighter(ui->mainTextEdit->document());

    //Hide the file explorer by default
    ui->console->setVisible(false);

    //Create a graphic scene for the pdf document
    scene.reset(new QGraphicsScene(this));

    //Initialise the environment input dialogue
    initEnvironmentInput();
}

void NoteTaker::initEnvironmentInput(){

    //Create the input widget, which user types environment into
    envInput = new QInputDialog(this);
    envInput->setLabelText("Environment");
    envInput->setInputMode(QInputDialog::TextInput);

    //Create a list of the environment options
    QStringList environments = QStringList();
    environments << "display" << "enumerate"
                 <<"itemize";

    //Create the autocompleter object for the input widget
    envCompleter = new QCompleter(environments,this);
    envCompleter->setCompletionMode(QCompleter::InlineCompletion);

    //Find the text box in the input dialogue
    QLineEdit * textInput = envInput->findChild<QLineEdit*>();

    //Attatch the autocompleter object to it
    textInput->setCompleter(envCompleter);
}

void NoteTaker::on_actionNew_triggered()
{
    //Clear the filename
    this->currentFile.clear();

    //Reset the text box
    ui->mainTextEdit->setPlainText(QString());

    ui->filenameTextBrowser->setText(currentFile);
}

void NoteTaker::on_actionOpen_triggered()
{
    // Get the user to select the file to open
    currentFile = QFileDialog::getOpenFileName(this,
                                               "Open file",
                                               "~/",
                                               "*.tex");

    //Create a file object from the file they chose
    QFile file{currentFile};

    //Create an error warning if the file doesn't open
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }


    ui->filenameTextBrowser->setText(currentFile);

    //Create a text input stream to read from the file
    QTextStream in(&file);

    //Write the file contents to the text edit widget
    ui->mainTextEdit->setPlainText(in.readAll());

    //Close the file, as it's no longer needed
    file.close();

    setPdfFromFilename(currentFile);



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

    createPdfFromTex(currentFile);

    setPdfFromFilename(currentFile);




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

    createPdfFromTex(currentFile);

    setPdfFromFilename(currentFile);
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
        QString environment = "\\[\n{$1}\n\\]";
        int index = environment.indexOf("{$1}");
        environment = environment.remove(QRegularExpression("\\{\\$\\d\\}"));
        QTextCursor cursor = ui->mainTextEdit->textCursor();
        ui->mainTextEdit->insertPlainText(environment);
        cursor.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::MoveAnchor, environment.length() - index);
        ui->mainTextEdit->setTextCursor(cursor);
    } else if(text=="enumerate"){
        QString environment = """\\begin{enumerate}\n"
                                "\t\\item\n"
                                    "\t\t{$1}\n"
                              "\\end{enumerate}""";
        int index = environment.indexOf("{$1}");
        environment = environment.remove(QRegularExpression("\\{\\$\\d\\}"));
        QTextCursor cursor = ui->mainTextEdit->textCursor();
        QTextStream(stdout) << "Cursor Position before insert: " << cursor.position() << '\n';
        ui->mainTextEdit->insertPlainText(environment);
        QTextStream(stdout) << "Cursor Position after insert: " << cursor.position();
        cursor.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::MoveAnchor, environment.length() - index);
        ui->mainTextEdit->setTextCursor(cursor);
    } else if (text=="itemize"){
        QString environment = """\\begin{itemize}\n"
                                "\t\\item\n"
                                    "\t\t{$1}\n"
                              "\\end{itemize}""";
        int index = environment.indexOf("{$1}");
        environment = environment.remove(QRegularExpression("\\{\\$\\d\\}"));
        QTextCursor cursor = ui->mainTextEdit->textCursor();
        QTextStream(stdout) << "Cursor Position before insert: " << cursor.position() << '\n';
        ui->mainTextEdit->insertPlainText(environment);
        QTextStream(stdout) << "Cursor Position after insert: " << cursor.position();
        cursor.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::MoveAnchor, environment.length() - index);
        ui->mainTextEdit->setTextCursor(cursor);
    }

}

void NoteTaker::setPdfFromFilename(QString filename){
    //Load the pdf
    document = loadDocument(withFileExtension(filename, "pdf"));

    if (document!=nullptr){
        pages = loadPagesAsPixmap(document);

        //Display the pdf
        setPdfView((*pages)[0]);
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
    QString word{""};
    QTextCursor cursor = ui->mainTextEdit->textCursor();
    int i = cursor.position()-1;
    QString text = ui->mainTextEdit->toPlainText();
    while((text[i]!=' ') && (text[i]!='\n') && (i>=0)){
        word.prepend(text[i]);
        i--;
        if(i<0){
            break;
        }
    }
    QTextStream(stdout) << word << '\n';
    if (word=="dm"){

        // Get a copy of the text cursor
        QTextCursor cursor = ui->mainTextEdit->textCursor();

        //Save its position before inserting snippet
        int currentPos = cursor.position();

        //The environment to insert
        QString environment = """\\[\n"
                              "{$1}\n"
                              "\\]""";

        //Get the desired position of the cursor after inserting
        int posOffset = environment.indexOf("{$1}");

        QTextStream(stdout) << posOffset << " " << currentPos;

        //Remove the position indicators from the text
        environment = environment.remove(QRegularExpression("\\{\\$\\d\\}"));

        //Remove the snippet trigger
        ui->mainTextEdit->setPlainText(ui->mainTextEdit->toPlainText().remove(ui->mainTextEdit->toPlainText().length()-2,2)); //This is causing buggy behaviour

        //Insert the snippet
        ui->mainTextEdit->insertPlainText(environment);

        //Update the cursor position
        cursor.setPosition(currentPos + posOffset -2); //Minus two because the dm text is deleted

        //Apply the changed cursor to the actual text cursor
        ui->mainTextEdit->setTextCursor(cursor);
    }

}
