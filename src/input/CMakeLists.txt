#
# Define the library
#

add_library(ion-input STATIC)
add_library(ion::input ALIAS ion-input)
set_target_properties(ion-input PROPERTIES VERSION 0.2.0 SOVERSION 0)

target_sources(ion-input
    PRIVATE
        axis.cpp

    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/input
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/input/axis.hpp)

#
# Compile and Install
#

# link the required dependencies into a static library
target_link_libraries(ion-input PUBLIC SDL3::SDL3)
install_ion_module(input)
