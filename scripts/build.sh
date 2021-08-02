#!/usr/bin/env sh

mkdir -p build/ && cd build/

CC=gcc; CXX=g++; cmake -G"Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

cmake --build .
