include(CMakeFindDependencyMacro)
find_dependency(SDL2)
include("${CMAKE_CURRENT_LIST_DIR}/ion-window-targets.cmake")
add_library(ion::window ALIAS ion::ion-window)
