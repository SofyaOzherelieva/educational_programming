cmake_minimum_required(VERSION 3.9)
project(quadratic_solver)

set(CMAKE_CXX_STANDARD 11)

add_executable(quadratic_solver main.cpp test.cpp test.h quadratic_solver.cpp quadratic_solver.h)