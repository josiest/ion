cmake_minimum_required(VERSION 3.23)
project(hello-ion)

add_executable(hello hello.cpp project.yml)

set_target_properties(hello PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED CONFIG REQUIRED COMPONENTS engine)
target_link_libraries(hello PRIVATE ion::engine)

file(COPY project.yml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
