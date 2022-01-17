cmake_minimum_required(VERSION 3.18)
project(simple-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(ion REQUIRED)

add_executable(simple simple.cpp)

include_directories(ion::ion SDL2::Core)
target_link_libraries(simple ion::ion SDL2::Core SDL2::TTF)
