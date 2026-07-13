#
# Define the library
#

add_library(ion-mylar INTERFACE)
add_library(ion::mylar ALIAS ion-mylar)

target_sources(ion-mylar PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/ion/mylar
    FILES
        ${CMAKE_SOURCE_DIR}/include/ion/mylar/reflect.hpp)

set_target_properties(ion-mylar PROPERTIES
    CXX_STANDARD 23
    CX_STANDARD_REQUIRED True
    VERSION 0.2.0
    SOVERSION 0)

#
# Compile and Install the library
#

find_package(EnTT REQUIRED CONFIG)
find_package(SDL3 REQUIRED CONFIG)
target_link_libraries(ion-mylar INTERFACE SDL3::SDL3 EnTT::EnTT)
install_ion_module(mylar)
