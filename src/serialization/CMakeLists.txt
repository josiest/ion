#
# Define the Library
#

add_library(ion-serialization STATIC)
add_library(ion::serialization ALIAS ion-serialization)

set_target_properties(ion-serialization PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.1.0
        SOVERSION 0)

target_sources(ion-serialization
    PRIVATE
        paths.cpp
        misc_serialization.cpp
        sdl_yaml.cpp
        meta_yaml.cpp
        color_yaml.cpp

    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/serialization
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/serialization/paths.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/serialization/misc_serialization.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/serialization/sdl_yaml.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/serialization/meta_yaml.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/serialization/color_yaml.hpp)

#
# Compile and Install
#

# link the required dependencies into a static library
find_package(yaml-cpp REQUIRED)
find_package(EnTT REQUIRED)
target_link_libraries(ion-serialization
        PUBLIC SDL3::SDL3 yaml-cpp::yaml-cpp EnTT::EnTT ion-containers ion-mylar)

install_ion_module(serialization)
