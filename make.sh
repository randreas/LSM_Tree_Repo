#! /bin/sh

rm ./build -rf
mkdir build
cd build
cmake ..
cmake --build .

# cd build/src/LSMTree
# ./main 3 ../../../test_3_3_3.wl ../../../test_3_3_3.wl