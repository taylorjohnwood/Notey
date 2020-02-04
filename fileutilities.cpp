#include "fileutilities.h"


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

std::unique_ptr<std::vector<QPixmap>> loadPagesAsPixmap(std::unique_ptr<Poppler::Document> &document, double &zoom)
{

    //Create the vecotr that stores the pages of the pdf
    std::unique_ptr<std::vector<QPixmap>> pages{new std::vector<QPixmap>};

    // Check that document is valid
    if (document == nullptr){
        return nullptr;
    }

    //Loop through the document and collect its pages
    for(int i{0}; i < document->numPages();i++){

        //Get the i'th page
        Poppler::Page* page{document->page(i)};

        QSize pageSize = page->pageSize();

        QTextStream(stdout) << pageSize.width() << " " << pageSize.height();


         //Add the pixmap image to the vectors
         pages->push_back(QPixmap::fromImage(
                    page->renderToImage(zoom,zoom,1.3*zoom,0*zoom,5.8*zoom,83.33*zoom)));

        //Delete the page, as it is copied into the vector
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
