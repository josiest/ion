include(CMakeFindDependencyMacro)
find_dependency(SDL3)
include("${CMAKE_CURRENT_LIST_DIR}/ion-input-targets.cmake")
add_library(ion::input ALIAS ion::ion-input)
