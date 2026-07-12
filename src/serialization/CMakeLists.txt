#
# Define the Library
#

add_library(ion-serialization STATIC)
add_library(ion::serialization ALIAS ion-serialization)

target_sources(ion-serialization
    PRIVATE
        paths.cpp
        misc_serialization.cpp

    PUBLIC
        FILE_SET ion_serialization_headers
        TYPE HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/serialization
        FILES
            ${CMAKE_SOURCE_DIR}/include/ion/serialization/paths.hpp
            ${CMAKE_SOURCE_DIR}/include/ion/serialization/misc_serialization.hpp)

# specify the include directories for ion-serialization
target_include_directories(ion-serialization PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies into a static library
target_link_libraries(ion-serialization PUBLIC SDL3::SDL3)

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-serialization
        EXPORT ion-serialization-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/serialization
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/serialization
        FILE_SET ion_serialization_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/serialization)

# install the header files for the serialization component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/serialization.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

# create and install the target files
install(EXPORT ion-serialization-targets
        FILE ion-serialization-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/serialization)

# generate and install version and config files
get_target_property(ion_serialization_VERSION ion-serialization VERSION)
set(ion_serialization_version_filepath "${CMAKE_CURRENT_BINARY_DIR}/ion-serialization-config-version.cmake")

write_basic_package_version_file(${ion_serialization_version_filepath}
        VERSION ${ion_serialization_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-serialization-config.cmake ${ion_serialization_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/serialization)
