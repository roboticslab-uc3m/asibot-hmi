// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_RESPONDER_HPP__
#define __WEB_RESPONDER_HPP__

#include <string>

#include <yarp/os/PortReader.h>
#include <yarp/os/ConnectionReader.h>
#include <yarp/os/ResourceFinder.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/IPositionControl.h>
#include <yarp/dev/IControlMode.h>

#include <ICartesianControl.h>

#define JOYPAD_RELMOVE 5  // [deg]
#define CJOYPAD_RELMOVE_POS 0.10  // [m]
#define CJOYPAD_RELMOVE_ORI 15  // [deg]
#define NUM_CART_COORDS 5  // 

namespace roboticslab
{

/**
 *
 * @ingroup webInterface
 *
 * The WebResponder class manages YARP web RPCs as callbacks and additionally
 * manages resouces and parses HTML code before acting as a mini-server. Used by
 * WebInterface.
 *
 */
class WebResponder : public yarp::os::PortReader
{
public:

    std::string getCss();
    bool init();
    bool closeDevices();
    bool read(yarp::os::ConnectionReader& in);
    bool setResourceFinder(yarp::os::ResourceFinder &rf);
    bool setUserPath(const std::string& _userPath);
    bool setResourcePath(const std::string& _resourcePath);

protected:

    yarp::os::ResourceFinder rf;
    bool simConnected, realConnected;
    std::string userPath;
    std::string resourcePath;
    std::string readFile(const std::string& filePath);  // needs absoulte path
    std::string readHtml(const std::string& fileName);  // grabs from htmlPath
    bool appendToFile(const std::string& absFile, const std::string& inString); // writes to userPath
    bool rewriteFile(const std::string& absFile, const std::string& inString); // writes to userPath
    bool deleteFile(const std::string& absFile); // needs absoulte path
    std::string& replaceAll(std::string& context, const std::string& from, const std::string& to);
    int stringToInt(const std::string& inString);
    double stringToDouble(const std::string& inString);
    std::string doubleToString(const double& inDouble);
    std::string intToString(const int& inInt);
    std::string pipedExec(const std::string& cmd);
    std::string pointButtonCreator(const std::string& pointsFile);
    std::string wordOptionCreator(const std::string& wordsFile);
    std::string fileListCreator();
    std::string taskListCreator();
    std::string taskButtonCreator();

    yarp::dev::PolyDriver simDevice;
    yarp::dev::IPositionControl *simPos;
    yarp::dev::IControlMode *simMode;
    roboticslab::ICartesianControl *simCart;

    yarp::dev::PolyDriver realDevice;
    yarp::dev::IPositionControl *realPos;
    yarp::dev::IControlMode *realMode;
    roboticslab::ICartesianControl *realCart;

    yarp::dev::PolyDriver cartesianClientDevice;

    double captureX[NUM_CART_COORDS];
    std::string lastEditName;
};

}  //-- namespace roboticslab

#endif  //-- __WEB_RESPONDER_HPP__
