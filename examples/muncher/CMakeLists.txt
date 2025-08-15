cmake_minimum_required(VERSION 3.18)
project(muncher-example)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(muncher ${SOURCES})
set_target_properties(muncher PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)

target_include_directories(muncher PRIVATE include)

find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
if (TARGET SDL2::SDL2main)
    target_link_libraries(muncher PRIVATE SDL2::SDL2main)
endif ()

find_package(ion REQUIRED CONFIG REQUIRED COMPONENTS ion window graphics input)
target_link_libraries(muncher PRIVATE ion::ion ion::window ion::graphics ion::input)

find_package(EnTT REQUIRED CONFIG)
target_link_libraries(muncher INTERFACE EnTT::EnTT)

