// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_INTERFACE_HPP__
#define __WEB_INTERFACE_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/Port.h>
#include <yarp/os/Contact.h>

#include "WebResponder.hpp"

#define DEFAULT_PERIOD 5   // [s]
#define DEFAULT_RESOURCES "robots.uc3m.es/webInterface/html"
#define DEFAULT_WEB_IP "localhost"
#define DEFAULT_WEB_PORT 8080

namespace roboticslab
{

/**
 * @ingroup webInterface
 */
class WebInterface : public yarp::os::RFModule
{
public:

    WebInterface();
    bool configure(yarp::os::ResourceFinder &rf);

protected:

    yarp::os::Port server;
    WebResponder responder;
    yarp::os::Contact contact;  // will get webIp and webPort

    bool updateModule();
    bool interruptModule();
    double getPeriod();

    double period;
    int counter;
};

}  //-- namespace roboticslab

#endif  //-- __WEB_INTERFACE_HPP__
