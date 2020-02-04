#include "pdfview.h"
#include <QWidget>
#include <QTextStream>
#include <QtEvents>

PdfView::PdfView(QWidget* parent):
    QGraphicsView(parent){

    QTextStream(stdout) << parent;
}

PdfView::~PdfView(){

}

void PdfView::keyPressEvent(QKeyEvent *event){

    // CTRL + Keyboard Events
    if (event->modifiers() & Qt::KeyboardModifier::ControlModifier){
        QTextStream(stdout) << event->key();

        switch(event->key()){

            case(Qt::Key::Key_Backslash):{
                QTextStream(stdout) << "Success";
                emit focusToggled();
            }

        }
    }
}


void PdfView::slotTest(){
    QTextStream(stdout) << "\nSlot Activated";
}
