cmake_minimum_required(VERSION 3.18)
project(muncher-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(ion_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../build)

find_package(SDL2 REQUIRED)
find_package(ion REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(muncher ${SOURCES})
target_link_libraries(muncher ion::ion SDL2::SDL2)

set(entt_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/entt/src)
include_directories(muncher include ${entt_DIR} ion::ion SDL2::SDL2)
