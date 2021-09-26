cmake_minimum_required(VERSION 3.18)
project(opengl-example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ion REQUIRED COMPONENTS gl)
find_package(glm REQUIRED)

file(GLOB_RECURSE sources src/*.cpp)
add_executable(bezier ${sources})

include_directories(src
    ion::ion SDL2::SDL2 OpenGL::OpenGL glm::glm)
target_link_libraries(bezier PRIVATE
    ion::ion ion::gl SDL2::SDL2 OpenGL::OpenGL)
