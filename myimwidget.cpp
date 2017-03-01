#include "myimwidget.h"
#include <QPainter>
#include <QFile>

MyIMWidget::MyIMWidget(QWidget *parent) : QWidget(parent)
{
    surfFile = "" ;
    qim = 0L ;
    resX = .1 ;
    resY = .1 ;
    boxLon = 180. ;
    boxLat = 0. ;
    boxSize = 1400. / 64. ;
    //calcResolution() ;

}

void MyIMWidget::resizeEvent (QResizeEvent *event) {
    calcResolution() ;
}

void MyIMWidget::calcResolution () {
    int w, h ;
    w = this->width() ;
    h = this->height() ;
    resX =  360. / float(w) ;
    resY = 180./ float(h) ;
}

void MyIMWidget::setSurfFile (const char *sf, int ns, int nl) {
    surfFile = QString (sf) ;
    nsamps = ns ;
    nlines = nl ;
    createQImage() ;
}

void MyIMWidget::createQImage() {
    float *indat = new float [nsamps * nlines] ;
    unsigned char *udat = new unsigned char [nsamps * nlines * 4] ;
    QFile qf (surfFile) ;
    qf.open (QIODevice::ReadOnly ) ;
    qf.read ((char *)indat, 4 * nsamps * nlines) ;
    qf.close() ;
    for (int i=0; i<nsamps * nlines ; i++) {
        udat[i*4+3] = 200 ;
        udat[i*4+1] = (unsigned char) (int(indat[i] * 255.)) ;
        udat[i*4+2] = udat[i*4+1] ;
        udat[i*4]= udat[i*4+1] ;
    }


    qim = new QImage (udat, nsamps, nlines, QImage::Format_ARGB32) ;
    //qim->scaled (this->width(), this->height()) ;
    delete [] udat ;
}

void MyIMWidget::paintEvent (QPaintEvent *ev) {
    int w, h, xloc, yloc ;
    int nboxPix ;
    w = this->width() ;
    h = this->height() ;
    QPainter p (this) ;
    if (qim) {
        p.drawImage (QRect(0,0,nsamps, nlines),*qim) ;
        //p.drawImage (QRect(0,0,this->width(), this->height()), *qim, QRect(0,0, nsamps, nlines)) ;
    }
    xloc = boxLon / resX ;
    yloc = (90. - boxLat) / resY ;
    nboxPix = (int)(boxSize / resX) ;
    p.drawRect (QRect (xloc, yloc, xloc+nboxPix, yloc+nboxPix)) ;

}

void MyIMWidget::setBoxCoords (float lon, float lat) {
    boxLon = lon ;
    boxLat = lat ;
}
