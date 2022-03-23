#! /bin/sh

rm ./build -rf
mkdir build
cd build
cmake ..
cmake --build .
