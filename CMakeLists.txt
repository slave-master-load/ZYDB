cmake_minimum_required(VERSION 3.22)

project(ZYDB)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3")

find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(base)
add_subdirectory(core)
add_subdirectory(example)
