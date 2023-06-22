cmake_minimum_required(VERSION 3.14)
project(penum_examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(example example.cpp)

target_include_directories(example PRIVATE "../include" )
