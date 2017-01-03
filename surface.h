#ifndef SURFACE_H
#define SURFACE_H

#include <QObject>

class Surface : public QObject
{
    Q_OBJECT
public:
    explicit Surface(QObject *parent = 0);
    char demname[240] ;
    void setDEMName (const char *dname) ;
    int readSurface (int ss, int sl, int ssfac, bool sflag) ;
    int readSurface (float lon, float lat, int ssfac) ;


    int readSurface (int ssfac) ;
    //void updateSurface (float lat, float lon) ;
    void calcNormals () ;
    void crossProd (double x[], double y[], double *cross) ;
    void normalize (double *vec) ;
    void setCentLL (float lon, float lat) ;
    float *myverts ;
    float *mynorms ;
    int ssfac ;
    int nl, ns, pixPerDegree ;
    float latcent, loncent, lat0dem, lon0dem, llspace, gridspace, xzscale, yscale ;
signals:
    
public slots:
    
};

#endif // SURFACE_H
