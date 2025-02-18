#!/bin/bash

mkdir -p build
rm -rf build/*
cd build

cmake -DCMAKE_TOOLCHAIN_FILE="~/vcpkg/scripts/buildsystems/vcpkg.cmake" -GNinja ..

cd ..

cmake --build build