QT += core
QT += network
QT -= gui

TARGET = peerServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    commands.cpp \
    udpserver.cpp \
    rtttimer.cpp

HEADERS += \
    commands.h \
    udpserver.h \
    rtttimer.h

