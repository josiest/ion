#
# Define the library
#

add_library(ion-mylar INTERFACE)
add_library(ion::mylar ALIAS ion-mylar)

find_package(SDL2 REQUIRED CONFIG)
find_package(EnTT REQUIRED CONFIG)

target_link_libraries(ion-mylar INTERFACE SDL2::SDL2 EnTT::EnTT)

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-mylar
        EXPORT ion-mylar-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/mylar
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/mylar)

# install the header files for the mylar component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/mylar.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

install(FILES reflect.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/mylar)

# create and install the target files
install(EXPORT ion-mylar-targets
        FILE ion-mylar-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/mylar)

# generate and install version and config files
get_target_property(ion_yaml_serialization_VERSION ion-mylar VERSION)
set(ion_yaml_serialization_version_filepath
        "${CMAKE_CURRENT_BINARY_DIR}/ion-mylar-config-version.cmake")

write_basic_package_version_file(${ion_yaml_serialization_version_filepath}
        VERSION ${ion_yaml_serialization_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-mylar-config.cmake ${ion_yaml_serialization_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/mylar)
