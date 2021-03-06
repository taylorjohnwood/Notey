#ifndef NOTETAKER_H
#define NOTETAKER_H

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QFile>
#include <QtGlobal>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QTextBlock>
#include <QCompleter>
#include <QLineEdit>
#include <vector>
#include <poppler/qt5/poppler-qt5.h>
#include <memory>
#include <highlighter.h>

#include "fileutilities.h"
#include "stringutilities.h"
#include "ui_notetaker.h"
#include "pdfview.h"
#include "latextexteditor.h"



QT_BEGIN_NAMESPACE
namespace Ui { class NoteTaker; }
QT_END_NAMESPACE

class NoteTaker : public QMainWindow
{
    Q_OBJECT

public:
    NoteTaker(QWidget *parent = nullptr);
    ~NoteTaker();



private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_actionToggle_PDF_View_triggered();

    void on_actionFile_Tree_triggered();

    void on_actionToggle_Console_triggered();

    void on_actionAdd_Subject_triggered();

    void on_actionAdd_Chapter_triggered();



private:

    Ui::NoteTaker* ui;
    Highlighter *highlighter;
    QString currentFile{""};
    QInputDialog *envInput;
    QCompleter *envCompleter;
    double zoom;


//    std::unique_ptr<Poppler::Document> document;
    std::unique_ptr<std::vector<QPixmap>> pages;
    std::unique_ptr<QGraphicsScene> scene{};

    void setPdfFromFilename(QString filename);
    void setPdfView(QPixmap pixmap);
    void initGUI();
    void initEnvironmentInput();
    void setupSignalsAndSlots();


};
#endif // NOTETAKER_H
