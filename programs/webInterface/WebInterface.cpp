// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "WebInterface.hpp"

#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>
#include <yarp/os/Value.h>
#include <yarp/os/Bottle.h>

using namespace roboticslab;

constexpr auto DEFAULT_PERIOD = 5; // [s]
constexpr auto DEFAULT_RESOURCES = "robots.uc3m.es/webInterface/html";
constexpr auto DEFAULT_WEB_IP = "localhost";
constexpr auto DEFAULT_WEB_PORT = 8080;

/************************************************************************/
WebInterface::WebInterface() { }

/************************************************************************/
bool WebInterface::configure(yarp::os::ResourceFinder &rf)
{
    period = DEFAULT_PERIOD;  // double
    std::string resources = DEFAULT_RESOURCES;
    std::string webIp = DEFAULT_WEB_IP;
    int webPort = DEFAULT_WEB_PORT;

    std::printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        std::printf("WebInterface Options:\n");
        std::printf("\t--period [s] (default: \"%f\")\n",period);
        std::printf("\t--resources (default: \"%s\")\n",resources.c_str());
        std::printf("\t--webIp (default: \"%s\")\n",webIp.c_str());
        std::printf("\t--webPort (default: \"%d\")\n",webPort);
    }

    counter = 0;
    if(rf.check("period")) period = rf.find("period").asFloat64();
    if(rf.check("resources")) resources = rf.find("resources").asString();
    if(rf.check("webIp")) webIp = rf.find("webIp").asString();
    if(rf.check("webPort")) webPort = rf.find("webPort").asInt32();
    std::printf("WebInterface using period: %f, resources: %s.\n", period,resources.c_str());
    std::printf("WebInterface using webIp: %s, webPort: %d.\n",webIp.c_str(),webPort);

    responder.setResourceFinder(rf);
    std::string userPath = rf.getHomeContextPath() + "/";
    std::printf("WebInterface using userPath: %s\n",userPath.c_str());
    responder.setUserPath(userPath);
    std::string resourcePath = "http://";
    resourcePath += resources + "/";
    std::printf("WebInterface using resourcePath: %s\n",resourcePath.c_str());
    responder.setResourcePath(resourcePath);

    std::printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        std::exit(1);
    }

    responder.init();
    server.setReader(responder);

    std::string name = rf.check("name",yarp::os::Value("/web")).asString();

    contact.setName(name);
    if (webPort!=0) {
        contact.setSocket("",webIp,webPort);
    }
    if (!server.open(contact)) return false;
    contact = server.where();

    return true;
}

/************************************************************************/
bool WebInterface::updateModule()
{
    std::printf("Server running, visit: http://%s:%d/index\n",
                   contact.getHost().c_str(),
                   contact.getPort());
    yarp::os::Bottle push;
    push.addString("web");
    std::ostringstream s;
    s << counter;
    std::string div = std::string("<div>")+s.str()+" counter count</div>";
    push.addString(div);
    server.write(push);
    counter++;
    return true;
}

/************************************************************************/
double WebInterface::getPeriod()
{
    return period;  // seconds
}

/************************************************************************/
bool WebInterface::interruptModule()
{
    server.interrupt();
    responder.closeDevices();
    server.close();
    return true;
}

