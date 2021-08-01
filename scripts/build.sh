#!/usr/bin/env sh

mkdir -p bulid/ && cd build/

CC=gcc; CXX=g++; cmake -G"Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

cmake --build .