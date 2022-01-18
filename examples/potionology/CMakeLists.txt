cmake_minimum_required(VERSION 3.10)
project(potionology)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED)

file(GLOB_RECURSE sources src/*cpp)
add_executable(potionology ${sources})

include_directories(src ion::ion)
target_link_libraries(potionology ion::ion)
