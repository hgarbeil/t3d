
#include "MyGLWidget.h"
#include <math.h>
#define DEG2RAD 0.017453293


float mat_specular [] = {1., 1., 1., 1.} ;
float diffuse_material [] = {.9, .9, .9, 1.} ;
float ambient_light [] = {.4, .4, .4, 1.} ;
float source_light [] = {1., 1., 1., 1.} ;

MyGLWidget::MyGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    rotAngY = 00. ;
    rotAngX = 0. ;
    rotAngZ = 0. ;
    yTrans =  1 ;
    //xzscale = 50000. ;
    yscale = 100. ;
    xzscale = 1 ;
    //yscale = 1 ;
    landerFOV = 55. ;
    shiftFlag = false ;
    light_position = new float [4] ;
    *light_position = 0.1f ;
    *(light_position+1) = 0.3f ;
    *(light_position+2) = 0.3f ;
    *(light_position+3) = 0.0f ;
    this->setFocusPolicy (Qt::StrongFocus) ;
}



MyGLWidget::~MyGLWidget (){
    delete [] light_position ;
}

void MyGLWidget::initializeGL () {


    glClearColor (0., 0., .7, 1.) ;
    glEnable (GL_DEPTH_TEST) ;
    glEnable (GL_COLOR_MATERIAL) ;
    glEnable (GL_LIGHTING) ;
    glEnable (GL_LIGHT0);
    //glEnable (GL_LIGHT1) ;
    //glEnable (GL_NORMALIZE) ;



}




