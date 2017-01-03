#include "surface.h"
#include <stdio.h>
#include <QFile>
#include <math.h>


Surface::Surface(QObject *parent) :
    QObject(parent)
{

    // characteristics of the dem
    lat0dem = 90.f ;
    lon0dem = -180.f ;
    pixPerDegree = 64 ;
    // 1/64th dem
    llspace = 1.f / pixPerDegree ;
    // translates to1 m dem
    gridspace = 1. ;
    ssfac = 1 ;
    strcpy (demname, "/Users/hg/kil_lidar_1m") ;
    xzscale = 100. ;
    yscale = 10000. ;


}


/*
int Surface::readEnviHeader (QString qstr) {
    bool bval ;
       QStringList slist ;
       QFile *file = new QFile (hdrName) ;
       if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        while (!file->atEnd()) {
            QTextStream in(file);
              while (!in.atEnd()) {
                   QString line = in.readLine();
                   if (line.contains("=")) {
                       if (line.contains ("lines")) {
                           slist = line.split("=") ;
                           nldem = slist[1].toInt(&bval) ;
                       }

                       if (line.contains ("sampl")) {
                           slist = line.split("=") ;
                           nsdem = slist[1].toInt(&bval) ;
                       }
                       if (line.contains ("bands")) {
                           slist = line.split("=") ;
                           nb = slist[1].toInt(&bval) ;
                       }
                       if (line.contains ("data")) {
                           slist = line.split("=") ;
                           dt = slist[1].toInt(&bval) ;
                       }
                       if (line.contains ("map info")){

                       }
                   }
               }
        }



}

*/

void Surface::setDEMName(const char *dname) {
    strcpy (demname, dname) ;
}




int Surface::readSurface (float lon, float lat, int ssfac) {
    int i, j, is ;
    int startline, startsamp ;
    int nskil, nlkil ;
    float zval, landerElev, landerScaleElev ;
    this->setDEMName ("/Users/hg/data/lola") ;

    nskil = 23040 ;
    nlkil = nskil / 2 ;


    startsamp = int (lon * 64.) ;
    startline = int ((90. - lat) * 64.) ;


    // assume global dem

    ns = 1400 ;
    nl = 1400 ;
    this->ssfac = ssfac ;


    QFile qf (demname) ;
    qf.open (QIODevice::ReadOnly) ;
    //startline = sl - nl / 2 ;
    //startsamp = ss - ns / 2 ;

    myverts = new float [ns * nl * 3] ;
    mynorms = new float [ns * nl * 3] ;
    short * linedat= new short [nskil] ;
    float minzval=1.E99, minxval=1.E99, minyval=1.E99;
    float maxzval=-1.E99, maxxval=-1.E99, maxyval=-1.E99;

    // read in the elevation from the moon dem
    qf.seek (startline * nskil * 2) ;
    for (i=0; i<nl; i++){
        qf.read ((char *)&linedat[0], nskil * 2) ;
        zval = (i-nl/2) * ssfac * gridspace ;
        for (j=0; j<ns; j++){
            myverts [i * ns * 3 + j * 3] = ((j - ns/2)* ssfac * gridspace)/xzscale  ;
            myverts [i * ns * 3 + j * 3 + 1] = double(linedat [startsamp + j * ssfac])/yscale ;
            myverts [i * ns * 3 + j * 3 + 2] = zval/xzscale  ;

            if (myverts [i * ns * 3 + j * 3 + 0] > maxxval) maxxval = myverts [i * ns * 3 + j * 3 + 0] ;
            if (myverts [i * ns * 3 + j * 3 + 0] < minxval) minxval = myverts [i * ns * 3 + j * 3 + 0] ;
            if (myverts [i * ns * 3 + j * 3 + 2] > maxzval) maxzval = myverts [i * ns * 3 + j * 3 + 2] ;
            if (myverts [i * ns * 3 + j * 3 + 2] < minzval) minzval = myverts [i * ns * 3 + j * 3 + 2] ;
            if (i==700 && j==700) {
                landerElev =  double(linedat [startsamp + j * ssfac]) ;
                landerScaleElev = double(linedat [startsamp + j * ssfac])/yscale ;
            }
        }
        for (is=1; is<ssfac ; is++) qf.read ((char *)&linedat[0], nskil * 2) ;
    }

    for (i=0; i<1400; i++){
        for (j=0; j<1400; j++) {
            myverts [i * 1400 * 3 + j * 3 + 1] -= landerScaleElev ;
            if (myverts [i * 1400 * 3 + j * 3 + 1] > maxyval) maxyval = myverts [i * 1400 * 3 + j * 3 + 1] ;
            if (myverts [i * 1400 * 3 + j * 3 + 1] < minyval) minyval = myverts [i * 1400 * 3 + j * 3 + 1] ;
        }
    }

    printf ("Y Min and max %f \t %f\r\n",minyval, maxyval) ;
    printf ("X Min and max %f \t %f\r\n",minxval, maxxval) ;
    printf ("Z Min and max %f \t %f\r\n",minzval, maxzval) ;

    return (1) ;


}



