cmake_minimum_required(VERSION 3.8)

project(MySimplePlayer)
set(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_C_COMPILER "/usr/local/Cellar/gcc/9.2.0_1/bin/gcc-9")
SET(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/9.2.0_1/bin/g++-9")


add_subdirectory(Test)
add_subdirectory(MyPlayer)
add_subdirectory(MyThread)
add_subdirectory(MyQueue)