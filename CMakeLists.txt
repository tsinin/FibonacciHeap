cmake_minimum_required(VERSION 3.12)
project(FibHeap)

set(CMAKE_CXX_STANDARD 17)

add_executable(FibHeap main.cpp FibHeap.h Vector.h)