int Surface::readSurface(int ss, int sl, int ssfac, bool sflag) {

    int i, j, is ;
    int nskil, nlkil, startline, startsamp ;
    float zval, landerElev, landerScaleElev ;


    // assume global dem
    nskil = 7750 ;
    nlkil = 4942 ;
    ns = 1400 ;
    nl = 1400 ;
    this->ssfac = ssfac ;


    QFile qf (demname) ;
    qf.open (QIODevice::ReadOnly) ;
    startline = sl - nl / 2 ;
    startsamp = ss - ns / 2 ;

    myverts = new float [ns * nl * 3] ;
    mynorms = new float [ns * nl * 3] ;
    float * linedat= new float [nskil] ;
    float minzval=1.E99, minxval=1.E99, minyval=1.E99;
    float maxzval=-1.E99, maxxval=-1.E99, maxyval=-1.E99;

    // read in the elevation from the moon dem
    qf.seek (startline * nskil * 4) ;
    for (i=0; i<nl; i++){
        qf.read ((char *)&linedat[0], nskil * 4) ;
        zval = (i-nl/2) * ssfac * gridspace ;
        for (j=0; j<ns; j++){
            myverts [i * ns * 3 + j * 3] = ((j - ns/2)* ssfac * gridspace)/xzscale  ;
            myverts [i * ns * 3 + j * 3 + 1] = double(linedat [startsamp + j * ssfac])/yscale ;
            myverts [i * ns * 3 + j * 3 + 2] = zval/xzscale  ;

            if (myverts [i * ns * 3 + j * 3 + 0] > maxxval) maxxval = myverts [i * ns * 3 + j * 3 + 0] ;
            if (myverts [i * ns * 3 + j * 3 + 0] < minxval) minxval = myverts [i * ns * 3 + j * 3 + 0] ;
            if (myverts [i * ns * 3 + j * 3 + 2] > maxzval) maxzval = myverts [i * ns * 3 + j * 3 + 2] ;
            if (myverts [i * ns * 3 + j * 3 + 2] < minzval) minzval = myverts [i * ns * 3 + j * 3 + 2] ;
            if (i==700 && j==700) {
                landerElev =  double(linedat [startsamp + j * ssfac]) ;
                landerScaleElev = double(linedat [startsamp + j * ssfac])/yscale ;
            }
        }
        for (is=1; is<ssfac ; is++) qf.read ((char *)&linedat[0], nskil * 4) ;
    }
    for (i=0; i<1400; i++){
        for (j=0; j<1400; j++) {
            myverts [i * 1400 * 3 + j * 3 + 1] -= landerScaleElev ;
            if (myverts [i * 1400 * 3 + j * 3 + 1] > maxyval) maxyval = myverts [i * 1400 * 3 + j * 3 + 1] ;
            if (myverts [i * 1400 * 3 + j * 3 + 1] < minyval) minyval = myverts [i * 1400 * 3 + j * 3 + 1] ;
        }
    }
    printf ("Y Min and max %f \t %f\r\n",minyval, maxyval) ;
    printf ("X Min and max %f \t %f\r\n",minxval, maxxval) ;
    printf ("Z Min and max %f \t %f\r\n",minzval, maxzval) ;

    return (1) ;

}

