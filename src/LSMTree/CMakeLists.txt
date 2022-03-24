add_library(LSMTree LSMTree.cpp)
add_library(Tuple Tuple.cpp)
add_executable(main main.cpp)
target_link_libraries(main LSMTree Tuple)
