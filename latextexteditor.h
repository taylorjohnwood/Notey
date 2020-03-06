#ifndef LATEXTEXTEDITOR_H
#define LATEXTEXTEDITOR_H

#include <QTextEdit>
#include <QTextStream>
#include <QtEvents>
#include <QWidget>
#include <QObject>
#include <memory>
#include <QRegularExpression>
#include <snippetcontroller.h>

class SnippetController;
class Snippet;

class LatexTextEditor : public QTextEdit{
    Q_OBJECT

public:
    explicit LatexTextEditor(QWidget *parent = 0);
    virtual ~LatexTextEditor();
    QString previousWord(int curPos);

    int previousPos;
    int previousBufferSize;

signals:
     void focusToggled();

public slots:
     void onTextChanged();

private:
    void keyPressEvent(QKeyEvent *event);
    void trackCursorPosition();
    std::unique_ptr<SnippetController> snipCont;


};

#endif // LATEXTEXTEDITOR_H
