#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <vector>
#include <iostream>
#include <memory>
#include <poppler/qt5/poppler-qt5.h>



std::unique_ptr<std::vector<QPixmap>> loadPagesAsPixmap(std::unique_ptr<Poppler::Document> &document, double &zoom);

void createPdfFromTex(QString path);

int saveTextToFile(QString text, QString path);
#endif // PDFDOCUMENT_H
