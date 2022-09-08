cmake_minimum_required(VERSION 3.24)
project(pipes-example)

file(GLOB_RECURSE sources src/*.cpp)
add_executable(pipes ${sources})
include_directories(pipes src)

set_target_properties(pipes PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED)
target_link_libraries(pipes PRIVATE ion::ion)

find_package(EnTT REQUIRED)
target_link_libraries(pipes INTERFACE EnTT::EnTT)
