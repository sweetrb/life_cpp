#!/bin/bash
# Build script for Conway's Life C++ version on Windows with MSYS2

export PATH="/c/msys64/mingw64/bin:$PATH"
export TEMP="/tmp"
export TMP="/tmp"

g++ -o life.exe life.cpp \
    -I/c/msys64/mingw64/include/ncurses \
    -L/c/msys64/mingw64/lib \
    -lncursesw

if [ $? -eq 0 ]; then
    echo "Build successful: life.exe"
    ls -lh life.exe
else
    echo "Build failed"
    exit 1
fi
