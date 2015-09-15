#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T18:24:05
#
#-------------------------------------------------

TARGET = test
TEMPLATE = app

SOURCES += main.cpp \
    player.cpp \
    practicedummy.cpp \
    sensor.cpp \
    camera.cpp \
    weapon.cpp \
    hittable.cpp \
    enemylist.cpp \
    entity.cpp

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

HEADERS += \
    player.h \
    practicedummy.h \
    conversions.h \
    sensor.h \
    camera.h \
    weapon.h \
    utility.h \
    hittable.h \
    enemylist.h \
    components.h \
    entity.h

QMAKE_CXXFLAGS += -Wall -Werror -std=c++14 -Wno-unused-parameter -g
#QMAKE_CFLAGS += -flto

DISTFILES += \
    TODO
