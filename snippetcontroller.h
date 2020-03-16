#ifndef SNIPPETCONTROLLER_H
#define SNIPPETCONTROLLER_H

#include <QObject>
<<<<<<< HEAD
#include <list>
#include <vector>
#include "latextexteditor.h"
=======
#include "latextexteditor.h"
#include <list>
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375

class LatexTextEditor;
class Snippet;
class SnippetController;
<<<<<<< HEAD
struct Stop;
typedef std::vector<std::list<Stop>> StopList;

struct Stop{
    int start;
    int end;
=======

struct SnippetStop{
  int position;
  int size;
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375
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
<<<<<<< HEAD
    StopList stops{};
    int currentGroup;
    std::list<Stop> * currentStop{};
=======
    std::list<int> stops;
    std::list<int>::iterator currentStop;


>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375
};

class SnippetController: QObject
{
    Q_OBJECT

public:
    SnippetController(LatexTextEditor * parent);
    ~SnippetController();

    QString previousWord();
    QString getSnipContents(QString trigger);
<<<<<<< HEAD
    void insertSnippet(QString trigger);
=======
    void insertSnippet();
>>>>>>> 06d60f23343caa570edac0bcedf0b10de2389375

    LatexTextEditor * parent;
    Snippet * currentSnippet;


};



#endif // SNIPPETCONTROLLER_H
