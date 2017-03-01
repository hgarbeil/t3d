#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    sf = new Surface () ;
    sf->setDEMName ("/Users/hg/kil_lidar_1m") ;
    sf->setCentLL (70., 45.) ;
    //sf->readSurface(2300., 1450.,1) ;
    sf->readSurface (20., 45., 1) ;
    sf->calcNormals() ;
    ui->setupUi(this);
    ui->widget->loadSurface(sf);
    ui->imwidget->setSurfFile("/Users/hg/data/lola_shade", 1152,576) ;
    ui->imwidget->setBoxCoords(20., 45.) ;
    ui->imwidget->repaint() ;
}

MainWindow::~MainWindow()
{
    delete ui;
}
