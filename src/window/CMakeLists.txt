#
# Define the library
#

add_library(ion-window STATIC hardware_renderer.cpp software_renderer.cpp sdl_context.cpp)
add_library(ion::window ALIAS ion-window)

# specify the include directories for ion-window
target_include_directories(ion-window PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                             "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies into a static library
target_link_libraries(ion-window PUBLIC SDL2::SDL2)

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-window
        EXPORT ion-window-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/window
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/window)

# install the header files for the window component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/window.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

install(FILES ${CMAKE_SOURCE_DIR}/include/ion/window/hardware_renderer.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/window/software_renderer.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/window/sdl_context.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/window/resource.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/window)

# create and install the target files
install(EXPORT ion-window-targets
        FILE ion-window-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/window)

# generate and install version and config files
get_target_property(ion_window_VERSION ion-window VERSION)
set(ion_window_version_filepath "${CMAKE_CURRENT_BINARY_DIR}/ion-window-config-version.cmake")

write_basic_package_version_file(${ion_window_version_filepath}
        VERSION ${ion_window_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-window-config.cmake ${ion_window_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/window)
