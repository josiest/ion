# --- project setup ---
cmake_minimum_required(VERSION 3.18)
project(ion VERSION 0.8.0)

# add addnl modules
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

add_subdirectory(src)

#
# Export and install the libary
#

include(GNUInstallDirs)

# specify header file directories for building/installing
target_include_directories(ion PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# specify the library targets
install(TARGETS ion
        EXPORT ion-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install the export details
install(EXPORT ion-targets
        FILE ion-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)

# install the include files
install(DIRECTORY include/ion
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

add_library(ion::ion STATIC IMPORTED)
set_target_properties(ion::ion PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include")

#
# Create and install the package
#

include(CMakePackageConfigHelpers)

# generate the package configuration file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/ion-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ion-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)

# generate the version file for the config file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ion-config-version.cmake
    COMPATIBILITY AnyNewerVersion)

# install the generated files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ion-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ion-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)

# install cmake sdl2 modules
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2/
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/Modules
        FILES_MATCHING PATTERN *.cmake)

# allow exporting from build tree
export(EXPORT ion-targets
       FILE ${CMAKE_CURRENT_BINARY_DIR}/ion-targets.cmake
       NAMESPACE ion::)
