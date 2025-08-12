include(CMakeFindDependencyMacro)
find_dependency(SDL2)
include("${CMAKE_CURRENT_LIST_DIR}/ion-core-targets.cmake")
add_library(ion::ion ALIAS ion::ion-core)
