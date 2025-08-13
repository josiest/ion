cmake_minimum_required(VERSION 3.10)
project(fibonacci-example)

add_executable(fibonacci fibonacci.cpp)
file(COPY settings.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(fibonacci PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
if (TARGET SDL2::SDL2main)
    target_link_libraries(fibonacci PRIVATE SDL2::SDL2main)
endif ()

find_package(ion REQUIRED CONFIG REQUIRED COMPONENTS ion window graphics input yaml-serialization)
target_link_libraries(fibonacci PRIVATE ion::ion ion::window ion::graphics ion::input ion::yaml-serialization)

find_package(yaml-cpp REQUIRED CONFIG)
target_link_libraries(fibonacci PRIVATE yaml-cpp::yaml-cpp)