# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Start a project.
project(ASIBOT_HMI LANGUAGES CXX)

# Set module path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Hard dependencies.
find_package(YCM 0.11 REQUIRED)
find_package(YARP 3.3 REQUIRED COMPONENTS os dev)

# Soft dependencies.
find_package(Doxygen QUIET)
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
add_subdirectory(doc)

# Output configuration files.
include(ASIBOT_HMIDescribe)

# Invoke CPack from here.
include(ASIBOT_HMIPackage)

# Generate a report of enabled/disabled features.
feature_summary(WHAT ALL)
