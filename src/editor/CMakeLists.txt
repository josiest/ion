#
# Define
#

add_library(ion-editor STATIC)
set_target_properties(ion-editor PROPERTIES VERSION 0.1.0 SOVERSION 0)
add_library(ion::editor ALIAS ion-editor)

target_sources(ion-editor
   PRIVATE
       editor.cpp

   PUBLIC FILE_SET HEADERS
   BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/editor
   FILES
       ${CMAKE_SOURCE_DIR}/include/ion/editor/editor.hpp)

#
# Compile
#

# link the required dependencies into a static library
find_package(yaml-cpp REQUIRED CONFIG)
target_link_libraries(ion-editor PUBLIC SDL3::SDL3 yaml-cpp::yaml-cpp ion-engine ion-serialization)

# specify the include directories for ion-editor
target_include_directories(ion-editor PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

#
# Install the library
#

# install the compiled object and library files
install(TARGETS ion-editor
        EXPORT ion-editor-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/editor
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/editor)

# install the header files for the editor component
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/editor.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

# create and install the target files
install(EXPORT ion-editor-targets
        FILE ion-editor-targets.cmake
        NAMESPACE ion::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/editor)

# generate and install version and config files
get_target_property(ion_editor_VERSION ion-editor VERSION)
set(ion_editor_version_filepath "${CMAKE_CURRENT_BINARY_DIR}/ion-editor-config-version.cmake")

write_basic_package_version_file(${ion_editor_version_filepath}
        VERSION ${ion_editor_VERSION}
        COMPATIBILITY AnyNewerVersion)

# install the config files
install(FILES ion-editor-config.cmake ${ion_editor_version_filepath}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/editor)
