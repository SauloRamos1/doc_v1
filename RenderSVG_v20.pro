#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T16:35:02
#
#-------------------------------------------------

QT       += core gui opengl svg quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RenderSVG_v01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    halfedge.cpp \
    openglwidget.cpp

HEADERS  += mainwindow.h \
    nanosvg.h \
    nanosvgrast.h \
    halfedge.h \
    openglwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

#DISTFILES += \
#   RenderSVG_v09.pro.user
