cmake_minimum_required(VERSION 3.10)
project(fibonacci-example)

add_executable(fibonacci fibonacci.cpp)
set_target_properties(fibonacci PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED CONFIG REQUIRED COMPONENTS editor mylar)
target_link_libraries(fibonacci PRIVATE ion::editor ion::mylar)

file(COPY config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
