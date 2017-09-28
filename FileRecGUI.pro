#-------------------------------------------------
#
# Project created by QtCreator 2014-06-03T08:47:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileRecGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Core.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    Core.h \
    about.h

FORMS    += mainwindow.ui \
    about.ui

RESOURCES += \
    Resources.qrc
