#include "fileutilities.h"

std::unique_ptr<Poppler::Document> loadDocument(QString path)
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

std::unique_ptr<std::vector<QPixmap>> loadPagesAsPixmap(std::unique_ptr<Poppler::Document> &document)
{
    std::unique_ptr<std::vector<QPixmap>> pages{new std::vector<QPixmap>};
    for(int i{0}; i < document->numPages();i++){

        //Get the i'th page
        Poppler::Page* page{document->page(i)};

         //Add the pixmap image to the vectors
         pages->push_back(QPixmap::fromImage(
                    page->renderToImage(120.0,120.0)));

        //Delete the page
        delete page;
    }

    //Return the pages
    return pages;
}


int saveTextToFile(QString text, QString path){
    // Open a file in that location
    QFile file{path};

    //Check to see if file opened correctly
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        return false;
    }

    // Write the contents of the text edit to the file
    QTextStream(&file) << text;

    //Close the file
    file.close();
    return true;
}

void createPdfFromTex(QString path){
    //Compile the .tex file into a pdf
    std::system(
                "latexmk --pdf --cd --interaction=batchmode "
                + path.toUtf8()
                );

}
