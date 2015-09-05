#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T18:24:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp \
    player.cpp \
    drawlist.cpp \
    logicalobject.cpp \
    updatelist.cpp

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

HEADERS += \
    player.h \
    drawlist.h \
    logicalobject.h \
    updatelist.h

CONFIG += c++14
QMAKE_CXXFLAGS += -Wall -Werror
