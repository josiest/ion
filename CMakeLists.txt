cmake_minimum_required(VERSION 3.10)
project(ion)

file(GLOB SOURCES src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2)

set_target_properties(${PROJECT_NAME}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(examples/simple)

