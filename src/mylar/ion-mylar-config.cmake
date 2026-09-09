include(CMakeFindDependencyMacro)
find_dependency(SDL3)
find_dependency(EnTT)
include("${CMAKE_CURRENT_LIST_DIR}/ion-mylar-targets.cmake")
