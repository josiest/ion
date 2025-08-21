cmake_minimum_required(VERSION 3.10)
project(pipes-example)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(pipes
    src/pipes.cpp
    src/components.cpp

    src/entities/tile.cpp

    src/systems/input.cpp
    src/systems/tile.cpp)

target_sources(pipes PUBLIC FILE_SET HEADERS FILES
    src/pipes.hpp
    src/entities.hpp
    src/components.hpp
    src/systems.hpp

    src/entities/tile.hpp

    src/systems/grid.hpp
    src/systems/input.hpp
    src/systems/point.hpp
    src/systems/render.hpp
    src/systems/tile.hpp)

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

