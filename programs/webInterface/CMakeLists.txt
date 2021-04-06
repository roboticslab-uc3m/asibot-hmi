# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

# Configure executable build target.
add_executable(webInterface main.cpp
                            WebInterface.cpp
                            WebInterface.hpp
                            WebResponder.cpp
                            WebResponder.hpp)

# Link external libraries.
target_link_libraries(webInterface YARP::YARP_os
                                   YARP::YARP_dev
                                   YARP::YARP_init
                                   ROBOTICSLAB::KinematicsDynamicsInterfaces)

# Create installation rule.
install(TARGETS webInterface
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT web_interface)
