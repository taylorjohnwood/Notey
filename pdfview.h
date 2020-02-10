#ifndef PDFVIEW_H
#define PDFVIEW_H
#include <QWidget>
#include <QObject>
#include <QTextStream>
#include <QtEvents>
#include <QGraphicsView>
#include <poppler/qt5/poppler-qt5.h>
//#include <poppler/glib/poppler-document.h>
#include <memory>
#include <QScrollBar>

std::unique_ptr<Poppler::Document> loadPdf(QString path);

class PdfView  :public QGraphicsView{

    Q_OBJECT

public:
    PdfView(QWidget* parent=0);
    virtual ~PdfView();

    void setDocument(std::unique_ptr<Poppler::Document>& document);
    void displayPage(int pageNumber);

    int currentPage;

private:
    std::unique_ptr<Poppler::Document> document;
    double zoom;
    QGraphicsScene pageScene;
    void keyPressEvent(QKeyEvent *event);
    void displayPixmap(QPixmap pagePixmap);


signals:
    void focusToggled();

};


#endif // PDFVIEW_H
