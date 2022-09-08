cmake_minimum_required(VERSION 3.18)
project(muncher-example)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(muncher ${SOURCES})
target_include_directories(muncher PRIVATE include)

set_target_properties(muncher PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED)
target_link_libraries(muncher PRIVATE ion::ion)

find_package(EnTT REQUIRED)
target_link_libraries(muncher INTERFACE EnTT::EnTT)
