cmake_minimum_required(VERSION 3.24)
project(Common)

set(CMAKE_CXX_STANDARD 17)

add_executable(Common main.cpp List.h String.cpp String.h Vector.h)
