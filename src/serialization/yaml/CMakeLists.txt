#
# Define the library
#

add_library(ion-yaml-serialization STATIC sdl_primitives.cpp window.cpp)
add_library(ion::yaml-serialization ALIAS ion-yaml-serialization)

# specify the include directories for ion-yaml-serialization
target_include_directories(ion-yaml-serialization PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies into a static library
find_package(yaml-cpp REQUIRED CONFIG)
target_link_libraries(ion-yaml-serialization PUBLIC SDL2::SDL2 yaml-cpp::yaml-cpp)

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-yaml-serialization
        EXPORT ion-yaml-serialization-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/serialization/yaml
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/serialization/yaml)

# install the header files for the yaml-serialization component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/serialization/yaml.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/serialization)

install(FILES ${CMAKE_SOURCE_DIR}/include/ion/serialization/yaml/sdl_primitives.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/serialization/yaml/window.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/serialization/yaml)

# create and install the target files
install(EXPORT ion-yaml-serialization-targets
        FILE ion-yaml-serialization-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/yaml-serialization)

# generate and install version and config files
get_target_property(ion_yaml_serialization_VERSION ion-yaml-serialization VERSION)
set(ion_yaml_serialization_version_filepath
    "${CMAKE_CURRENT_BINARY_DIR}/ion-yaml-serialization-config-version.cmake")

write_basic_package_version_file(${ion_yaml_serialization_version_filepath}
        VERSION ${ion_yaml_serialization_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-yaml-serialization-config.cmake ${ion_yaml_serialization_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/yaml-serialization)
