#include "latextexteditor.h"

LatexTextEditor::LatexTextEditor(QWidget *parent) : QTextEdit(parent)
{
    //Create the text editors snippet controller
    snipCont.reset(new SnippetController);


    //Run the snippet controller when the text is changed
    QObject::connect(this, &LatexTextEditor::textChanged,
                     this, &LatexTextEditor::runSnippetController);
}

LatexTextEditor::~LatexTextEditor(){

}

void LatexTextEditor::runSnippetController(){
    QTextStream(stdout) << "Snippet Controller called";

}

void LatexTextEditor::keyPressEvent(QKeyEvent *event){

    QTextEdit::keyPressEvent(event);

    // CTRL + Keyboard Events
    if (event->modifiers() & Qt::KeyboardModifier::ControlModifier){
        switch(event->key()){
        case Qt::Key::Key_Backslash:
            emit focusToggled();
            break;
       }
    }
}
