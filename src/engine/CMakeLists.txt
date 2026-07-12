#
# Define the Library
#

add_library(ion-engine STATIC)
add_library(ion::engine ALIAS ion-engine)

target_sources(ion-engine
    PRIVATE
        sdl_resources.cpp
        sdl_events.cpp

    PUBLIC
        FILE_SET ion_engine_headers
        TYPE HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/engine
        FILES
            ${CMAKE_SOURCE_DIR}/include/ion/engine/sdl_resources.hpp
            ${CMAKE_SOURCE_DIR}/include/ion/engine/sdl_events.hpp)

# specify the include directories for ion-engine
target_include_directories(ion-engine PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies into a static library
find_package(EnTT REQUIRED)
target_link_libraries(ion-engine PUBLIC SDL3::SDL3 EnTT::EnTT)

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-engine
        EXPORT ion-engine-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/engine
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/engine
        FILE_SET ion_engine_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/engine)

# install the header files for the engine component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/engine.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

# create and install the target files
install(EXPORT ion-engine-targets
        FILE ion-engine-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/engine)

# generate and install version and config files
get_target_property(ion_engine_VERSION ion-engine VERSION)
set(ion_engine_version_filepath "${CMAKE_CURRENT_BINARY_DIR}/ion-engine-config-version.cmake")

write_basic_package_version_file(${ion_engine_version_filepath}
        VERSION ${ion_engine_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-engine-config.cmake ${ion_engine_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/engine)
