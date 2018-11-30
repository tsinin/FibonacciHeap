cmake_minimum_required(VERSION 3.12)
project(FibHeap)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(google-tests/googletest-master)
include_directories(google-tests/googletest-master/googletest/include)
include_directories(google-tests/googletest-master/googlemock/include)

add_executable(FibHeap main.cpp Tests/FibHeap.h Tests/Vector.h Tests/test.cpp)
target_link_libraries(FibHeap gtest gtest_main)