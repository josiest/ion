#
# Define
#

add_library(ion-engine STATIC)
set_target_properties(ion-engine PROPERTIES VERSION 0.1.0 SOVERSION 0)
add_library(ion::engine ALIAS ion-engine)

target_sources(ion-engine
    PRIVATE
        sdl_resources.cpp
        sdl_events.cpp

    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/engine
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/engine/sdl_resources.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/engine/sdl_events.hpp)

#
# Compile and Install
#

# link the required dependencies into a static library
find_package(EnTT REQUIRED)
target_link_libraries(ion-engine PUBLIC SDL3::SDL3 EnTT::EnTT)
install_ion_module(engine)
