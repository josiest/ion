#
# Define the library
#

add_library(ion-input STATIC axis.cpp)
add_library(ion::input ALIAS ion-input)
target_sources(ion-input PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/input
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/input/axis.hpp)

set_target_properties(ion-input PROPERTIES VERSION 0.1.0 SOVERSION 0)

# specify the include directories for ion-input
target_include_directories(ion-input PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies into a static library
target_link_libraries(ion-input PUBLIC SDL3::SDL3)

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-input
        EXPORT ion-input-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/input
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/input)

# install the header files for the input component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/input.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

# create and install the target files
install(EXPORT ion-input-targets
        FILE ion-input-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/input)

# generate and install version and config files
get_target_property(ion_input_VERSION ion-input VERSION)
set(ion_input_version_filepath "${CMAKE_CURRENT_BINARY_DIR}/ion-input-config-version.cmake")

write_basic_package_version_file(${ion_input_version_filepath}
                                 VERSION ${ion_input_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-input-config.cmake ${ion_input_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/input)
