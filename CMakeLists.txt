# --- project setup ---
cmake_minimum_required(VERSION 3.23) # FILE_SETS available since 23
project(ion VERSION 0.12.0)

add_library(ion STATIC
    # opengl
    src/glad.c

    # imgui
    src/imgui/backends/imgui_impl_opengl3.cpp
    src/imgui/backends/imgui_impl_sdl.cpp
    src/imgui/backends/imgui_impl_sdlrenderer.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui.cpp

    # ion
    src/ion/string.cpp
    src/ion/system.cpp
    src/ion/tag.cpp
    src/ion/ui.cpp)

target_sources(ion PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES

    # opengl
    include/glad/glad.h
    include/KHR/khrplatform.h
    
    # imgui
    include/imgui/backends/imgui_impl_opengl3_loader.h
    include/imgui/backends/imgui_impl_opengl3.h
    include/imgui/backends/imgui_impl_sdl.h
    include/imgui/backends/imgui_impl_sdlrenderer.h
    include/imgui/imconfig.h
    include/imgui/imgui_internal.h
    include/imgui/imgui.h
    include/imgui/imstb_rectpack.h
    include/imgui/imstb_textedit.h
    include/imgui/imstb_truetype.h

    # konbu
    include/konbu/konbu.h

    # ion
    include/ion/expected.hpp
    include/ion/histogram.hpp
    include/ion/string.hpp
    include/ion/system.hpp
    include/ion/tag.hpp
    include/ion/timer.hpp
    include/ion/try.hpp
    include/ion/ui.hpp)

set_target_properties(ion PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)
set(OpenGL_GL_PREFERENCE GLVND) 
 
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(EnTT REQUIRED)
find_package(yaml-cpp REQUIRED)

target_link_libraries(ion
    PUBLIC yaml-cpp SDL2::SDL2 OpenGL::GL ${CMAKE_DL_LIBS}
    INTERFACE EnTT::EnTT)

#
# Export and install the libary
#

include(GNUInstallDirs)

# specify header file directories for building/installing
target_include_directories(ion PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/imgui>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/backends>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# install the include files
install(TARGETS ion FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES src/imgui/imgui_demo.cpp
        DESTINATION src)

# specify the library targets
install(TARGETS ion
        EXPORT ion-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILE_SET HEADERS)

# install the export details
install(EXPORT ion-targets
        FILE ion-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)

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
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ion-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ion-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)

# allow exporting from build tree
export(EXPORT ion-targets
       FILE ${CMAKE_CURRENT_BINARY_DIR}/ion-targets.cmake NAMESPACE ion::)

#
# Examples
#

add_executable(simple examples/simple/simple.cpp)
set_target_properties(simple PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

target_link_libraries(simple PRIVATE ion)