cmake_minimum_required(VERSION 3.18)
project(muncher-example)

add_executable(muncher)
set_target_properties(muncher PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)

target_sources(muncher
PRIVATE
    src/entities/munchable.cpp
    src/entities/player.cpp

    src/systems/mechanics.cpp
    src/systems/physics.cpp

    src/muncher.cpp

PUBLIC FILE_SET HEADERS
BASE_DIRS include FILES
    include/entities/munchable.hpp
    include/entities/player.hpp

    include/systems/mechanics.hpp
    include/systems/physics.hpp
    include/systems/render.hpp

    include/components.hpp
    include/muncher.hpp)

target_include_directories(muncher PRIVATE include)

find_package(ion REQUIRED CONFIG REQUIRED COMPONENTS editor input)
target_link_libraries(muncher PRIVATE ion::editor ion::input)

find_package(EnTT REQUIRED CONFIG)
target_link_libraries(muncher INTERFACE EnTT::EnTT)

