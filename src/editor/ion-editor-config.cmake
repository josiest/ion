include(CMakeFindDependencyMacro)
find_dependency(SDL3)
include("${CMAKE_CURRENT_LIST_DIR}/ion-editor-targets.cmake")
add_library(ion::editor ALIAS ion::ion-editor)
