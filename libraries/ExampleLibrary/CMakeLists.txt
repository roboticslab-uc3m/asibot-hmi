# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

set(KEYWORD "ExampleLibrary")

# Find YARP.
find_package(YARP REQUIRED)

# Add current directory to global include paths, retrieve current values.
set_property(GLOBAL APPEND PROPERTY ASIBOT_HMI_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")
get_property(ASIBOT_HMI_INCLUDE_DIRS GLOBAL PROPERTY ASIBOT_HMI_INCLUDE_DIRS)

# Define include paths.
include_directories(${YARP_INCLUDE_DIRS} ${ASIBOT_HMI_INCLUDE_DIRS})

# Configure library build target.
add_library(${KEYWORD} ExampleLibrary.cpp ExampleLibrary.hpp)

# Link external libraries.
target_link_libraries(${KEYWORD} LINK_PRIVATE ${YARP_LIBRARIES})

# Create installation rule.
install(TARGETS ${KEYWORD}
        EXPORT ASIBOT_HMI
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT example_library
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT example_library
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT example_library)

# Export library name and current target.
set_property(GLOBAL APPEND PROPERTY ASIBOT_HMI_LIBRARIES ${KEYWORD})
set_property(GLOBAL APPEND PROPERTY ASIBOT_HMI_TARGETS ${KEYWORD})
