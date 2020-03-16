#ifndef SNIPPETCONTROLLER_H
#define SNIPPETCONTROLLER_H

#include <QObject>
#include <list>
#include <vector>
#include "latextexteditor.h"

class LatexTextEditor;
class Snippet;
class SnippetController;
struct Stop;
typedef std::vector<std::list<Stop>> StopList;

struct Stop{
    int start;
    int end;
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
    StopList stops{};
    int currentGroup;
    std::list<Stop> * currentStop{};
};

class SnippetController: QObject
{
    Q_OBJECT

public:
    SnippetController(LatexTextEditor * parent);
    ~SnippetController();

    QString previousWord();
    QString getSnipContents(QString trigger);
    void insertSnippet(QString trigger);

    LatexTextEditor * parent;
    Snippet * currentSnippet;


};



#endif // SNIPPETCONTROLLER_H
