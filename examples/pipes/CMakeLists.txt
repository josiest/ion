cmake_minimum_required(VERSION 3.10)
project(pipes-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED)
find_package(SDL2 REQUIRED)

file(GLOB_RECURSE sources src/*.cpp)
add_executable(pipes ${sources})

include_directories(pipes src include ion::ion SDL2::SDL2 entt::entt)
target_link_libraries(pipes ion::ion SDL2::SDL2)
