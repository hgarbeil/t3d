#ifndef MYIMWIDGET_H
#define MYIMWIDGET_H

#include <QWidget>
#include <QImage>

class MyIMWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyIMWidget(QWidget *parent = 0);
    int nsamps, nlines ;
    float resX, resY, boxLon, boxLat,boxSize ;
    QString surfFile ;
    QImage *qim ;
    void setSurfFile (const char *, int ns, int nl) ;
    void setBoxCoords(float lon, float lat) ;
    void createQImage () ;
    void calcResolution();

protected :
    void paintEvent (QPaintEvent *event) ;
    void resizeEvent (QResizeEvent *event) ;

signals:

public slots:
};

#endif // MYIMWIDGET_H
