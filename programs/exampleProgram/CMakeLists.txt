# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

set(KEYWORD "exampleProgram")

# Find YARP.
find_package(YARP REQUIRED)

# Retrieve global include paths and libraries.
get_property(ASIBOT_HMI_INCLUDE_DIRS GLOBAL PROPERTY ASIBOT_HMI_INCLUDE_DIRS)
get_property(ASIBOT_HMI_LIBRARIES GLOBAL PROPERTY ASIBOT_HMI_LIBRARIES)

# Define include paths.
include_directories(${YARP_INCLUDE_DIRS} ${ASIBOT_HMI_INCLUDE_DIRS})

# Configure executable build target.
add_library(${KEYWORD} main.cpp ExampleProgram.cpp ExampleProgram.hpp)

# Link external libraries.
target_link_libraries(${KEYWORD} ${YARP_LIBRARIES} ${ASIBOT_HMI_LIBRARIES})

# Create installation rule.
install(TARGETS ${_example_target}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT example_program)

