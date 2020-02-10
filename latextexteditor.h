#ifndef LATEXTEXTEDITOR_H
#define LATEXTEXTEDITOR_H

#include <QTextEdit>
#include <QTextStream>
#include <QtEvents>
#include <QWidget>
#include <QObject>
class LatexTextEditor : public QTextEdit{
    Q_OBJECT

public:
    explicit LatexTextEditor(QWidget *parent = 0);
    virtual ~LatexTextEditor();

signals:
     void focusToggled();

private:
    void keyPressEvent(QKeyEvent *event);
};

#endif // LATEXTEXTEDITOR_H
