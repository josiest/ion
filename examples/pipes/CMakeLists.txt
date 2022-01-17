cmake_minimum_required(VERSION 3.10)
project(pipes-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(ion REQUIRED)
file(GLOB_RECURSE sources src/*.cpp)
add_executable(pipes ${sources})

include_directories(pipes src include ion::ion entt::entt)
target_link_libraries(pipes ion::ion)
