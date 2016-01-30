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
    Tests/ecs_test.cpp \
    Tests/include_tester.cpp \
    ECS/entity_base.cpp \
    Graphics/textures.cpp \
    Graphics/animation.cpp

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

HEADERS += \
    ECS/entity.h \
    ECS/system.h \
    ECS/systemiterator.h \
    ECS/ecs_impl.h \
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
    Utility/asserts.h \
    Physics/sensor.h \
    Tests/ecs_test.h \
    ECS/entity_base.h \
    ECS/entity_handle.h \
    ECS/common_components.h \
    Graphics/textures.h \
    Graphics/animation.h

debug:QMAKE_CXXFLAGS += -Wall -Werror -std=c++1z -g
debug:QMAKE_CXXFLAGS_WARN_ON += -Wno-missing-braces #-Wno-unused-parameter
#QMAKE_CFLAGS += -flto

DISTFILES += \
    TODO
