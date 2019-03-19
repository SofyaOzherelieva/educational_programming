cmake_minimum_required(VERSION 3.9)
project(hometask_4)

set(CMAKE_CXX_STANDARD 11)

add_executable(hometask_4
        ../Onegin_sort/Book.h ../Onegin_sort/Book.cpp ../Stack/buffer.h ../Stack/stack.h main.cpp cpu.h parser.h commands.h registers.h CCommand.h)