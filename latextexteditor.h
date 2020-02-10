#ifndef LATEXTEXTEDITOR_H
#define LATEXTEXTEDITOR_H

#include <QTextEdit>
#include <QTextStream>
#include <QtEvents>
#include <QWidget>
#include <QObject>
#include <memory>
#include "snippetcontroller.h"

class LatexTextEditor : public QTextEdit{
    Q_OBJECT

public:
    explicit LatexTextEditor(QWidget *parent = 0);
    virtual ~LatexTextEditor();

signals:
     void focusToggled();

public slots:
     void runSnippetController();

private:
    void keyPressEvent(QKeyEvent *event);

    std::unique_ptr<SnippetController> snipCont;
};

#endif // LATEXTEXTEDITOR_H
