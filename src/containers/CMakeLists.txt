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

# specify include directories for ion-containers
target_include_directories(ion-containers INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Install the Library
#

# install the compiled objects for ion-containers
install(TARGETS ion-containers
        EXPORT ion-containers-targets
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/containers)

# create and install the target files
install(EXPORT ion-containers-targets
        FILE ion-containers-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/containers)

install(FILES ${CMAKE_SOURCE_DIR}/include/ion/containers.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

#
# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(ion_containers_VERSION ion-containers VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/ion-containers-config-version.cmake"
        VERSION ${ion_containers_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-containers-config.cmake
              "${CMAKE_CURRENT_BINARY_DIR}/ion-containers-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/containers)
