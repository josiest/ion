cmake_minimum_required(VERSION 3.10)
project(pipes-example)

file(GLOB_RECURSE sources src/*.cpp)
add_executable(pipes ${sources})
set_target_properties(pipes PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

target_include_directories(pipes PRIVATE src)

find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
if (TARGET SDL2::SDL2main)
    target_link_libraries(pipes PRIVATE SDL2::SDL2main)
endif ()

find_package(ion REQUIRED CONFIG REQUIRED COMPONENTS window graphics input)
target_link_libraries(pipes PRIVATE ion::window ion::graphics ion::input)

find_package(EnTT REQUIRED CONFIG)
target_link_libraries(pipes INTERFACE EnTT::EnTT)

