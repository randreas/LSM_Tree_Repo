#! /bin/sh

rm ./build -rf
mkdir build
cd build
cmake ..
cmake --build .

# cd build/src/LSMTree
# ./main 3 3 ../../../data.wl ../../../empty.wl
# ./main 3 3 ../../../data.wl ../../../workload.wl