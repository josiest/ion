#
# Define the ion library
#

# compile library
file(GLOB_RECURSE ion_SOURCES "*.cpp")
add_library(ion STATIC ${ion_SOURCES})
set_target_properties(ion PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(raisin REQUIRED)
find_package(tl-expected REQUIRED)
target_link_libraries(ion
    PUBLIC SDL2::SDL2 SDL2::TTF
    INTERFACE raisin::raisin tl::expected)
