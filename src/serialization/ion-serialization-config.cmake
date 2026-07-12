include(CMakeFindDependencyMacro)
find_dependency(SDL3)
include("${CMAKE_CURRENT_LIST_DIR}/ion-serialization-targets.cmake")
add_library(ion::serialization ALIAS ion::ion-serialization)
