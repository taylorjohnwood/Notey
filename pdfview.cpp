#include "pdfview.h"



PdfView::PdfView(QWidget* parent):
    QGraphicsView(parent),
    currentPage{0},
    document{nullptr},
    zoom{120.0}{

}

PdfView::~PdfView(){
    document.reset();
}

void PdfView::keyPressEvent(QKeyEvent *event){

    // CTRL + Keyboard Events
    if (event->modifiers() & Qt::KeyboardModifier::ControlModifier){
        QTextStream(stdout) << event->key();
        switch(event->key()){
            case(Qt::Key::Key_Backslash):{
                emit focusToggled();
                break;
            }
            case(Qt::Key::Key_Down):{
                if(document->numPages() > currentPage + 1){
                    currentPage++;
                    displayPage(currentPage);
                    verticalScrollBar()->setValue(0);
                }
                break;
            }
            case(Qt::Key::Key_Up):{
                if(currentPage!=0){
                    currentPage--;
                    displayPage(currentPage);
                    verticalScrollBar()->setValue(0);
                }
                break;
            }
            case(Qt::Key::Key_Equal):{
                zoom += 20.0;
                displayPage(currentPage);
                break;
            }
            case(Qt::Key::Key_Minus):{
                zoom -= 20.0;
                displayPage(currentPage);
                break;
            }

        }
        }
    else {
        switch(event->key()){
            case(Qt::Key::Key_Down):{
                QScrollBar * bar = verticalScrollBar();
                bar->setValue(bar->value()+25);
                break;
            }
            case(Qt::Key::Key_Up):{
                QScrollBar * bar = verticalScrollBar();
                bar->setValue(bar->value()-25);
                break;
            }

            case(Qt::Key::Key_Left):{
                QScrollBar * bar = horizontalScrollBar();
                bar->setValue(bar->value()-25);
                break;
            }
            case(Qt::Key::Key_Right):{
                QScrollBar * bar = horizontalScrollBar();
                bar->setValue(bar->value()+25);
                break;
            }
        }
    }
}

void PdfView::setDocument(std::unique_ptr<Poppler::Document> &pdfDoc){
    this->document = std::move(pdfDoc);
}

void PdfView::displayPage(int pageNumber)
{
    // Check that document is valid
    if (document == nullptr){
        return;
    }

    //Get the i'th page
    Poppler::Page* page{document->page(pageNumber)};

    //Add the pixmap image to the vectors
    QImage pageImage = page->renderToImage(zoom,zoom,1.3*zoom,1.3*zoom,5.8*zoom,10*zoom);
//            .convertToFormat(QImage::Format::Format_Grayscale16);
//    pageImage.invertPixels();

//    QTextStream(stdout) << pageImage.pixelColor(20,20).rgba();
//    for (int y=0; y<pageImage.height(); y++){
//        for(int x=0; x<pageImage.width(); x++){
//            if (qGray(pageImage.pixelColor(x,y).rgb()) < qGray(10,10,10)){
//                pageImage.setPixel(x,y, qRgb(40, 44, 52));
//        }
//    }}

    QPixmap pagePixmap = QPixmap::fromImage(pageImage);


    displayPixmap(pagePixmap);

}

void PdfView::displayPixmap(QPixmap pagePixmap){
    pageScene.clear();
    pageScene.addPixmap(pagePixmap);
    pageScene.setSceneRect(pagePixmap.rect());
    this->setScene(&pageScene);
}

std::unique_ptr<Poppler::Document> loadPdf(QString path)
{
    //Load the pdf using the poppler library
    std::unique_ptr<Poppler::Document> document{Poppler::Document::load(path)};
    //Check if the document is locked
    if (!document || document->isLocked()){
        document.reset();
        return nullptr;

    }

    document->setRenderHint( Poppler::Document::Antialiasing);
    document->setRenderHint( Poppler::Document::TextAntialiasing);
    document->setRenderHint( Poppler::Document::TextHinting);


    return document;
}
