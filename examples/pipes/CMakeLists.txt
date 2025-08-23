cmake_minimum_required(VERSION 3.10)
project(pipes-example)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(pipes
    src/pipes.cpp

    src/private/Board.cpp
    src/private/Deck.cpp
    src/private/Hand.cpp
    src/private/Tile/Tile.cpp
    src/private/Tile/TileInfo.cpp
    src/private/Tile/TileMap.cpp)

target_sources(pipes PUBLIC FILE_SET HEADERS FILES
    src/pipes.hpp
    src/components.hpp

    src/public/Pipes/Grid.hpp
    src/public/Pipes/Board.hpp
    src/public/Pipes/PointSet.hpp
    src/public/Pipes/TileSettings.hpp
    src/public/Pipes/Deck.hpp
    src/public/Pipes/Hand.hpp
    src/public/Pipes/Tile/Tile.hpp
    src/public/Pipes/Tile/TileInfo.hpp
    src/public/Pipes/Tile/TileMap.hpp)

set_target_properties(pipes PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

target_include_directories(pipes PRIVATE src src/public)

find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
if (TARGET SDL2::SDL2main)
    target_link_libraries(pipes PRIVATE SDL2::SDL2main)
endif ()

find_package(ion REQUIRED CONFIG REQUIRED COMPONENTS window graphics input)
target_link_libraries(pipes PRIVATE ion::window ion::graphics ion::input)

find_package(EnTT REQUIRED CONFIG)
target_link_libraries(pipes INTERFACE EnTT::EnTT)

