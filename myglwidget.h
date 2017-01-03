#ifndef MyGLWidGET_H
#define MyGLWidGET_H

#include <QOpenGLWidget>

#include <QKeyEvent>
#include <GL/freeglut.h>
#include <GL/glu.h>

#include "surface.h"

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget () ;
    void paintGL () ;
    void initializeGL () ;
    void resizeGL (int w, int h) ;
    void loadSurface (Surface *s) ;
    void setSolarParams (float az, float elev) ;
    void setLanderParams (float fov, float az, float elev) ;
    void spin () ;
    void spin (bool ccw) ;
    void tilt (bool tiltUp) ;
    void shiftY (float inc) ;
    void keyPressEvent (QKeyEvent *) ;
    void loadVecs () ;
    bool shiftFlag ;
    float *verts, *norms, *light_position, rotAngY, rotAngZ, rotAngX, yTrans ;
    float xzscale, yscale ;
    float landerFOV ;
    Surface *srf ;
    float rotqube ;
signals:
    void changeLanderVal (float) ;
    void shiftYinc (float) ;

public slots:
};

#endif // MyGLWidGET_H
