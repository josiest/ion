#
# Define the Library
#

add_library(ion-containers INTERFACE)
add_library(ion::containers ALIAS ion-containers)

target_sources(ion-containers PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/containers
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/containers/lookup_table.hpp)

set_target_properties(ion-containers PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.2.0
        SOVERSION 0)

#
# Compile and Install
#

install_ion_module(containers)
