# Store the package in the user registry.
export(PACKAGE ASIBOT_HMI)

# Retrieve global properties.
get_property(_common_includes GLOBAL PROPERTY ASIBOT_HMI_INCLUDE_DIRS)
get_property(_exported_targets GLOBAL PROPERTY ASIBOT_HMI_TARGETS)

# CMake installation path.
if(WIN32)
    set(_cmake_destination cmake)
else()
    set(_cmake_destination ${CMAKE_INSTALL_LIBDIR}/cmake/ASIBOT_HMI)
endif()

# Create and install config files.
include(CMakePackageConfigHelpers)

# Set exported variables (build tree).
set(ASIBOT_HMI_INCLUDE_DIR "${_common_includes}")
set(ASIBOT_HMI_MODULE_DIR ${CMAKE_SOURCE_DIR}/cmake)

# <pkg>Config.cmake (build tree).
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/templates/ASIBOT_HMIConfig.cmake.in
                              ${CMAKE_BINARY_DIR}/ASIBOT_HMIConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_BINARY_DIR}
                              INSTALL_PREFIX ${CMAKE_BINARY_DIR}
                              PATH_VARS ASIBOT_HMI_INCLUDE_DIR
                                        ASIBOT_HMI_MODULE_DIR
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)

# Set exported variables (install tree).
set(ASIBOT_HMI_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(ASIBOT_HMI_MODULE_DIR ${CMAKE_INSTALL_DATADIR}/ASIBOT_HMI/cmake)

# <pkg>Config.cmake (install tree).
configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/templates/ASIBOT_HMIConfig.cmake.in
                              ${CMAKE_BINARY_DIR}/ASIBOT_HMIConfig.cmake.install
                              INSTALL_DESTINATION ${_cmake_destination}
                              PATH_VARS ASIBOT_HMI_INCLUDE_DIR
                                        ASIBOT_HMI_MODULE_DIR
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)

# Install <pkg>Config.cmake.
install(FILES ${CMAKE_BINARY_DIR}/ASIBOT_HMIConfig.cmake.install
        RENAME ASIBOT_HMIConfig.cmake
        DESTINATION ${_cmake_destination})

# Export library targets if enabled.
# https://github.com/roboticslab-uc3m/project-generator/issues/19
if(_exported_targets)
    # <pkg>Targets.cmake (build tree).
    export(EXPORT ASIBOT_HMI
           NAMESPACE ROBOTICSLAB::
           FILE ASIBOT_HMITargets.cmake)

    # <pkg>Targets.cmake (install tree).
    install(EXPORT ASIBOT_HMI
            DESTINATION ${_cmake_destination}
            NAMESPACE ROBOTICSLAB::
            FILE ASIBOT_HMITargets.cmake)
endif()

# Configure and create uninstall target (YCM).
include(AddUninstallTarget)
