include(CMakeFindDependencyMacro)
include(${CMAKE_CURRENT_LIST_DIR}/konbu-targets.cmake)
find_dependency(yaml-cpp REQUIRED)
check_required_components(${PROJECT_NAME})
