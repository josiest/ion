#
# Define the Library
#

add_library(ion-containers INTERFACE)
add_library(ion::containers ALIAS ion-containers)

set_target_properties(ion-containers
    PROPERTIES
        VERSION 0.2.0
        SOVERSION 0
        EXPORT_NAME containers)

target_sources(ion-containers
    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/containers
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/containers/lookup_table.hpp)

#
# Compile and Install
#

install_ion_module(containers)
