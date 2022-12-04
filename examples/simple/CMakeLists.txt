cmake_minimum_required(VERSION 3.23)
project(ion-simple-example)

add_executable(simple simple.cpp)
set_target_properties(simple PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

find_package(ion 0.11.0 EXACT REQUIRED PATHS ../../build)
target_link_libraries(simple PRIVATE ion::ion)

