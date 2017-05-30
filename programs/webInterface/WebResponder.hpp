// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_RESPONDER_HPP__
#define __WEB_RESPONDER_HPP__

#include <yarp/os/all.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include <ICartesianControl.h>

#include <fstream>
#include <sstream>
#include <vector>

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>  // for listing directory contents
#endif

#include <stdlib.h>  // for system

#define JOYPAD_RELMOVE 5  // [deg]
#define CJOYPAD_RELMOVE_POS 0.10  // [m]
#define CJOYPAD_RELMOVE_ORI 15  // [deg]
#define NUM_CART_COORDS 5  // 

namespace roboticslab
{

using namespace yarp::os;  // FIXME
using namespace std;  // FIXME

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

    yarp::os::ConstString getCss();
    bool init();
    bool closeDevices();
    bool read(yarp::os::ConnectionReader& in);
    bool setResourceFinder(yarp::os::ResourceFinder &rf);
    bool setUserPath(const yarp::os::ConstString& _userPath);
    bool setResourcePath(const yarp::os::ConstString& _resourcePath);

protected:

    yarp::os::ResourceFinder rf;
    bool simConnected, realConnected;
    yarp::os::ConstString userPath;
    yarp::os::ConstString resourcePath;
    std::string readFile(const yarp::os::ConstString& filePath);  // needs absoulte path
    std::string readHtml(const yarp::os::ConstString& fileName);  // grabs from htmlPath
    bool appendToFile(const yarp::os::ConstString& absFile, const yarp::os::ConstString& inString); // writes to userPath
    bool rewriteFile(const yarp::os::ConstString& absFile, const yarp::os::ConstString& inString); // writes to userPath
    bool deleteFile(const yarp::os::ConstString& absFile); // needs absoulte path
    std::string& replaceAll(std::string& context, const std::string& from, const std::string& to);
    int stringToInt(const yarp::os::ConstString& inString);
    double stringToDouble(const yarp::os::ConstString& inString);
    yarp::os::ConstString doubleToString(const double& inDouble);
    yarp::os::ConstString intToString(const int& inInt);
    yarp::os::ConstString pipedExec(const yarp::os::ConstString& cmd);
    yarp::os::ConstString pointButtonCreator(const yarp::os::ConstString& pointsFile);
    yarp::os::ConstString wordOptionCreator(const yarp::os::ConstString& wordsFile);
    yarp::os::ConstString fileListCreator();
    yarp::os::ConstString taskListCreator();
    yarp::os::ConstString taskButtonCreator();

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
    yarp::os::ConstString lastEditName;
};

}  //-- namespace roboticslab

#endif  //-- __WEB_RESPONDER_HPP__
