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
    Graphics/physicals.cpp \
    Tests/tester.cpp \
    Tests/collisions.cpp \
    Tests/physics_utility.cpp \
    ECS/entity.cpp

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

HEADERS += \
    player.h \
    practicedummy.h \
    camera.h \
    utility.h \
    make_function.h \
    Physics/body.h \
    Physics/circle.h \
    Physics/physics_utility.h \
    Graphics/physicals.h \
    Physics/collision.h \
    Physics/aabb.h \
    Physics/line.h \
    Physics/shapes.h \
    Tests/tester.h \
    Tests/collisions.h \
    Tests/physics_utility.h \
    Tests/tests_utility.h \
    ECS/entity.h \
    ECS/system.h \
    ECS/systemiterator.h \
    ECS/ecs_impl.h

debug:QMAKE_CXXFLAGS += -Wall -Werror -std=c++14 -g
debug:QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-braces #-Wno-unused-parameter
#QMAKE_CFLAGS += -flto

DISTFILES += \
    TODO
