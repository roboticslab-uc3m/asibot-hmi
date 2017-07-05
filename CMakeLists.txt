# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

# Start a project.
project(ASIBOT_HMI)

# Set module path.
set(ASIBOT_HMI_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${ASIBOT_HMI_MODULE_PATH})

# Bootstrap YCM.
# https://github.com/robotology/ycm/issues/118
set(YCM_TAG v0.2.3)
include(YCMBootstrap)

# Retrieve current ASIBOT_HMI version.
include(ASIBOT_HMIVersion)

# Define standard paths, check system configuration.
include(ASIBOT_HMISystem)

# Setup build options.
include(ASIBOT_HMIOptions)

# Define global properties to store relevant configuration.
set_property(GLOBAL PROPERTY ASIBOT_HMI_INCLUDE_DIRS)
set_property(GLOBAL PROPERTY ASIBOT_HMI_LIBRARIES)
set_property(GLOBAL PROPERTY ASIBOT_HMI_TARGETS)

# Define and enter subdirectories.
add_subdirectory(share)
#add_subdirectory(libraries)
add_subdirectory(programs)

# Output configuration files.
include(ASIBOT_HMIDescribe)

# Create target for documentation generation.
include(ASIBOT_HMIDoc)

# Invoke CPack from here.
include(ASIBOT_HMIPackage)

# Generate a report of enabled/disabled features.
feature_summary(FATAL_ON_MISSING_REQUIRED_PACKAGES WHAT ALL)

