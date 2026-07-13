# --- project setup ---
cmake_minimum_required(VERSION 3.23)
project(ion VERSION 0.9.0)

#
# Export and install the libary
#

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

function(install_interface_targets MODULE_NAME)
    target_include_directories(ion-${MODULE_NAME} INTERFACE
            "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

    install(TARGETS ion-${MODULE_NAME}
            EXPORT ion-${MODULE_NAME}-targets
            FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/${MODULE_NAME})
endfunction()

function(install_static_targets MODULE_NAME)
    target_include_directories(ion-${MODULE_NAME} PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

    install(TARGETS ion-${MODULE_NAME}
            EXPORT ion-${MODULE_NAME}-targets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/ion/${MODULE_NAME}
            FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/${MODULE_NAME})
endfunction()

function(install_ion_module MODULE_NAME)

    get_target_property(ion_module_type ion-${MODULE_NAME} TYPE)
    if(${ion_module_type} STREQUAL STATIC_LIBRARY)
        install_static_targets(${MODULE_NAME})
    elseif(${ion_module_type} STREQUAL INTERFACE_LIBRARY)
        install_interface_targets(${MODULE_NAME})
    endif()

    install(EXPORT ion-${MODULE_NAME}-targets
            FILE ion-${MODULE_NAME}-targets.cmake
            NAMESPACE ion::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/${MODULE_NAME})

    # generate and install version and config files
    get_target_property(ion_module_VERSION ion-${MODULE_NAME} VERSION)
    set(ion_module_version_filepath "${CMAKE_CURRENT_BINARY_DIR}/ion-${MODULE_NAME}-config-version.cmake")

    write_basic_package_version_file(${ion_module_version_filepath}
            VERSION ${ion_module_VERSION}
            COMPATIBILITY AnyNewerVersion)

    # install the config files
    install(FILES ion-${MODULE_NAME}-config.cmake ${ion_module_version_filepath}
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion/${MODULE_NAME})
endfunction()

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
add_subdirectory(src/containers)
add_subdirectory(src/mylar)
add_subdirectory(src)

add_subdirectory(src/engine)
add_subdirectory(src/serialization)
add_subdirectory(src/editor)
add_subdirectory(src/input)

# create and install config files
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/ion-config-version.cmake"
                                 VERSION ${CMAKE_PROJECT_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

configure_package_config_file(ion-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/ion-config.cmake"
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/ion-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/ion-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ion)