void MyGLWidget::paintGL () {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





    //Add ambient light
    GLfloat ambientColor[] = {0.1f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //Directional light (ie Sun) will rotate with any surface rotation or tilting.
    GLfloat lightColor0[] = {0.79f, 0.7f, 0.6f, 1.0f}; //Color (0.5, 0.5, 0.5)

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    //Leaving the eye at (0,0,0), move the model 15 units into the distance

    glTranslatef (0., -yTrans, 0.) ;
    glRotatef (rotAngX, 1., 0., 0.) ;
    glRotatef (rotAngY, 0, 1, 0.) ;
    glRotatef (rotAngZ, 0., 0., 1.) ;


    // load polygons - vertices and normals which are defined in the Surface class
    glBegin (GL_QUADS) ;

    loadVecs () ;

    glFlush () ;
    glEnd();



    rotqube += .9f ;
}


void MyGLWidget::setLanderParams (float fov, float az, float elev) {

    int w, h ;
    landerFOV = fov ;
    this->updateGeometry () ;
    rotAngY = az ;

    w= this->width () ;
    h= this->height() ;
    glMatrixMode (GL_PROJECTION) ;
    glLoadIdentity () ;
    gluPerspective (landerFOV, float(w)/float(h), 0.0001, 25.);
    glMatrixMode (GL_MODELVIEW) ;

    yTrans = elev / yscale ;
}

void MyGLWidget::setSolarParams (float az, float elev){

    double azd, elevd ;
    azd = az * DEG2RAD ;
    elevd = elev * DEG2RAD ;

    light_position [0] = sin(azd) * cos (elevd) ;
    light_position [1] = sin (elevd) ;
    light_position [2] = cos (azd) * cos (elevd) ;

    repaint () ;

}

void MyGLWidget::loadSurface(Surface *s) {
    int i, is, j, ns, nl, sampnum, nPolys, nVerts  ;
    float scale ;
    ns = s->ns ;
    nl = s->nl ;
    srf = s ;
    nPolys = (srf->ns-1) * (srf->nl-1) ;
    nVerts = ns * nl ;
    verts = new float [nVerts*3] ;
    norms = new float [nVerts * 3] ;

    for (i=0; i<nl; i++){
        for (j=0; j<ns; j++){

            sampnum = i * ns + j ;
            for (is=0; is<3; is++) {
                //scale = xzscale ;
                //if (is== 1) scale = yscale ;
                    verts [sampnum * 3+is] = srf->myverts[i * ns* 3+j*3+is] ;
                    norms [sampnum * 3+is] = srf->mynorms[i*(ns)*3+j*3+is]  ;
                }



            }
        }

}


void MyGLWidget::loadVecs (){
    int i, j, ns, nl ;
    ns = srf->ns ;
    nl = srf->nl ;
    QString str ;
//glColor3f( .5,.5,.5) ;
glFrontFace (GL_CW) ;

    for (i=0; i<nl-1; i+=1) {
        for (j=0; j<ns-1; j+=1) {
            ;

            glNormal3f ( norms[i*ns*3+j*3], norms[i*srf->ns*3+j*3+1], norms[i*srf->ns*3+j*3+2])  ;
            //glNormal3f (0., 1., 1.)  ;
            glVertex3f (verts[i*srf->ns*3+j*3], verts[i*srf->ns*3+j*3+1], verts[i*srf->ns*3+j*3+2] ) ;
            //glNormal3f (norms[i*ns*3+(j+1)*3],norms[i*srf->ns*3+(j+1)*3+1],norms[i*srf->ns*3+(j+1)*3+2])  ;
            glVertex3f (verts[i*srf->ns*3+(j+1)*3], verts[i*srf->ns*3+(j+1)*3+1], verts[i*srf->ns*3+(j+1)*3+2] ) ;
            //glNormal3f (norms[(i+1)*ns*3+(j+1)*3],norms[(i+1)*srf->ns*3+(j+1)*3+1],norms[(i+1)*srf->ns*3+(j+1)*3+2])  ;
            glVertex3f (verts[(i+1)*srf->ns*3+(j+1)*3], verts[(i+1)*srf->ns*3+(j+1)*3+1], verts[(i+1)*srf->ns*3+(j+1)*3+2] ) ;
            //glNormal3f (norms[(i+1)*ns*3+(j)*3],norms[(i+1)*srf->ns*3+(j)*3+1],norms[(i+1)*srf->ns*3+(j)*3+2])  ;
            glVertex3f (verts[(i+1)*srf->ns*3+(j)*3], verts[(i+1)*srf->ns*3+(j)*3+1], verts[(i+1)*srf->ns*3+(j)*3+2] ) ;

        }
    }

}

void MyGLWidget::resizeGL (int w, int h) {



            glViewport (0, 0, w, h) ;
            glMatrixMode (GL_PROJECTION) ;
            glLoadIdentity () ;
            gluPerspective (landerFOV, float(w)/float(h), 0.0001, 25.);
            glMatrixMode (GL_MODELVIEW) ;

}

void MyGLWidget::spin (){
    bool cwflag = false ;
    if (cwflag) rotAngY -=1 ;
    else rotAngY+= 1 ;
    this->repaint() ;
}

void MyGLWidget::shiftY (float inc) {

    yTrans += (inc / yscale) ;
    repaint () ;


}

void MyGLWidget::spin (bool spinCCW){

    if (spinCCW) {
        rotAngY -=1 ;
        emit (this->changeLanderVal (-1.)) ;
    }
    else {
        rotAngY+= 1 ;
        emit(this->changeLanderVal (1.)) ;
    }
    this->repaint() ;

}

void MyGLWidget::tilt (bool tiltUp){

    if (tiltUp) rotAngX -=1 ;
    else rotAngX+= 1 ;
    this->repaint() ;
}


void MyGLWidget::keyPressEvent (QKeyEvent *ev){


               switch( ev->key() )
               {
               case Qt::Key_Right :
                   spin(false) ;
                   break ;
               case Qt::Key_Left :
                   spin(true) ;
                   break ;
               case Qt::Key_R:
                   if (shiftFlag)spin (true) ;
                   else spin (false) ;
                   shiftFlag = false ;
                   break ;
               case Qt::Key_D :
                   if (shiftFlag)tilt (true) ;
                   else tilt (false) ;
                   shiftFlag = false ;
                   break ;
                case Qt::Key_Up :
                   shiftY (10.) ;
                   emit (shiftYinc (10) ) ;
                   break ;
               case Qt::Key_Down :
                   shiftY (-10.) ;
                   emit (shiftYinc (-10) ) ;
                  break ;

                case Qt::Key_Shift :
                   shiftFlag = true ;
                   break ;

               default:
                   shiftFlag = false ;
                   break ;

               }


}
