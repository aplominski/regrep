cmake_minimum_required(VERSION 3.31)
project(regrep)

set(CMAKE_CXX_STANDARD 20)

add_executable(regrep main.cpp
        regrep.cpp
        regrep.h)
