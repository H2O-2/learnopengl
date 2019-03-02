#!/bin/bash

if [[ "$OSTYPE" = "msys" ]]; then
    cd build/bin/Debug
    ./learnOpenGL.exe
else
    cd build/bin
    ./learnOpenGL
fi
