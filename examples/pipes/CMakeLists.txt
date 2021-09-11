cmake_minimum_required(VERSION 3.10)
project(pipes-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(SDL2 REQUIRED)

set(ion_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../build)
find_package(ion REQUIRED)

file(GLOB SOURCES src/*.cpp)
add_executable(pipes ${SOURCES})

target_link_libraries(pipes PUBLIC ion SDL2)
target_include_directories(pipes PUBLIC include)
