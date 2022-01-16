# --- project setup ---
cmake_minimum_required(VERSION 3.18)
project(ion)

# versioning
set(ion_MAJOR_VERSION 0)
set(ion_MINOR_VERSION 5)
set(ion_PATCH_VERSION 0)
set(ion_VERSION
    ${ion_MAJOR_VERSION}.${ion_MINOR_VERSION}.${ion_PATCH_VERSION})

# make cache variables for install destinations
include(GNUInstallDirs)

# compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify file interface
set(ion_SRCEXT cpp)
set(ion_HEXT hpp)
set(ion_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(src/gl)

# --- create package for library ---

# include helper functions for creating config files
include(CMakePackageConfigHelpers)

# set versions
set_property(TARGET ion PROPERTY VERSION ${ion_VERSION})
set_property(TARGET ion PROPERTY
    INTERFACE_ion_MAJOR_VERSION ${ion_MAJOR_VERSION})

# must be compatibale with dependencies of any depender
set_property(TARGET ion APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING ion_MAJOR_VERSION)

# generate the version file for the config file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ionConfigVersion.cmake
    VERSION ${ion_VERSION}
    COMPATIBILITY AnyNewerVersion)

# generate the package configuration file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ionConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)

# install the generated files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ionConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ionConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)

# install the include files
install(DIRECTORY ${ion_INCLUDE_DIR}/ion
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.${ion_HEXT}")

# allow exporting from build tree
export(EXPORT ionTargets
       FILE ${CMAKE_CURRENT_BINARY_DIR}/ionTargets.cmake
       NAMESPACE ion::)

if (UNIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
elseif(WIN32)
    set(CMAKE_INSTALL_PREFIX "C:\\Program Files\\ion")
endif()
