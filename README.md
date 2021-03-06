[![Build Status](https://travis-ci.org/Toeger/ARPGT1.svg?branch=master)](https://travis-ci.org/Toeger/ARPGT1)

# ARPGT1
This is the client of ARPGT1.

It uses [ECS](https://github.com/Toeger/ECS) as a submodule, therefore you need to clone using

    git clone https://github.com/Toeger/ARPGT1
    git submodule init
    git submodule update

so that it pulls the required ECS submodule too.

ARPGT1 currently requires [Irrlicht](http://irrlicht.sourceforge.net/). Tested with Irrlicht Engine version 1.8.3.
Also [Lua](http://lua-users.org/wiki/LuaBinaries) 5.1 is required.

You can use `cd ARPGT1 && mkdir build && cd build && cmake .. && make -j8 && ./ARPGT1` to get the program built and running. If you want to build outside the `ARPGT1` directory you need to copy the folders `Art` and `Data`. Alternatively a [qmake](http://doc.qt.io/qt-5/qmake-manual.html) .pro file is available to build.
