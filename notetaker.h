#ifndef NOTETAKER_H
#define NOTETAKER_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>


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

private:
    Ui::NoteTaker *ui;
    QString currentFile{""};
};
#endif // NOTETAKER_H
