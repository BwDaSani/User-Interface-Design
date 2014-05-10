#-------------------------------------------------
#
# Project created by QtCreator 2014-03-22T10:24:13
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    boximage.cpp \
    addimagecommand.cpp \
    deleteallcommand.cpp \
    addcollectioncommand.cpp \
    deleteimagecommand.cpp \
    flickrcollector.cpp \
    imagecollector.cpp

HEADERS  += mainwindow.h \
    boximage.h \
    addimagecommand.h \
    deleteimagecommand.h \
    addcollectioncommand.h \
    deleteallcommand.h \
    flickrcollector.h \
    imagecollector.h

FORMS    += mainwindow.ui
