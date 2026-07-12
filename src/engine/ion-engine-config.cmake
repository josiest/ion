include(CMakeFindDependencyMacro)
find_dependency(SDL3)
include("${CMAKE_CURRENT_LIST_DIR}/ion-engine-targets.cmake")
add_library(ion::window ALIAS ion::ion-engine)
