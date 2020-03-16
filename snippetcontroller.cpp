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
<<<<<<< HEAD
    else if (trigger=="ibeg"){
        return "\\begin\{itemize}\n\\item\n\t{$1}\n\\end\{itemize}";
    }
    else if (trigger=="ebeg"){
        return "\\begin\{enumerate}\n\\item\n\t{$1}\n\\end\{enumerate}";
    }
    else if (trigger=="abeg"){
        return "\\begin\{align*}\n\t{$1}\n\\end\{align*}";
    }
    else if (trigger=="ii"){
        return "\\item\n\t{$1}";
=======
    else if (trigger=="mk"){
        return "${$1}$";
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375
    }

    return "";
}

<<<<<<< HEAD
void SnippetController::insertSnippet(QString trigger){

    QString contents = getSnipContents(trigger);
    if (contents == ""){
        return;
    }

    //Delete previous snippet from memory
    if (currentSnippet){
        delete currentSnippet;
    }
=======
void SnippetController::insertSnippet(){
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375

    //Create a new snippet object
    currentSnippet = new Snippet;

<<<<<<< HEAD
    // Get the snippet contents and load them into a snippet object
    // If there is no snippet for the key word then exit function

    currentSnippet->loadFromQString(contents);

    //Get a copy of the text cursor
    QTextCursor cursor{parent->textCursor()};
    int position{cursor.position()};

    //The length of the trigger word
    int triggerLength{trigger.length()};

    // Delete the trigger word
    parent->setPlainText(parent->toPlainText().remove(position-triggerLength-1, triggerLength+1));

    //Move the cursor to where the trigger was
    cursor.setPosition(position-triggerLength-1);
    parent->setTextCursor(cursor);

    // Mark where the snippet starts
    currentSnippet->startPos = cursor.position();

    //Insert the snippets text
    parent->insertPlainText(currentSnippet->text);

    // Mark where the snippet ends
    currentSnippet->endPos = cursor.position();

    //Move the cursor to the first stop
    cursor.setPosition(currentSnippet->startPos + currentSnippet->stops[0].begin()->start);
    parent->setTextCursor(cursor);
}

void Snippet::loadFromQString(QString snipContents){
    /* TODO:
     * Implement this function with the new snippet stop system
     */

    //The position of where to slice the string as it's searched
    int stopNumber{};

    for(int stopPos = snipContents.indexOf(QRegularExpression("\{\\$\\d\\}"));
        stopPos != -1;
        stopPos = snipContents.indexOf(QRegularExpression("\{\\$\\d\\}"))){


        // The number that the user assigned to this stop
        stopNumber = snipContents.at(stopPos+2).digitValue();

        snipContents.remove(stopPos,4);

        Stop stop = Stop();
        stop.start = stopPos,
        stop.end = stopPos;



        //Add the stop to the stop list
        if ((int)stops.size() < stopNumber){
            stops.push_back(std::list<Stop>());
        }

        stops[stopNumber-1].push_back(stop);

    }
    text = snipContents;
=======
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
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375
}
