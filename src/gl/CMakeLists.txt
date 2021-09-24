# --- compile library ---

# declare sources
file(GLOB_RECURSE gl_SOURCES *.${ion_SRCEXT})

# compile library
add_library(gl STATIC ${gl_SOURCES})
add_library(ion::gl ALIAS gl)

# specify header file directories for building/installing
target_include_directories(gl PUBLIC
    $<BUILD_INTERFACE:${ion_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# find SDL2 and OpenGL
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(SDL2::SDL2 OpenGL::OpenGL)
target_link_libraries(gl SDL2::SDL2 OpenGL::OpenGL)

# --- export library ---

# specify the library targets
install(TARGETS gl
        EXPORT ionglTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install the export details
install(EXPORT ionglTargets
        FILE ionglTargets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)
