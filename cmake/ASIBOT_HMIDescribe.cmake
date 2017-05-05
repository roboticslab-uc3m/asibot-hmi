# Store the package in the user registry.
export(PACKAGE ASIBOT_HMI)

# Retrieve global properties.
get_property(ASIBOT_HMI_INCLUDE_DIRS GLOBAL PROPERTY ASIBOT_HMI_INCLUDE_DIRS)
get_property(ASIBOT_HMI_TARGETS GLOBAL PROPERTY ASIBOT_HMI_TARGETS)
get_property(_ASIBOT_HMI_LIBRARIES GLOBAL PROPERTY ASIBOT_HMI_LIBRARIES)

# Append namespace prefix to exported libraries.
set(ASIBOT_HMI_LIBRARIES)
foreach(lib ${_ASIBOT_HMI_LIBRARIES})
  list(APPEND ASIBOT_HMI_LIBRARIES ASIBOT_HMI::${lib})
endforeach()
unset(_ASIBOT_HMI_LIBRARIES) # just in case

# Set build/install pairs of paths for each exported property.
set(ASIBOT_HMI_BUILD_INCLUDE_DIRS ${ASIBOT_HMI_INCLUDE_DIRS})
set(ASIBOT_HMI_INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_FULL_INCLUDEDIR})

# Create and install config and version files (YCM).
include(InstallBasicPackageFiles)

install_basic_package_files(ASIBOT_HMI
                            VERSION ${ASIBOT_HMI_VERSION_SHORT}
                            COMPATIBILITY AnyNewerVersion
                            TARGETS_PROPERTY ASIBOT_HMI_TARGETS
                            NO_CHECK_REQUIRED_COMPONENTS_MACRO
                            EXTRA_PATH_VARS_SUFFIX INCLUDE_DIRS)

# Configure and create uninstall target (YCM).
include(AddUninstallTarget)

