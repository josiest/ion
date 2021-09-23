cmake_minimum_required(VERSION 3.18)
project(simple-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(ion_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../build)

find_package(SDL2 REQUIRED)
find_package(ion REQUIRED)

add_executable(simple simple.cpp)

include_directories(ion::ion SDL2::SDL2)
target_link_libraries(simple ion::ion SDL2::SDL2)
