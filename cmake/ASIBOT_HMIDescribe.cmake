# Store the package in the user registry.
export(PACKAGE ASIBOT_HMI)

# Retrieve global properties.
get_property(_exported_targets GLOBAL PROPERTY ASIBOT_HMI_TARGETS)

# CMake installation path.
if(WIN32)
    set(_cmake_destination cmake)
else()
    set(_cmake_destination ${CMAKE_INSTALL_LIBDIR}/cmake/ASIBOT_HMI)
endif()

# <pkg>Config.cmake (build tree).
file(WRITE ${CMAKE_BINARY_DIR}/ASIBOT_HMIConfig.cmake
     "include(\${CMAKE_CURRENT_LIST_DIR}/ASIBOT_HMITargets.cmake)")

# Install <pkg>Config.cmake.
install(FILES ${CMAKE_BINARY_DIR}/ASIBOT_HMIConfig.cmake
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
