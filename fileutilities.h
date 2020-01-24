#ifndef PDFDOCUMENT_H
#define PDFDOCUMENT_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <vector>
#include <iostream>
#include <poppler/qt5/poppler-qt5.h>

Poppler::Document* loadDocument(QString path);
std::vector<QPixmap>* loadPagesAsPixmap(Poppler::Document* document);
void createDocumentFromTex(QString path);
#endif // PDFDOCUMENT_H
