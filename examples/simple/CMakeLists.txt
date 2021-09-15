cmake_minimum_required(VERSION 3.10)
project(simple-example)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

set(ion_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../build)
find_package(ion REQUIRED)

add_executable(simple simple.cpp)
target_link_libraries(simple PUBLIC ion SDL2)
