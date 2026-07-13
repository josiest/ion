#
# Define the Library
#

add_library(ion-konbu STATIC)
add_library(ion::konbu ALIAS ion-konbu)

set_target_properties(ion-konbu PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.1.0
        SOVERSION 0)

target_sources(ion-konbu
    PRIVATE
        paths.cpp
        misc_serialization.cpp
        sdl_yaml.cpp
        meta_yaml.cpp
        color_yaml.cpp

    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/konbu
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/konbu/paths.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/konbu/misc_serialization.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/konbu/sdl_yaml.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/konbu/meta_yaml.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/konbu/color_yaml.hpp)

#
# Compile and Install
#

# link the required dependencies into a static library
find_package(yaml-cpp REQUIRED)
find_package(EnTT REQUIRED)
target_link_libraries(ion-konbu
        PUBLIC SDL3::SDL3 yaml-cpp::yaml-cpp EnTT::EnTT ion-containers ion-mylar)

install_ion_module(konbu)
