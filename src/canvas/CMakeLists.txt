#
# Define
#

add_library(ion-canvas STATIC)
set_target_properties(ion-canvas PROPERTIES VERSION 0.1.0 SOVERSION 0)
add_library(ion::canvas ALIAS ion-canvas)

target_sources(ion-canvas PRIVATE canvas.cpp)

#
# Compile and Install
#

# link the required dependencies into a static library
find_package(glm REQUIRED)
target_link_libraries(ion-canvas PUBLIC glm::glm ion-photon)
install_ion_module(canvas)
