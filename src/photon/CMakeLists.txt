add_library(ion-photon STATIC)
set_target_properties(ion-photon PROPERTIES VERSION 0.1.0 SOVERSION 0)
add_library(ion::photon ALIAS ion-photon)

target_sources(ion-photon
    PRIVATE
        mesh.cpp
        shader.cpp
        shader_manager.cpp

    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/photon
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/photon/mesh.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/photon/shader.hpp
        ${CMAKE_SOURCE_DIR}/include/ion/photon/shader_manager.hpp)

#
# Compile and Install
#

# link the required dependencies into a static library
find_package(GLEW REQUIRED)
target_link_libraries(ion-photon PUBLIC SDL3::SDL3 GLEW::GLEW)
install_ion_module(photon)
