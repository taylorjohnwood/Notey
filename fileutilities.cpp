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
    //export PATH=~/.TinyTeX/bin/x86_64-linux:$PATH
//    std::system(
//                "export PATH=~/.TinyTeX/bin/x86_64-linux:$PATH && latexmk --pdf --cd --interaction=batchmode "
//                + path.toUtf8()
//                );
    std::system(
<<<<<<< HEAD
                "Rscript /home/taylor/Scripts/compileTexDoc.R \""
                + path.toUtf8() +"\""
=======
                "source ~/.bashrc && latexmk --pdf --cd --interaction=batchmode "
                + path.toUtf8()
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375
                );

    QTextStream(stdout) << path.toUtf8();

}
