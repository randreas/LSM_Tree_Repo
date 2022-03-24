add_library(LSMTree LSMTree.cpp Utils.h)
add_library(Tuple tuple.cpp)
add_executable(main main.cpp)
target_link_libraries(main LSMTree Tuple)
