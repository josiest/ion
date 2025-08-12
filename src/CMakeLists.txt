#
# Define the ion library
#

add_library(ion-core STATIC events.cpp)
add_library(ion::ion ALIAS ion-core)

# specify the include directories for ion
target_include_directories(ion-core PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies into a static library
target_link_libraries(ion-core PUBLIC SDL2::SDL2)

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-core
        EXPORT ion-core-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

# install the header files for ion core
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/ion.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/time.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/events.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/isotope.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

install(FILES ${CMAKE_SOURCE_DIR}/include/ion/time/clock.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/time)

# create and install the target files
install(EXPORT ion-core-targets
        FILE ion-core-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)

# generate and install version and config files
get_target_property(ion_core_VERSION ion-core VERSION)
set(ion_core_version_filepath "${CMAKE_CURRENT_BINARY_DIR}/ion-core-config-version.cmake")

write_basic_package_version_file(${ion_core_version_filepath}
        VERSION ${ion_core_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-core-config.cmake ${ion_core_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)
