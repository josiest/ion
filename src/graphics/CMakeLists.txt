#
# Define the library
#

add_library(ion-graphics STATIC font.cpp surface.cpp texture.cpp)
add_library(ion::graphics ALIAS ion-graphics)
set_target_properties(ion-graphics PROPERTIES VERSION 0.1.0 SOVERSION 0)

# specify the include directories for ion-graphics
target_include_directories(ion-graphics PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                               "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies into a static library
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
target_link_libraries(ion-graphics PUBLIC SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-graphics
        EXPORT ion-graphics-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/graphics
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/graphics)

# install the header files for the graphics component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/graphics.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

install(FILES ${CMAKE_SOURCE_DIR}/include/ion/graphics/font.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/graphics/surface.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/graphics/texture.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/graphics)

# create and install the target files
install(EXPORT ion-graphics-targets
        FILE ion-graphics-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/graphics)

get_target_property(ion_graphics_VERSION ion-graphics VERSION)
set(ion_graphics_version_filepath "${CMAKE_CURRENT_BINARY_DIR}/ion-graphics-config-version.cmake")
write_basic_package_version_file(${ion_graphics_version_filepath}
                                 VERSION ${ion_graphics_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-graphics-config.cmake ${ion_graphics_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/graphics)