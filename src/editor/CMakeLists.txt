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
# Compile and Install
#

# link the required dependencies into a static library
find_package(yaml-cpp REQUIRED CONFIG)
target_link_libraries(ion-editor PUBLIC SDL3::SDL3 yaml-cpp::yaml-cpp ion-engine ion-serialization)
install_ion_module(editor)
