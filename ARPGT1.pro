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
    Graphics/physicals.cpp \
    Tests/tester.cpp \
    Tests/collisions.cpp \
    Tests/physics_utility.cpp \
    Tests/ecs_test.cpp \
    Tests/include_tester.cpp \
    ECS/entity_base.cpp \
    network.cpp \
    Tests/perlin_test.cpp \
    GamePlay/map.cpp \
    Graphics/window.cpp \
    Utility/converter.cpp \
    Graphics/camera.cpp \
    Graphics/terrain.cpp \
    Graphics/input_handler.cpp \
    Graphics/common_graphics_components.cpp

LIBS += -lpthread -lIrrlicht

HEADERS += \
    ECS/entity.h \
    ECS/system.h \
    ECS/systemiterator.h \
    ECS/ecs_impl.h \
    player.h \
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
    network.h \
    ECS/utility.h \
    Utility/vmap.h \
    Graphics/perlinnoise.h \
    Tests/perlin_test.h \
    GamePlay/map.h \
    Physics/aarect.h \
    Graphics/window.h \
    Utility/converter.h \
    Graphics/camera.h \
    Graphics/terrain.h \
    Graphics/input_handler.h \
    Graphics/common_graphics_components.h

QMAKE_CXXFLAGS += -std=c++1z
QMAKE_CXXFLAGS_DEBUG += -fno-omit-frame-pointer -Wall -Werror -ggdb -fsanitize=undefined,address#,safe-stack
#QMAKE_CXXFLAGS_DEBUG += -Weverything -Wno-c++98-compat -Wno-shadow -Wno-string-conversion -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-global-constructors
#QMAKE_CXXFLAGS_DEBUG_WARN_ON += -Wno-missing-braces #-Wno-unused-parameter
QMAKE_LFLAGS_DEBUG += -fsanitize=undefined,address#,safe-stack
QMAKE_CXXFLAGS_RELEASE += -O3
#QMAKE_CFLAGS += -flto

DISTFILES += \
    TODO

copyart.target = always
copyart.commands = mkdir -p Art; cp $${PWD}/Art/*.png Art;
QMAKE_EXTRA_TARGETS += copyart
PRE_TARGETDEPS += always


