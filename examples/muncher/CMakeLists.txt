cmake_minimum_required(VERSION 3.10)
project(muncher-example)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

set(ion_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../build)
find_package(ion REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(muncher ${SOURCES})
target_link_libraries(muncher PUBLIC ion SDL2)

set(entt_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/entt/src)
target_include_directories(muncher PUBLIC ${entt_DIR} include)
