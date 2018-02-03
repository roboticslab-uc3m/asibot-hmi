# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

set(KEYWORD "webInterface")

# Find YARP.
find_package(YARP REQUIRED)

# Find ROBOTICSLAB_KINEMATICS_DYNAMICS.
find_package(ROBOTICSLAB_KINEMATICS_DYNAMICS REQUIRED)

# Retrieve global include paths and libraries.
get_property(ASIBOT_HMI_INCLUDE_DIRS GLOBAL PROPERTY ASIBOT_HMI_INCLUDE_DIRS)
get_property(ASIBOT_HMI_LIBRARIES GLOBAL PROPERTY ASIBOT_HMI_LIBRARIES)

# Define include paths.
include_directories(${YARP_INCLUDE_DIRS} ${ASIBOT_HMI_INCLUDE_DIRS} ${ROBOTICSLAB_KINEMATICS_DYNAMICS_INCLUDE_DIRS})

# Configure executable build target.
add_executable(${KEYWORD} main.cpp WebInterface.cpp WebInterface.hpp WebResponder.cpp WebResponder.hpp)

# Link external libraries.
target_link_libraries(${KEYWORD} ${YARP_LIBRARIES} ${ASIBOT_HMI_LIBRARIES})

# Create installation rule.
install(TARGETS ${KEYWORD}
        EXPORT ASIBOT_HMI
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT web_interface)

# Export library name and current target.
set_property(GLOBAL APPEND PROPERTY ASIBOT_HMI_TARGETS ${KEYWORD})
