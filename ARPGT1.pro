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
    entity.cpp \
    zombieai.cpp \
    physical.cpp \
    components.cpp \
    bullet.cpp \
    interactiveobject.cpp \
    Physics/body.cpp \
    Physics/circle.cpp \
    Physics/utility.cpp \
    Graphics/physicals.cpp

LIBS += -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -lBox2D

HEADERS += \
    player.h \
    practicedummy.h \
    conversions.h \
    sensor.h \
    camera.h \
    weapon.h \
    utility.h \
    hittable.h \
    components.h \
    entity.h \
    zombieai.h \
    physical.h \
    bullet.h \
    interactiveobject.h \
    make_function.h \
    Physics/body.h \
    Physics/circle.h \
    Physics/physics_utility.h \
    Graphics/physicals.h

QMAKE_CXXFLAGS += -Wall -Werror -std=c++14 -g
QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-braces #-Wno-unused-parameter
#QMAKE_CFLAGS += -flto

DISTFILES += \
    TODO
