# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

# Find ROBOTICSLAB_KINEMATICS_DYNAMICS.
find_package(ROBOTICSLAB_KINEMATICS_DYNAMICS REQUIRED)

# Retrieve global include paths and libraries.
get_property(_common_includes GLOBAL PROPERTY ASIBOT_HMI_INCLUDE_DIRS)

# Define include paths.
include_directories(${_common_includes})

# Configure executable build target.
add_executable(webInterface main.cpp
                            WebInterface.cpp
                            WebInterface.hpp
                            WebResponder.cpp
                            WebResponder.hpp)

# Link external libraries.
target_link_libraries(webInterface YARP::YARP_OS
                                   YARP::YARP_dev
                                   YARP::YARP_init
                                   ROBOTICSLAB::KinematicsDynamicsInterfaces)

# Create installation rule.
install(TARGETS webInterface
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT web_interface)
