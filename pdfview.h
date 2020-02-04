#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <QGraphicsView>

class PdfView  : public QGraphicsView{

    Q_OBJECT

public:
    PdfView(QWidget* parent=0);
    virtual ~PdfView();

private:
    void keyPressEvent(QKeyEvent *event);

signals:
    void focusToggled();

public slots:
    void slotTest();
};


#endif // PDFVIEW_H
