cmake_minimum_required(VERSION 3.10)
project(simple-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED)
add_executable(simple simple.cpp)

include_directories(ion::ion)
target_link_libraries(simple ion::ion)
