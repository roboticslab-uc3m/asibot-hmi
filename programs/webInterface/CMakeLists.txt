# Copyright: Universidad Carlos III de Madrid (C) 2017
# Authors: Robotics Lab UC3M
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

add_executable(webInterface main.cpp
                            WebInterface.cpp
                            WebInterface.hpp
                            WebResponder.cpp
                            WebResponder.hpp)

target_link_libraries(webInterface YARP::YARP_os
                                   YARP::YARP_dev
                                   YARP::YARP_init
                                   ROBOTICSLAB::KinematicsDynamicsInterfaces)

install(TARGETS webInterface
        COMPONENT web_interface)
