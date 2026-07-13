cmake_minimum_required(VERSION 3.10)
project(pipes-example)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(pipes
    src/private/Pipes.cpp
    src/private/Board.cpp
    src/private/Deck.cpp
    src/private/Hand.cpp
    src/private/Tile/Tile.cpp
    src/private/Tile/TileInfo.cpp
    src/private/Tile/TileMap.cpp)

target_sources(pipes PUBLIC FILE_SET HEADERS FILES
    src/public/Pipes/Pipes.hpp
    src/public/Pipes/Grid.hpp
    src/public/Pipes/Board.hpp
    src/public/Pipes/PointSet.hpp
    src/public/Pipes/Deck.hpp
    src/public/Pipes/Hand.hpp
    src/public/ion/transform.hpp

    src/public/Pipes/Tile.hpp
    src/public/Pipes/Tile/Tile.hpp
    src/public/Pipes/Tile/TileSettings.hpp
    src/public/Pipes/Tile/TileInfo.hpp
    src/public/Pipes/Tile/TileMap.hpp)

set_target_properties(pipes PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

target_include_directories(pipes PRIVATE src/public)

find_package(ion REQUIRED CONFIG REQUIRED COMPONENTS editor mylar)
target_link_libraries(pipes PRIVATE ion::editor ion::mylar)

find_package(glm REQUIRED CONFIG)
target_link_libraries(pipes PRIVATE glm::glm)
