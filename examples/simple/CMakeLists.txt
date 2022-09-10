cmake_minimum_required(VERSION 3.10)
project(simple-example)

add_executable(simple simple.cpp)

set_target_properties(simple PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED)
target_link_libraries(simple PRIVATE ion::ion)
