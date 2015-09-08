#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T18:24:05
#
#-------------------------------------------------

TARGET = test
TEMPLATE = app

SOURCES += main.cpp \
    player.cpp \
    drawlist.cpp \
    logicalobject.cpp \
    updatelist.cpp \
    practicedummy.cpp \
    collidable.cpp \
    physicalcircle.cpp \
    physicalwall.cpp \
    sensor.cpp \
    camera.cpp \
    weapon.cpp \
    debugdraw.cpp

LIBS += -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -lBox2D

HEADERS += \
    player.h \
    drawlist.h \
    logicalobject.h \
    updatelist.h \
    practicedummy.h \
    collidable.h \
    conversions.h \
    physicalcircle.h \
    physicalwall.h \
    sensor.h \
    camera.h \
    weapon.h \
    utility.h \
    debugdraw.h

QMAKE_CXXFLAGS += -Wall -Werror -std=c++14

DISTFILES += \
    TODO
