#include "latextexteditor.h"

LatexTextEditor::LatexTextEditor(QWidget *parent) : QTextEdit(parent)
{
    //Create the text editors snippet controller
    snipCont.reset(new SnippetController(this));


    //Run the snippet controller when the text is changed
    QObject::connect(this, &LatexTextEditor::textChanged,
                     this, &LatexTextEditor::onTextChanged);
    QObject::connect(this, &LatexTextEditor::cursorPositionChanged,
                  this, &LatexTextEditor::trackCursorPosition);
}

LatexTextEditor::~LatexTextEditor(){

}

<<<<<<< HEAD
=======

void LatexTextEditor::onTextChanged(){
    int deltaSize{};
    if (snipCont->currentSnippet != nullptr){
        if (previousPos <= snipCont->currentSnippet->endPos){
            int deltaSize = toPlainText().length() - previousBufferSize;
            snipCont->currentSnippet->endPos += deltaSize;

            if (previousPos <= snipCont->currentSnippet->startPos){
                snipCont->currentSnippet->startPos += deltaSize;
            }

        }
        QTextStream(stdout) << "\nDelta size: " << deltaSize << '\n'
                            << "Text changed and Snippet exists with data;\n"
                            << "Start Position: " << snipCont->currentSnippet->startPos << '\n'
                            << "End Position: " << snipCont->currentSnippet->endPos << '\n'
                            << "Size: " << snipCont->currentSnippet->size() << '\n';
    }
    if (snipCont->previousWord() == "dm" or snipCont->previousWord() == "beg" or snipCont->previousWord() == "mk"){
        snipCont->insertSnippet();
    }

    QTextStream(stdout) << "\nCursor now at position: "<< textCursor().position()
                        << "\nBuffer size is now: " << toPlainText().length() << '\n';
    previousPos = textCursor().position();
    previousBufferSize = toPlainText().length();
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375

void LatexTextEditor::onTextChanged(){
    int deltaSize{toPlainText().length() - previousBufferSize};
    if (snipCont->currentSnippet){

        // If text changed before the snippet end
        if (previousPos <= snipCont->currentSnippet->endPos){

            // Move the end of the snippet forward
            snipCont->currentSnippet->endPos += deltaSize;

            // If text changed before the start of the snippet
            if (previousPos <= snipCont->currentSnippet->startPos){

                //Move the start of the snippet forward
                snipCont->currentSnippet->startPos += deltaSize;
            }

//            if (previousPos >= snipCont->currentSnippet->startPos){
//                for (Stop stop: *snipCont->currentSnippet->currentStop){
//                    nullptr;
//                }
//            }
        }
    }

    snipCont->insertSnippet(snipCont->previousWord());
    previousPos = textCursor().position();
    previousBufferSize = toPlainText().length();
}

void LatexTextEditor::keyPressEvent(QKeyEvent *event){

    QTextEdit::keyPressEvent(event);

    // CTRL + Keyboard Events
    if (event->modifiers() & Qt::KeyboardModifier::ControlModifier){
        switch(event->key()){
        case Qt::Key::Key_Backslash:
            emit focusToggled();
            break;

<<<<<<< HEAD
=======
        case Qt::Key::Key_Tab:
            if (snipCont->currentSnippet != nullptr){
                snipCont->currentSnippet->currentStop++;
                if(snipCont->currentSnippet->currentStop == snipCont->currentSnippet->stops.end()){
                    delete snipCont->currentSnippet;
                    return;
                }
                QTextCursor cursor(textCursor());
                QTextStream(stdout) << *snipCont->currentSnippet->currentStop;
                cursor.setPosition(snipCont->currentSnippet->startPos + *snipCont->currentSnippet->currentStop);
                setTextCursor(cursor);
            }
            break;
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375
        case Qt::Key::Key_K:
            QTextStream(stdout) << toHtml();
       }
       }


    }

void LatexTextEditor::trackCursorPosition(){
    previousPos = textCursor().position();
<<<<<<< HEAD
=======
    QTextStream(stdout) << "Cursor at position: " << previousPos << '\n';
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375
}
