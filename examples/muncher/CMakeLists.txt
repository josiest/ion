cmake_minimum_required(VERSION 3.10)
project(muncher-example)

find_package(SDL2 REQUIRED)

set(ion_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../build)
find_package(ion REQUIRED)

add_executable(muncher src/muncher.cpp)
target_link_libraries(muncher PUBLIC ion SDL2)

set(entt_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/entt)
target_include_directories(muncher PUBLIC entt_DIR)