int Surface::readSurface(int ssfac) {

    int i, j, is ;
    int nskil, nlmoon, startline, startsamp ;
    float zval, landerElev, landerScaleElev ;


    // assume global dem
    nskil = pixPerDegree * 360 ;
    nlmoon = pixPerDegree * 180 ;
    ns = 1400 ;
    nl = 1400 ;
    this->ssfac = ssfac ;


    QFile qf (demname) ;
    qf.open (QIODevice::ReadOnly) ;
    startline = (int)((lat0dem - latcent) * pixPerDegree) ;
    startsamp = (int)((loncent - lon0dem) * pixPerDegree) ;

    myverts = new float [1400 * 1400 * 3] ;
    mynorms = new float [1400 * 1400 * 3] ;
    float * linedat= new float [nskil] ;
    float minzval=1.E99, minxval=1.E99, minyval=1.E99;
    float maxzval=-1.E99, maxxval=-1.E99, maxyval=-1.E99;

    // read in the elevation from the moon dem
    qf.seek (startline * nskil * 4) ;
    for (i=0; i<1400; i++){
        qf.read ((char *)&linedat[0], nskil * 4) ;
        zval = (700 - i) * ssfac * gridspace ;
        for (j=0; j<1400; j++){
            myverts [i * 1400 * 3 + j * 3] = ((j - 700)* ssfac * gridspace)/50000.  ;
            myverts [i * 1400 * 3 + j * 3 + 1] = double(linedat [startsamp + j * ssfac])/50000. ;
            myverts [i * 1400 * 3 + j * 3 + 2] = zval/25000.  ;
            if (myverts [i * 1400 * 3 + j * 3 + 1] > maxyval) maxyval = myverts [i * 1400 * 3 + j * 3 + 1] ;
            if (myverts [i * 1400 * 3 + j * 3 + 1] < minyval) minyval = myverts [i * 1400 * 3 + j * 3 + 1] ;
            if (myverts [i * 1400 * 3 + j * 3 + 0] > maxxval) maxxval = myverts [i * 1400 * 3 + j * 3 + 0] ;
            if (myverts [i * 1400 * 3 + j * 3 + 0] < minxval) minxval = myverts [i * 1400 * 3 + j * 3 + 0] ;
            if (myverts [i * 1400 * 3 + j * 3 + 2] > maxzval) maxzval = myverts [i * 1400 * 3 + j * 3 + 2] ;
            if (myverts [i * 1400 * 3 + j * 3 + 2] < minzval) minzval = myverts [i * 1400 * 3 + j * 3 + 2] ;
            if (i==700 && j==700) {
                landerElev =  double(linedat [startsamp + j * ssfac]) ;
                landerScaleElev = double(linedat [startsamp + j * ssfac])/25000. ;
            }
        }
        for (is=1; is<ssfac ; is++) qf.read ((char *)&linedat[0], nskil * 2) ;
    }

    for (i=0; i<1400; i++){
        for (j=0; j<1400; j++) {
            myverts [i * 1400 * 3 + j * 3 + 1] -= landerScaleElev ;
        }
    }
    printf ("Y Min and max %f \t %f\r\n",minyval, maxyval) ;
    printf ("X Min and max %f \t %f\r\n",minxval, maxxval) ;
    printf ("Z Min and max %f \t %f\r\n",minzval, maxzval) ;

    return (1) ;

}

void Surface::setCentLL (float lon, float lat) {
    latcent = lat ;
    loncent = lon ;
}

void Surface::calcNormals () {

    int i, j ;

    double xvec[3], zvec[3], cross[3], leftelev, rightelev, botelev, topelev ;
    for (i=0; i< nl  ; i++){
        for (j=0; j<ns ; j++){

            rightelev = myverts [i*ns*3+(j+1)*3+1]  ;
            leftelev = myverts [i*ns*3+j*3+1]   ;
            topelev =  myverts [i*ns*3+j*3+1]  ;
            botelev =  myverts [(i+1)*ns*3+j*3+1]  ;
            xvec [0] = (ssfac * gridspace)/ xzscale ;
            xvec [2] = 0. ;
            xvec[1] = rightelev - leftelev ;
            zvec [2] = 1 * ssfac * gridspace / xzscale ;
            zvec [0] = 0. ;
            zvec[1] = 1. * (topelev - botelev) ;
            crossProd (zvec, xvec, cross) ;
            for (int is=0; is<3; is++) mynorms[i*(ns)*3+j*3+is] = cross[is] ;
        }
    }

}

void Surface::crossProd (double x[], double y[], double *cross){

    QString str ;
    cross[0] = x[1] * y[2] - x[2] * y[1] ;
    cross[1] = x[2] * y[0] - x[0] * y[2] ;
    cross[2] = x[0] * y[1] - x[1] * y[0] ;
    normalize (cross) ;
    //for (int iss=0; iss<3; iss++) str.sprintf ("%f %f %f\r\n", cross[0], cross[1], cross[2]) ;
    //qDebug (str.toLatin1().data()) ;
    int hg ;
    hg =1 ;

}

void Surface::normalize (double *vec){

    int i ;
    double length=0. ;

    for (i=0; i<3; i++) length += (vec[i] * vec[i]) ;
    length = sqrt (length) ;
    for (i=0; i<3; i++) vec[i]/=length ;

}

