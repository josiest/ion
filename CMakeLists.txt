# --- project setup ---
cmake_minimum_required(VERSION 3.23)
project(ion VERSION 0.8.1)

#
# Export and install the libary
#

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
find_package(EnTT REQUIRED)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# static components
add_subdirectory(src)
add_subdirectory(src/engine)
add_subdirectory(src/serialization)
add_subdirectory(src/editor)

add_subdirectory(src/window)
add_subdirectory(src/input)
add_subdirectory(src/graphics)
add_subdirectory(src/serialization/yaml)

# interface components
add_subdirectory(include/ion/mylar)

# create and install config files
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/ion-config-version.cmake"
                                 VERSION ${CMAKE_PROJECT_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

configure_package_config_file(ion-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/ion-config.cmake"
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ion-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/ion-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)