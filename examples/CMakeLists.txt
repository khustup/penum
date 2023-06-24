cmake_minimum_required(VERSION 3.14)
project(penum_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example_basic example_basic.cpp)
target_include_directories(example_basic PRIVATE "../include" )

add_executable(example_template example_template)
target_include_directories(example_template PRIVATE "../include" )

add_executable(example_same_type example_same_type.cpp)
target_include_directories(example_same_type PRIVATE "../include" )

add_executable(example_same_size example_same_size.cpp)
target_include_directories(example_same_size PRIVATE "../include" )

add_executable(example_mixed example_mixed.cpp)
target_include_directories(example_mixed PRIVATE "../include" )
