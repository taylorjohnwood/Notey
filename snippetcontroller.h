#ifndef SNIPPETCONTROLLER_H
#define SNIPPETCONTROLLER_H

#include <QObject>
#include "latextexteditor.h"
#include <list>

class LatexTextEditor;
class Snippet;
class SnippetController;

struct SnippetStop{
  int position;
  int size;
};

class Snippet{

public:
    Snippet();
    ~Snippet();


    void loadFromQString(QString snipContents);
    int startPos{-1};
    int endPos{-1};

    int size();
    QString text{""};
    std::list<int> stops;
    std::list<int>::iterator currentStop;


};

class SnippetController: QObject
{
    Q_OBJECT

public:
    SnippetController(LatexTextEditor * parent);
    ~SnippetController();

    QString previousWord();
    QString getSnipContents(QString trigger);
    void insertSnippet();

    LatexTextEditor * parent;
    Snippet * currentSnippet;


};



#endif // SNIPPETCONTROLLER_H
