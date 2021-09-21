cmake_minimum_required(VERSION 3.18)
project(opengl-example)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(ion_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../build)
find_package(ion REQUIRED)

file(GLOB_RECURSE sources src/*.cpp)
add_executable(basic ${sources})

target_include_directories(basic PUBLIC
    ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

target_link_libraries(basic PUBLIC ion SDL2 OpenGL::OpenGL GLEW::GLEW)
