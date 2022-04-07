cmake_minimum_required(VERSION 2.8.2)
project(googletest-download NONE)

include(ExternalProject)
ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    SOURCE_DIR "/home/jingyusu/Desktop/BU_Courses/SP2022/CS561/Projects/Final_proj/LSM_Tree_Repo/cmake-build-debug/external/googletest-src"
    BINARY_DIR "/home/jingyusu/Desktop/BU_Courses/SP2022/CS561/Projects/Final_proj/LSM_Tree_Repo/cmake-build-debug/external/googletest-build"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)

