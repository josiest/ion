# --- compile library ---

# declare sources
file(GLOB_RECURSE ion_SOURCES *.${ion_SRCEXT})

# compile library
add_library(ion STATIC ${ion_SOURCES})
add_library(ion::ion ALIAS ion)
set_target_properties(ion PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
target_link_libraries(ion PUBLIC SDL2::Core SDL2::TTF)

find_package(tl-expected REQUIRED)
target_link_libraries(ion INTERFACE tl::expected)

# specify header file directories for building/installing
target_include_directories(ion PUBLIC
    $<BUILD_INTERFACE:${ion_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# --- export library ---

# specify the library targets
install(TARGETS ion
        EXPORT ionTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install the export details
install(EXPORT ionTargets
        FILE ionTargets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)
