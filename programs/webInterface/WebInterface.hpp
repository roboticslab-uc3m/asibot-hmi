// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_INTERFACE_HPP__
#define __WEB_INTERFACE_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/Port.h>
#include <yarp/os/Contact.h>

#include "WebResponder.hpp"

namespace roboticslab
{

/**
 * @ingroup webInterface
 */
class WebInterface : public yarp::os::RFModule
{
public:

    WebInterface();
    bool configure(yarp::os::ResourceFinder &rf) override;

protected:

    yarp::os::Port server;
    WebResponder responder;
    yarp::os::Contact contact;  // will get webIp and webPort

    bool updateModule() override;
    bool interruptModule() override;
    double getPeriod() override;

    double period;
    int counter;
};

} //-- namespace roboticslab

#endif //-- __WEB_INTERFACE_HPP__
