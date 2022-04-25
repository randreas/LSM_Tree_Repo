#! /bin/bash


rm output*
rm ./build -rf
mkdir build
cd build
cmake ..
cmake --build .

# cd build/src/LSMTree
# ./main ../../../data.wl
