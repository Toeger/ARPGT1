#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T18:24:05
#
#-------------------------------------------------

TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    player.cpp \
    practicedummy.cpp \
    camera.cpp \
    weapon.cpp \
    entity.cpp \
    Physics/body.cpp \
    Physics/circle.cpp \
    Physics/utility.cpp \
    Graphics/physicals.cpp

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

HEADERS += \
    player.h \
    practicedummy.h \
    camera.h \
    weapon.h \
    utility.h \
    entity.h \
    make_function.h \
    Physics/body.h \
    Physics/circle.h \
    Physics/physics_utility.h \
    Graphics/physicals.h \
    Physics/collision.h \
    Physics/aabb.h

QMAKE_CXXFLAGS += -Wall -Werror -std=c++14 -g
QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-braces #-Wno-unused-parameter
#QMAKE_CFLAGS += -flto

DISTFILES += \
    TODO
