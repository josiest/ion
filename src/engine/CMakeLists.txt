#
# Define
#

add_library(ion-engine STATIC)
set_target_properties(ion-engine
    PROPERTIES
        VERSION 0.1.0
        SOVERSION 0
        CXX_STANDARD 26
        CXX_STANDARD_REQUIRED TRUE
        EXPORT_NAME engine)

target_sources(ion-engine
    PRIVATE
        sdl_resources.cpp
        sdl_events.cpp
        engine_component.cpp
        engine.cpp

    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/engine
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/engine/sdl_resources.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/engine/engine_component.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/engine/sdl_events.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/engine/engine.hpp)

target_compile_options(ion-engine PUBLIC "-freflection")

#
# Compile and Install
#

# link the required dependencies into a static library
find_package(EnTT REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(ion-engine PUBLIC
        SDL3::SDL3
        EnTT::EnTT
        GLEW::GLEW
        yaml-cpp::yaml-cpp
        ion-konbu
        ion-photon)

install_ion_module(engine)
