#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T18:24:05
#
#-------------------------------------------------

CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/lua5.1

LIBS += -lpthread
LIBS += -lIrrlicht
LIBS += -llua5.1
LIBS += -ltolua++5.1

SOURCES += \
    ECS/entity_base.cpp \
    GamePlay/map.cpp \
    GamePlay/skill.cpp \
    Graphics/camera.cpp \
    Graphics/common_graphics_components.cpp \
    Graphics/input_handler.cpp \
    Graphics/physicals.cpp \
    Graphics/terrain.cpp \
    Graphics/window.cpp \
    main.cpp \
    network.cpp \
    player.cpp \
    Tests/collisions.cpp \
    Tests/ecs_test.cpp \
    Tests/include_tester.cpp \
    Tests/perlin_test.cpp \
    Tests/physics_utility.cpp \
    Tests/skill_loader_test.cpp \
    Tests/tester.cpp \
    Utility/converter.cpp \
    GamePlay/Skills/luaskillbindings.cpp

HEADERS += \
    ECS/common_components.h \
    ECS/ecs_impl.h \
    ECS/entity_base.h \
    ECS/entity.h \
    ECS/entity_handle.h \
    ECS/system.h \
    ECS/systemiterator.h \
    ECS/utility.h \
    External/json.hpp \
    GamePlay/map.h \
    GamePlay/skill.h \
    Graphics/camera.h \
    Graphics/common_graphics_components.h \
    Graphics/input_handler.h \
    Graphics/perlinnoise.h \
    Graphics/physicals.h \
    Graphics/terrain.h \
    Graphics/window.h \
    make_function.h \
    network.h \
    Physics/aabb.h \
    Physics/aarect.h \
    Physics/body.h \
    Physics/circle.h \
    Physics/collision.h \
    Physics/line.h \
    Physics/physics_utility.h \
    Physics/sensor.h \
    Physics/shapes.h \
    player.h \
    Tests/collisions.h \
    Tests/ecs_test.h \
    Tests/perlin_test.h \
    Tests/physics_utility.h \
    Tests/skill_loader_test.h \
    Tests/tester.h \
    Tests/tests_utility.h \
    Utility/asserts.h \
    Utility/casts.h \
    Utility/converter.h \
    Utility/vmap.h \
    GamePlay/Skills/luaskills.h \
    GamePlay/Skills/luaskillbindings.pkg

QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_DEBUG += -fno-omit-frame-pointer -Wall -Werror -ggdb
linux-clang:QMAKE_CXXFLAGS_DEBUG += -fsanitize=undefined,address#,safe-stack
#QMAKE_CXXFLAGS_DEBUG += -Weverything -Wno-c++98-compat -Wno-shadow -Wno-string-conversion -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-global-constructors
#QMAKE_CXXFLAGS_DEBUG_WARN_ON += -Wno-missing-braces #-Wno-unused-parameter
linux-clang:QMAKE_LFLAGS_DEBUG += -fsanitize=undefined,address#,safe-stack
QMAKE_CXXFLAGS_RELEASE += -O3
#QMAKE_CFLAGS += -flto

DISTFILES += \
    TODO \
    LICENSE \
    README.md \
    Data/skills.json \
    Data/skill_documentation.txt

copyart.target = always
copyart.commands = mkdir -p Art; cp $${PWD}/Art/* Art || true;
QMAKE_EXTRA_TARGETS += copyart
PRE_TARGETDEPS += always
