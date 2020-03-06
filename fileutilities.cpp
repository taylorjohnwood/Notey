#include "fileutilities.h"

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
                "source ~/.bashrc && latexmk --pdf --cd --interaction=batchmode "
                + path.toUtf8()
                );

}
