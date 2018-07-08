# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# Start a project.
project(ASIBOT_HMI LANGUAGES CXX)

# Set module path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Hard dependencies.
find_package(YCM 0.8 REQUIRED)
# https://github.com/roboticslab-uc3m/questions-and-answers/issues/65
find_package(YARP 3.0 QUIET)
if(NOT YARP_FOUND)
    find_package(YARP 2.3.70 REQUIRED)
endif()

# Soft dependencies.
find_package(ROBOTICSLAB_KINEMATICS_DYNAMICS QUIET)

# Retrieve current ASIBOT_HMI version.
include(ASIBOT_HMIVersion)

# Define standard paths, check system configuration.
include(ASIBOT_HMISystem)

# Setup build options.
include(ASIBOT_HMIOptions)

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
feature_summary(WHAT ALL)
