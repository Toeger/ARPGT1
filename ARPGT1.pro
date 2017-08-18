#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T18:24:05
#
#-------------------------------------------------

CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++1z

QT_SELECT = 5 qmake

linux:INCLUDEPATH += /usr/include/lua5.1

LIBS += -lpthread
LIBS += -lIrrlicht
LIBS += -llua5.1

SOURCES += \
    ecs/common_components.cpp \
    ecs/ecs_impl.cpp \
    ecs/entity.cpp \
    ecs/entity_base.cpp \
    ecs/entity_handle.cpp \
    ecs/log.cpp \
    ecs/system.cpp \
    ecs/system_base.cpp \
    ecs/system_iterator.cpp \
    ecs/utility.cpp \
    gameplay/map.cpp \
    gameplay/skills/skill.cpp \
    graphics/camera.cpp \
    graphics/common_graphics_components.cpp \
    graphics/input_handler.cpp \
    graphics/physicals.cpp \
    graphics/terrain.cpp \
    graphics/window.cpp \
    main.cpp \
    network.cpp \
    player.cpp \
    tests/collisions.cpp \
    tests/ecs_test.cpp \
    tests/include_tester.cpp \
    tests/perlin_test.cpp \
    tests/physics_utility.cpp \
    tests/skill_loader_test.cpp \
    tests/tester.cpp \
    utility/converter.cpp

HEADERS += \
    ecs/common_components.h \
    ecs/ecs_impl.h \
    ecs/entity.h \
    ecs/entity_base.h \
    ecs/entity_handle.h \
    ecs/log.h \
    ecs/system.h \
    ecs/system_base.h \
    ecs/system_iterator.h \
    ecs/utility.h \
    external/LuaContext.hpp \
    external/json.hpp \
    gameplay/map.h \
    gameplay/skills/skill.h \
    graphics/camera.h \
    graphics/common_graphics_components.h \
    graphics/input_handler.h \
    graphics/perlinnoise.h \
    graphics/physicals.h \
    graphics/terrain.h \
    graphics/window.h \
    main.h \
    make_function.h \
    network.h \
    physics/aabb.h \
    physics/body.h \
    physics/circle.h \
    physics/collision.h \
    physics/line.h \
    physics/physics_utility.h \
    physics/rect.h \
    physics/sensor.h \
    physics/shapes.h \
    player.h \
    tests/collisions.h \
    tests/ecs_test.h \
    tests/perlin_test.h \
    tests/physics_utility.h \
    tests/skill_loader_test.h \
    tests/tester.h \
    tests/tests_utility.h \
    utility/asserts.h \
    utility/casts.h \
    utility/converter.h

QMAKE_CXXFLAGS += -std=c++1z
QMAKE_CXXFLAGS_DEBUG += -fno-omit-frame-pointer -Wall -ggdb #-Werror -Wfatal-errors
DEFINES += ARTDIR=\'\"$${PWD}\"\'
#QMAKE_CXXFLAGS_DEBUG += -Weverything
QMAKE_CXXFLAGS_DEBUG += -Wno-unused-function -Wno-c++98-compat -Wno-shadow -Wno-string-conversion -Wno-c++98-compat-pedantic -Wno-exit-time-destructors
QMAKE_CXXFLAGS_DEBUG += -Wno-global-constructors -Wno-missing-braces -Wno-newline-eof -Wno-padded -Wno-documentation -Wno-documentation-unknown-command
QMAKE_CXXFLAGS_DEBUG += -Wno-sign-conversion -Wno-float-conversion -Wno-double-promotion -Wno-shorten-64-to-32 -Wno-sign-compare
QMAKE_CXXFLAGS_DEBUG += -Wno-unneeded-internal-declaration -Wno-switch-enum #-Wno-unused-parameter
QMAKE_CXXFLAGS_DEBUG += --system-header-prefix=external/

linux-clang{
    QMAKE_CXXFLAGS_DEBUG += -fsanitize=undefined,address#,safe-stack
    QMAKE_LFLAGS_DEBUG += -fsanitize=undefined,address#,safe-stack
}
QMAKE_CXXFLAGS_PROFILE += -DNDEBUG
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG
gcc{
    clang{
        #clang pretends to be gcc but doesn't support -flto
    }
    else{
        QMAKE_CXXFLAGS_RELEASE += -flto
        QMAKE_LFLAGS_RELEASE += -flto
    }
}

DISTFILES += \
    CMakeLists.txt \
    data/skill_documentation.txt \
    data/skills.json \
    LICENSE \
    README.md \
    TODO
