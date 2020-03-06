#include "snippetcontroller.h"

SnippetController::SnippetController(LatexTextEditor * parentPtr):
    QObject(), parent(parentPtr){

}

SnippetController::~SnippetController(){

}

Snippet::Snippet(){

}

Snippet::~Snippet(){

}
int Snippet::size(){
    return endPos-startPos;
}

QString SnippetController::previousWord(){
    QRegularExpression whiteSpace("\\s+");
    QString word{""};
    QString::iterator it;


    int cursorPos = parent->textCursor().position();

    it = parent->toPlainText().begin()+cursorPos-1;

    if (it==parent->toPlainText().begin()){
        //Cursor is at the start of the text
        return "";
    }else if(!whiteSpace.match(*it).hasMatch()
             ){
        return "";
    }

    //Subtract one to move backwards past the whitespace
    it--;

    //Loop through the text
    while(it>=parent->toPlainText().begin()){

        //Once whitespace found terminate loop
        if (whiteSpace.match(*it).hasMatch()){
            break;
        }

        //Add the char to the word
        word.prepend(*it);
        it--;
    }
    return word;
}

QString SnippetController::getSnipContents(QString trigger){
    /*
     * TODO: Implement this function to read from a file
     * instead of just hardcoding the triggers
     */

    if(trigger == "dm"){
        return "\\[\n\t{$1}\n\\]{$2}";
    }
    else if (trigger=="beg"){
        return "\\begin\{{$1}}\n\t\n\\end\{{$2}}";
    }
    else if (trigger=="mk"){
        return "${$1}$";
    }

    return "";
}

void SnippetController::insertSnippet(){

    //Create a new snippet object
    currentSnippet = new Snippet;

    //Get a copy of the text cursor
    QTextCursor cursor{parent->textCursor()};

    int position{cursor.position()};


    //The trigger word for the snippet
    QString trigger{previousWord()};

    //The length of the trigger word
    int triggerLength{trigger.length()};

    //Get the snippet contents and load them into a snippet object
    currentSnippet->loadFromQString(getSnipContents(trigger));
    parent->setPlainText(parent->toPlainText().remove(position-triggerLength-1, triggerLength));
    cursor.setPosition(position-triggerLength-1);
    parent->setTextCursor(cursor);
    parent->insertPlainText(currentSnippet->text);
    cursor.setPosition(position-1-triggerLength+currentSnippet->stops.front());
    parent->setTextCursor(cursor);

    currentSnippet->startPos = position - triggerLength -1;
    currentSnippet->endPos = position + currentSnippet->text.length()-1;

    QTextStream(stdout) << "\nSnippet with trigger \"" << trigger <<"\""
                        << " has been inserted at: " << currentSnippet->startPos
                        << '\n';


}

void Snippet::loadFromQString(QString snipContents){


    for (int stopIndex = snipContents.indexOf(QRegularExpression("\\{\\$\\d\\}"));
         stopIndex!=-1;
         stopIndex = snipContents.indexOf(QRegularExpression("\\{\\$\\d\\}"))){

        stops.push_back(stopIndex);
        QTextStream(stdout) << "Adding stop " << stopIndex << '\n';
        snipContents.remove(stopIndex, 4);
    }
    text = snipContents;
    currentStop = stops.begin();
}
