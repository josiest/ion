cmake_minimum_required(VERSION 3.10)
project(potionology)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(ion REQUIRED)

add_executable(potionology potionology.cpp)

include_directories(ion::ion SDL2::Core SDL2:TTF)
target_link_libraries(potionology ion::ion SDL2::Core SDL2::TTF)
