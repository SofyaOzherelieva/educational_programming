cmake_minimum_required(VERSION 3.9)

project(hometask_3)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(GTest)

add_executable(hometask_3 main.cpp stack.h)