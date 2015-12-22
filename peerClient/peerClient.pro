#-------------------------------------------------
#
# Project created by QtCreator 2015-10-07T23:09:15
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = peerClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serverinfo.cpp \
    commands.cpp \
    thread.cpp \
    tcpserver.cpp \
    threaddownloader.cpp \
    rtttimer.cpp

HEADERS  += mainwindow.h \
    commands.h \
    serverinfo.h \
    thread.h \
    tcpserver.h \
    threaddownloader.h \
    rtttimer.h

FORMS    += mainwindow.ui \
    serverinfo.ui
