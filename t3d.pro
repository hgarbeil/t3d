#-------------------------------------------------
#
# Project created by QtCreator 2016-12-23T09:12:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

TARGET = t3d
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myglwidget.cpp \
    surface.cpp

HEADERS  += mainwindow.h \
    myglwidget.h \
    surface.h

FORMS    += mainwindow.ui

INCLUDEPATH += /opt/X11/include
