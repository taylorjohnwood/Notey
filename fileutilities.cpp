#include "pdfdocument.h"

Poppler::Document* loadDocument(QString path)
{

    QTextStream(stdout) << "Loading pdf:" << path;

    //Load the pdf using the poppler library
    Poppler::Document* document{Poppler::Document::load(path)};

    //Check if the document is locked
    if (!document || document->isLocked()){
        delete document;
        return nullptr;
    }

    document->setRenderHint( Poppler::Document::Antialiasing );
    document->setRenderHint( Poppler::Document::TextAntialiasing );
    document->setRenderHint( Poppler::Document::TextHinting );

    return document;
}

std::vector<QPixmap>* loadPagesAsPixmap(Poppler::Document* document)
{
    std::vector<QPixmap>* pages{new std::vector<QPixmap>};
    for(int i{0}; i < document->numPages();i++){

        //Get the i'th page
        Poppler::Page* page{document->page(i)};

        //Load it as a pixmap
        QPixmap pixmap = QPixmap::fromImage(
                    page->renderToImage(72.0,72.0,0,0,page->pageSize().width(),page->pageSize().height())
                    );

        //Delete the page
        delete page;

        //Add the pixmap image to the vectors
        pages->push_back(pixmap);
    }

    //Return the pages
    return pages;
}


void createDocumentFromTex(QString path){


}
