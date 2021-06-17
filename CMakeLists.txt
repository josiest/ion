# --- project setup ---
cmake_minimum_required(VERSION 3.10)
project(ion)

# versioning
set(ion_MAJOR_VERSION 0)
set(ion_MINOR_VERSION 1)
set(ion_PATCH_VERSION 0)
set(ion_VERSION
    ${ion_MAJOR_VERSION}.${ion_MINOR_VERSION}.${ion_PATCH_VERSION})

# make cache variables for install destinations
include(GNUInstallDirs)

# compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- compile library ---

# declare sources
set(ion_SRCEXT cpp)
file(GLOB SOURCES src/*.${ion_SRCEXT})

# compile library
add_library(ion STATIC ${SOURCES})

# specify header file directories for building/installing
target_include_directories(ion PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# link sdl
find_package(SDL2 REQUIRED)
target_link_libraries(ion SDL2)

# --- export library ---

# specify the library targets
install(TARGETS ion
    EXPORT ionTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# declare headers
set(ion_HEXT hpp)
file(GLOB HEADERS include/*.${ion_HEXT})

# install header files into installation include dir
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install the export details
install(EXPORT ionTargets
    FILE ionTargets.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/ion/cmake)

message("cmake current binary dir: ${CMAKE_CURRENT_BINARY_DIR}")

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

# allow exporting from build tree
export(EXPORT ionTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/ionTargets.cmake)

