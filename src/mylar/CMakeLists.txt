#
# Define the library
#

add_library(ion-mylar INTERFACE)
add_library(ion::mylar ALIAS ion-mylar)

target_sources(ion-mylar
PUBLIC
    FILE_SET ion_mylar_headers
    TYPE HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/mylar
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/mylar/reflect.hpp)

set_target_properties(ion-mylar PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.1.0
        SOVERSION 0)

find_package(EnTT REQUIRED CONFIG)

if(DEFINED USE_SDL2)
    find_package(SDL2 REQUIRED CONFIG)
    target_link_libraries(ion-mylar INTERFACE SDL2::SDL2 EnTT::EnTT)
else()
    find_package(SDL3 REQUIRED CONFIG)
    target_link_libraries(ion-mylar INTERFACE SDL3::SDL3 EnTT::EnTT)
endif()

# specify include directories for ion-mylar
target_include_directories(ion-mylar INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-mylar
        EXPORT ion-mylar-targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/mylar
        FILE_SET ion_mylar_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/mylar)

# create and install the target files
install(EXPORT ion-mylar-targets
        FILE ion-mylar-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/mylar)

# install the header files for the mylar component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/mylar.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

# generate and install version and config files
include(CMakePackageConfigHelpers)
get_target_property(ion_mylar_VERSION ion-mylar VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/ion-mylar-config-version.cmake"
        VERSION ${ion_mylar_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-mylar-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/ion-mylar-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/mylar)
