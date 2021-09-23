# --- compile library ---

# declare sources
file(GLOB_RECURSE ion_SOURCES *.${ion_SRCEXT})

# compile library
add_library(ion STATIC ${ion_SOURCES})
add_library(ion::ion ALIAS ion)

# specify header file directories for building/installing
target_include_directories(ion PUBLIC
    $<BUILD_INTERFACE:${ion_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# find SDL2
find_package(SDL2 REQUIRED)

include_directories(SDL2::SDL2)
target_link_libraries(ion SDL2::SDL2)

# --- export library ---

# specify the library targets
install(TARGETS ion
    EXPORT ionTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# declare headers
file(GLOB_RECURSE ion_HEADERS ${ion_INCLUDE_DIR}/*.${ion_HEXT})

# don't include gl component
file(GLOB_RECURSE ion_gl_HEADERS
    ${ion_INCLUDE_DIR}/gl.hpp ${ion_INCLUDE_DIR}/ion/gl/*.${ion_HEXT})
list(REMOVE_ITEM ion_HEADERS ${ion_gl_HEADERS})

# install header files into installation include dir
install(FILES ${ion_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install the export details
install(EXPORT ionTargets
    FILE ionTargets.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/ion/cmake)

