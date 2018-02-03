// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "WebResponder.hpp"

#include <cstdio>  // printf, feof, fgets
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>  // for listing directory contents
#endif

/************************************************************************/
bool roboticslab::WebResponder::init()
{
    simConnected = false;
    realConnected = false;
    simPos = 0;
    realPos = 0;
    simCart = 0;
    realCart = 0;
    simMode = 0;
    realMode = 0;
    lastEditName = "[none]";
    return true;
}

/************************************************************************/
bool roboticslab::WebResponder::closeDevices()
{
    if(realDevice.isValid())
        realDevice.close();
    if(simDevice.isValid())
        simDevice.close();
    if(cartesianClientDevice.isValid())
        cartesianClientDevice.close();
    return true;
}

/************************************************************************/
bool roboticslab::WebResponder::setResourceFinder(yarp::os::ResourceFinder &rf)
{
    this->rf = rf;
    return true;
}

/************************************************************************/
bool roboticslab::WebResponder::setUserPath(const std::string& _userPath)
{
    userPath = _userPath;
    return true;
}

/************************************************************************/
bool roboticslab::WebResponder::setResourcePath(const std::string& _resourcePath)
{
    resourcePath = _resourcePath;
    return true;
}

/************************************************************************/
std::string& roboticslab::WebResponder::replaceAll(std::string& context, const std::string& from, const std::string& to)
{
    // thank you Bruce Eckel for this one!! (TICPP-2nd-ed-Vol-two)
    std::size_t lookHere = 0;
    std::size_t foundHere;
    while((foundHere = context.find(from, lookHere)) != std::string::npos) {
        context.replace(foundHere, from.size(), to);
        lookHere = foundHere + to.size();
    }
    return context;
}

/************************************************************************/
std::string roboticslab::WebResponder::readHtml(const std::string& fileName)
{
    std::string filePath = rf.findFileByName("html/"+fileName);
    return readFile(filePath);
}

/************************************************************************/
std::string roboticslab::WebResponder::readFile(const std::string& filePath)
{
    std::printf("filePath: %s\n",filePath.c_str());
    // thank you Tyler McHenry @ nerdland.net and KeithB @ ndssl.vbi.vt.edu for this algorithm
    // link: http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring [2012-02-06]
    std::ifstream t(filePath.c_str());
    std::string str;
    if(!t.is_open()) {
        std::printf("Not able to open file.\n");
        return str;
    }
    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
    t.close();
    std::string resourceURL = resourcePath + "fig/";
    replaceAll(str, "fig/", resourceURL.c_str());
    if(simConnected) replaceAll(str, "simInit.jpg", "simCon.jpg");
    else replaceAll(str, "simInit.jpg", "simDis.jpg");
    if(realConnected) replaceAll(str, "realInit.jpg", "realCon.jpg");
    else replaceAll(str, "realInit.jpg", "realDis.jpg");
    return str;
}

/************************************************************************/
bool roboticslab::WebResponder::appendToFile(const std::string& fileName, const std::string& inString)
{
    std::string filePath = userPath + fileName;
    std::printf("saving: %s\n",inString.c_str());
    std::printf("to file: %s\n",filePath.c_str());
    std::ofstream t(filePath.c_str(), std::ios::app);
    t << inString << std::endl;
    t.close();
    return true;
}

/************************************************************************/
bool roboticslab::WebResponder::rewriteFile(const std::string& fileName, const std::string& inString)
{
    std::string filePath = userPath + fileName;
    std::printf("rewriting: %s\n",inString.c_str());
    std::printf("to file: %s\n",filePath.c_str());
    std::ofstream t(filePath.c_str());
    t << inString << std::endl;
    t.close();
    return true;
}

/************************************************************************/
bool roboticslab::WebResponder::deleteFile(const std::string& absFile)
{
    if (std::remove(absFile.c_str()) != 0 ) {
        std::printf("[error] could not delete file");
        return false;
    } else std::printf("Deleted %s",absFile.c_str());
    return true;
}

/************************************************************************/
int roboticslab::WebResponder::stringToInt(const std::string& inString)
{
    int outInt;
    std::istringstream buffer(inString.c_str());
    buffer >> outInt;
    if(inString=="") outInt = 0;
    return outInt;
}

/************************************************************************/
double roboticslab::WebResponder::stringToDouble(const std::string& inString)
{
    double outDouble;
    std::istringstream buffer(inString.c_str());
    buffer >> outDouble;
    if(inString=="") outDouble = 0.0;
    return outDouble;
}

/************************************************************************/
std::string roboticslab::WebResponder::doubleToString(const double& inDouble)
{
    // [thank you Adam Rosenfield] http://stackoverflow.com/questions/1123201/convert-double-to-string-c
    std::ostringstream s;
    s << inDouble;
    return std::string(s.str().c_str());
}

/************************************************************************/
std::string roboticslab::WebResponder::intToString(const int& inInt)
{
    std::ostringstream s;
    s << inInt;
    return std::string(s.str().c_str());
}

/************************************************************************/
std::string roboticslab::WebResponder::pipedExec(const std::string& cmd)
{
    // http://stackoverflow.com/a/478960
    const int bufferSize = 128;
    char buffer[bufferSize];
    std::string result = "";
    FILE *(*ppopen)(const char *, const char *);
    int (*ppclose)(FILE *);
#ifdef WIN32
    ppopen = _popen;
    ppclose = _pclose;
#else
    ppopen = ::popen;
    ppclose = ::pclose;
#endif
    FILE* pipe = ppopen(cmd.c_str(), "r");
    if (!pipe) return "could not execute command, popen() failed";
    try {
        while (!std::feof(pipe)) {
            if (std::fgets(buffer, bufferSize, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        ppclose(pipe);
        return "error, aborting command: " + cmd;
    }
    ppclose(pipe);
    return result;
}

/************************************************************************/
std::string roboticslab::WebResponder::pointButtonCreator(const std::string& pointsFile)
{
    std::string ret;
    std::printf("Reading points from file: %s\n",pointsFile.c_str());
    std::ifstream ifs(pointsFile.c_str());
    if (!ifs.is_open()) {
        std::printf("[warning] point file not open, assuming no points yet.\n");
        ret += "[No points captured yet]";
        return ret;
    }
    std::string line;
    while (std::getline(ifs, line)) {
        line += ' ';  // add a comma for easier parsing
        std::printf("line: %s.\n",line.c_str());
        int npos=0;
        int lpos=0;
        std::string pointName;
        std::string values;
        while ((npos = (int)line.find(' ', lpos)) != std::string::npos) {
            std::string subs(line.substr(lpos, npos - lpos).c_str());
            if (lpos==0) pointName = subs;
            else {
                values += subs;
                values += ",";
            }
            lpos = npos+1;
        }
        // ret empty for now
        ret += "<button onClick=\"pointToText('";
        ret += pointName.c_str();
        ret += "','";
        std::string tvalues = values.substr(0,values.length()-1);
        ret += tvalues.c_str();
        ret += "');\">";
        ret += pointName.c_str();
        ret += "</button><br>";
    }
    std::printf("Done reading points from file: %s\n",pointsFile.c_str());
    return ret;
}

/************************************************************************/
std::string roboticslab::WebResponder::wordOptionCreator(const std::string& wordsFile)
{
    std::string ret;
    std::printf("Reading words from file: %s\n",wordsFile.c_str());
    std::ifstream ifs(wordsFile.c_str());
    if (!ifs.is_open()) {
        std::printf("[warning] word file not open, assuming no words yet.\n");
        ret += "[No words captured yet]";
        return ret;
    }
    std::string line;
    while (std::getline(ifs, line)) {
        line += ' ';  // add a comma for easier parsing
        std::printf("line: %s.\n",line.c_str());
        int npos=0;
        int lpos=0;
        std::string pointName;
        std::string values;
        while ((npos = (int)line.find(' ', lpos)) != std::string::npos) {
            std::string subs(line.substr(lpos, npos - lpos).c_str());
            if (lpos==0) pointName = subs;
            else {
                values += subs;
                values += ",";
            }
            lpos = npos+1;
        }
        // ret empty for now
        ret += "<option>";
        ret += line.c_str();
        ret += "</option>";
    }
    std::printf("Done reading words from file: %s\n",wordsFile.c_str());
    return ret;
}

#ifdef WIN32
std::string roboticslab::WebResponder::fileListCreator() {
    std::string ret;
    std::string filePath = userPath.substr(0, filePath.size() - 1) + "\\*";
    std::printf("Reading files from: %s\n", filePath.c_str());
    HANDLE hFind;
    WIN32_FIND_DATA ffd;
    if ((hFind = FindFirstFile(filePath.c_str(), &ffd)) != INVALID_HANDLE_VALUE) {
        do {
            std::string fileName(ffd.cFileName);
            if (fileName.size() > 3 && (int)fileName.rfind(".py") == fileName.size() - 3) {
                printf("[%s] was py\n", fileName.c_str());
                ret += "<option>";
                ret += fileName.substr(0, fileName.size() - 3).c_str();
                ret += "</option>";
            }
        } while (FindNextFile(hFind, &ffd));
        FindClose(hFind);
    }
    else std::printf("[warning] Couldn't open the directory\n");
    std::printf("Done reading files from: %s\n", filePath.c_str());
    return ret;
}

/************************************************************************/
std::string roboticslab::WebResponder::taskListCreator() {
    std::string ret;
    std::string filePath = userPath.substr(0, filePath.size() - 1) + "\\*";
    std::printf("Reading files from: %s\n", filePath.c_str());
    HANDLE hFind;
    WIN32_FIND_DATA ffd;
    if ((hFind = FindFirstFile(filePath.c_str(), &ffd)) != INVALID_HANDLE_VALUE) {
        do {
            std::string fileName(ffd.cFileName);
            if ((int)fileName.find(".task", 0) != std::string::npos) {
                std::printf("[%s] was task\n", fileName.c_str());
                ret += "<option>";
                ret += fileName.substr(0, fileName.size() - 5).c_str();
                ret += "</option>";
            }
        } while (FindNextFile(hFind, &ffd));
        FindClose(hFind);
    }
    else std::printf("[warning] Couldn't open the directory\n");
    std::printf("Done reading files from: %s\n", filePath.c_str());
    return ret;
}

/************************************************************************/
std::string roboticslab::WebResponder::taskButtonCreator() {
    std::string ret;
    std::string filePath = userPath.substr(0, filePath.size() - 1) + "\\*";
    std::printf("Reading files from: %s\n", filePath.c_str());
    HANDLE hFind;
    WIN32_FIND_DATA ffd;
    if ((hFind = FindFirstFile(filePath.c_str(), &ffd)) != INVALID_HANDLE_VALUE) {
        do {
            std::string fileName(ffd.cFileName);
            int taskCount = 0;
            if ((int)fileName.find(".task", 0) != std::string::npos) {
                std::printf("[%s] was task, contents...\n", fileName.c_str());
                std::string taskPath(userPath);
                taskPath += fileName.c_str();
                std::ifstream ifs(taskPath.c_str());
                if (!ifs.is_open()) {
                    std::printf("[warning] word file not open, assuming no words yet.\n");
                    ret += "[error]";
                    return ret;
                }
                std::string line;
                int lineCount = 0;
                std::string strProgram;
                std::string strSpeech;
                std::string strIcon;
                while (std::getline(ifs, line)) {
                    if (lineCount == 0) strProgram = line;
                    else if (lineCount == 1) strSpeech = line;
                    else if (lineCount == 2) strIcon = line;
                    lineCount++;
                }
                std::printf("Program: %s\nSpeech: %s\nIcon: %s\n", strProgram.c_str(), strSpeech.c_str(), strIcon.c_str());
                ret += "<button name='program' type='button' value='";
                ret += strProgram.c_str();
                ret += "'><img src='";
                ret += resourcePath;
                ret += "fig/";
                ret += strIcon.c_str();
                ret += "' height='200'></button>";
                //                printf("<button name='program' type='submit' value=");
                //                printf("%s", line.c_str());
                //                printf("</button>\n");
            }
            if (taskCount % 2 == 0) ret += " &nbsp;\n";
            else ret += "<br><br>\n";
            taskCount++;
        } while (FindNextFile(hFind, &ffd));
        FindClose(hFind);
    }
    else std::printf("[warning] Couldn't open the directory\n");
    std::printf("Done reading files from: %s\n", filePath.c_str());
    return ret;
}
#else

/************************************************************************/
std::string roboticslab::WebResponder::fileListCreator()
{
    std::string ret;
    std::string filePath = userPath;
    std::printf("Reading files from: %s\n",filePath.c_str());
    DIR *dp;
    struct dirent *ep;
    dp = ::opendir(filePath.c_str());
    if (dp != NULL) {
        while (ep = ::readdir(dp)) {
            std::string fileName(ep->d_name);
            if(fileName.size() > 3 && (int)fileName.rfind(".py") == fileName.size()-3) {
                std::printf("[%s] was py\n",fileName.c_str());
                ret += "<option>";
                ret += fileName.substr(0, fileName.size()-3).c_str();
                ret += "</option>";
            }
        }
       (void) ::closedir(dp);
    } else std::printf("[warning] Couldn't open the directory\n");
    std::printf("Done reading files from: %s\n",filePath.c_str());
    return ret;
}

/************************************************************************/
std::string roboticslab::WebResponder::taskListCreator()
{
    std::string ret;
    std::string filePath = userPath;
    std::printf("Reading files from: %s\n",filePath.c_str());
    DIR *dp;
    struct dirent *ep;
    dp = ::opendir(filePath.c_str());
    if (dp != NULL) {
        while (ep = ::readdir(dp)) {
            std::string fileName(ep->d_name);
            if((int)fileName.find(".task", 0) != std::string::npos) {
                std::printf("[%s] was task\n",fileName.c_str());
                ret += "<option>";
                ret += fileName.substr(0, fileName.size()-5).c_str();
                ret += "</option>";
            }
        }
       (void) ::closedir(dp);
    } else std::printf("[warning] Couldn't open the directory\n");
    std::printf("Done reading files from: %s\n",filePath.c_str());
    return ret;
}

/************************************************************************/
std::string roboticslab::WebResponder::taskButtonCreator()
{
    std::string ret;
    std::string filePath = userPath;
    std::printf("Reading files from: %s\n",filePath.c_str());
    DIR *dp;
    struct dirent *ep;
    dp = ::opendir(filePath.c_str());
    if (dp != NULL) {
        while (ep = ::readdir(dp)) {
            std::string fileName(ep->d_name);
            int taskCount = 0;
            if((int)fileName.find(".task", 0) != std::string::npos) {
                std::printf("[%s] was task, contents...\n",fileName.c_str());
                std::string taskPath(userPath);
                taskPath += fileName.c_str();
                std::ifstream ifs(taskPath.c_str());
                if (!ifs.is_open()) {
                    std::printf("[warning] word file not open, assuming no words yet.\n");
                    ret += "[error]";
                    return ret;
                }
                std::string line;
                int lineCount = 0;
                std::string strProgram;
                std::string strSpeech;
                std::string strIcon;
                while (std::getline(ifs, line)) {
                    if (lineCount==0) strProgram = line;
                    else if (lineCount==1) strSpeech = line;
                    else if (lineCount==2) strIcon = line;
                    lineCount++;
                }
                std::printf("Program: %s\nSpeech: %s\nIcon: %s\n",strProgram.c_str(),strSpeech.c_str(),strIcon.c_str());
                ret += "<button name='program' type='button' value='";
                ret += strProgram.c_str();
                ret += "'><img src='";
                ret += resourcePath;
                ret += "fig/";
                ret += strIcon.c_str();
                ret += "' height='200'></button>";
//                printf("<button name='program' type='submit' value=");
//                printf("%s", line.c_str());
//                printf("</button>\n");
            }
            if (taskCount%2==0) ret += " &nbsp;\n";
            else ret += "<br><br>\n";
            taskCount++;
        }
        (void) ::closedir(dp);
    } else std::printf("[warning] Couldn't open the directory\n");
    std::printf("Done reading files from: %s\n",filePath.c_str());
    return ret;
}
#endif

/************************************************************************/
std::string roboticslab::WebResponder::getCss()
{
    return std::string(readHtml("style.css").c_str());
}

/************************************************************************/
bool roboticslab::WebResponder::read(yarp::os::ConnectionReader& in)
{
    yarp::os::Bottle request, response;
    if (!request.read(in)) return false;
    std::printf("Request: %s\n", request.toString().c_str());
    yarp::os::ConnectionWriter *out = in.getWriter();
    if (out==NULL) return true;
    response.addString("web");

    std::string code = request.get(0).asString();
    if (code=="style.css") {
        response.addString(getCss());
        response.addString("mime");
        response.addString("text/css");
        return response.write(*out);
    } else if (code=="xmlhttp.js") {
        response.addString(readHtml("xmlhttp.js").c_str());
        return response.write(*out);
    } else if (code=="connectionTab.js") {
        response.addString(readHtml("connectionTab.js").c_str());
        return response.write(*out);
    } else if (code=="testEqual") {
        response.addString(readHtml("testEqual.html").c_str());
        return response.write(*out);
    } else if (code=="equal.1") {
        std::string inParam = request.find("a").asString();
        std::printf("Got an %s, going to equal it.\n",inParam.c_str());
        response.addString(inParam);
        return response.write(*out);
    } else if (code=="connectReal.1") {
        std::string inParam = request.find("real").asString();
        std::printf("Got %s. ",inParam.c_str());
        std::string outParam;
        if (realConnected){
            std::printf("Disconnecting from real robot.\n");
            realDevice.close();
            realConnected = false;
            realPos = 0;
            // Maybe perform some checks here
            outParam = "REALOFF";
        } else {
            std::printf("Connecting to real robot.\n");
            yarp::os::Property options;
            options.put("device","remote_controlboard");
            options.put("remote","/canbot");
            options.put("local","/webLocal");
            realDevice.open(options);
            bool ok = true;
            if(!realDevice.isValid()) {
                std::printf("[error] canbot device not available.\n");
                ok = false;
            } else std::printf ("[success] canbot device available.\n");
            if(!realDevice.view(realPos) || !realDevice.view(realMode)) {
                std::printf("[error] canbot interface not available.\n");
                ok = false;
            } else std::printf ("[success] canbot interface available.\n");
            if (!ok) {
                realDevice.close();
                realConnected = false;
                realPos = 0;
                outParam = "REALOFF";
            } else {
                realConnected = true;
                outParam = "REALON";
            }
        }
        response.addString(outParam);
        return response.write(*out);
    } else if (code=="connectSim.1") {
        std::string inParam = request.find("sim").asString();
        std::printf("Got %s. ",inParam.c_str());
        std::string outParam;
        if (simConnected){
            std::printf("Disconnecting from robot simulator.\n");
            simDevice.close();
            cartesianClientDevice.close();
            simConnected = false;
            simPos = 0;
            // Maybe perform some checks here
            outParam = "SIMOFF";
        } else {
            std::printf("Connecting to robot simulator.\n");
            yarp::os::Property optionsDevice;
            optionsDevice.put("device","remote_controlboard");
            optionsDevice.put("remote","/asibot/asibotManipulator");
            optionsDevice.put("local","/webLocal");
            simDevice.open(optionsDevice);
            bool ok = true;
            if(!simDevice.isValid()) {
                std::printf("[error] ravebot device not available.\n");
                ok = false;
            } else printf ("[success] ravebot device available.\n");
            if(!simDevice.view(simPos) || !simDevice.view(simMode)) {
                std::printf("[error] ravebot simPos not available.\n");
                ok = false;
            } else std::printf ("[success] ravebot simPos available.\n");
            std::printf("Connecting to cartesian client.\n");
            yarp::os::Property optionsCartesian;
            optionsCartesian.put("device", "CartesianControlClient");
            optionsCartesian.put("cartesianRemote", "/cartesianRemote");
            optionsCartesian.put("cartesianLocal", "/cartesianLocal");
            cartesianClientDevice.open(optionsCartesian);
            if(!cartesianClientDevice.isValid()) {
                std::printf("[error] cartesianClientDevice/ravebot device not available.\n");
                ok = false;
            } else std::printf ("[success] cartesianClientDevice/ravebot device available.\n");
            if(!cartesianClientDevice.view(simCart)) {
                std::printf("[error] cartesianClientDevice/ravebot simCart not available.\n");
                ok = false;
            } else std::printf ("[success] cartesianClientDevice/ravebot simCart available.\n");
            if(!ok) {
                simDevice.close();
                cartesianClientDevice.close();
                simConnected = false;
                simPos = 0;
                outParam = "SIMOFF";
            } else {
                simConnected = true;
                outParam = "SIMON";
            }
        }
        response.addString(outParam);
        return response.write(*out);
    } else if (code=="index") {
        response.addString(readHtml("index.html").c_str());
        return response.write(*out);
    } else if (code=="joint") {
        std::string str = readHtml("joint.html");
        //TCbegin
        std::string taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<JOPTS>", "<script src='jTaskTab.js' type='text/javascript'></script>");
        }
        //TCend
        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="joint.1") {
        std::string theJoint = request.find("joint").asString();
        int inJoint = stringToInt(theJoint);
        std::string inMovement = request.find("movement").asString();
        std::printf("Going to move joint [%d] towards the [%s].\n", inJoint, inMovement.c_str());
        if(simPos) {
            int ax;
            simPos->getAxes(&ax);
            std::vector<int> modes(ax, VOCAB_CM_POSITION);
            simMode->setControlModes(modes.data());
        }
        if((simPos)&&(inMovement == std::string("right"))) simPos->relativeMove(inJoint-1,JOYPAD_RELMOVE);
        if((simPos)&&(inMovement == std::string("left"))) simPos->relativeMove(inJoint-1,-JOYPAD_RELMOVE);
        if(realPos) {
            int ax;
            realPos->getAxes(&ax);
            std::vector<int> modes(ax, VOCAB_CM_POSITION);
            realMode->setControlModes(modes.data());
        }
        if((realPos)&&(inMovement == std::string("right"))) realPos->relativeMove(inJoint-1,JOYPAD_RELMOVE);
        if((realPos)&&(inMovement == std::string("left"))) realPos->relativeMove(inJoint-1,-JOYPAD_RELMOVE);
        return response.write(*out);
    } else if (code=="joint.2") {
        std::string inMovement = request.find("movement").asString();
        std::string q1 = request.find("one").asString();
        std::string q2 = request.find("two").asString();
        std::string q3 = request.find("three").asString();
        std::string q4 = request.find("four").asString();
        std::string q5 = request.find("five").asString();
        double targets[5];
        targets[0] = stringToDouble(q1);
        targets[1] = stringToDouble(q2);
        targets[2] = stringToDouble(q3);
        targets[3] = stringToDouble(q4);
        targets[4] = stringToDouble(q5);
        std::printf("Going to move%s\n", inMovement.c_str());
        if(simPos) {
            int ax;
            simPos->getAxes(&ax);
            std::vector<int> modes(ax, VOCAB_CM_POSITION);
            simMode->setControlModes(modes.data());
        }
        if((simPos)&&(inMovement == std::string("absolute"))) simPos->positionMove(targets);
        if((simPos)&&(inMovement == std::string("relative"))) simPos->relativeMove(targets);
        if(realPos) {
            int ax;
            realPos->getAxes(&ax);
            std::vector<int> modes(ax, VOCAB_CM_POSITION);
            simMode->setControlModes(modes.data());
        }
        if((realPos)&&(inMovement == std::string("absolute"))) realPos->positionMove(targets);
        if((realPos)&&(inMovement == std::string("relative"))) realPos->relativeMove(targets);
        return response.write(*out);
    } else if (code=="stop.0") {
        if(simPos) simPos->stop();
        if(realPos) realPos->stop();
        return response.write(*out);
    } else if (code=="cartesian") {
        std::string str = readHtml("cartesian.html");
        //TCbegin
        std::string taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<COPTS>", "<script src='cTaskTab.js' type='text/javascript'></script>");
        }
        //TCend
        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="cartesian.1") {
        std::string theAxis = request.find("axis").asString();
        std::string inMovement = request.find("movement").asString();
        std::printf("Going to move axis [%s] towards the [%s].\n", theAxis.c_str(), inMovement.c_str());
        std::vector<double> cartCoords;
        int state;
        if(simCart) simCart->stat(state, cartCoords);
        if(realCart) simCart->stat(state, cartCoords); // REAL OVERWRITES COORDS
        std::printf("At: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);
        if(inMovement == std::string("right")) {
            std::printf("right movement...\n");
            if(theAxis == std::string("px")) cartCoords[0]+= CJOYPAD_RELMOVE_POS;
            if(theAxis == std::string("py")) cartCoords[1]+= CJOYPAD_RELMOVE_POS;
            if(theAxis == std::string("pz")) cartCoords[2]+= CJOYPAD_RELMOVE_POS;
            if(theAxis == std::string("oyP")) cartCoords[3]+= CJOYPAD_RELMOVE_ORI;
            if(theAxis == std::string("ozPP")) cartCoords[4]+= CJOYPAD_RELMOVE_ORI;
        } else if(inMovement == std::string("left")) {
            if(theAxis == std::string("px")) cartCoords[0]-= CJOYPAD_RELMOVE_POS;
            if(theAxis == std::string("py")) cartCoords[1]-= CJOYPAD_RELMOVE_POS;
            if(theAxis == std::string("pz")) cartCoords[2]-= CJOYPAD_RELMOVE_POS;
            if(theAxis == std::string("oyP")) cartCoords[3]-= CJOYPAD_RELMOVE_ORI;
            if(theAxis == std::string("ozPP")) cartCoords[4]-= CJOYPAD_RELMOVE_ORI;
        }
        std::printf("To: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);
        if(simCart) simCart->movl(cartCoords);
        if(realCart) realCart->movl(cartCoords);
        return response.write(*out);
    } else if (code=="cstop.0") {
        if(simCart) simCart->stopControl();
        if(realCart) realPos->stop();
        return response.write(*out);
    } else if (code=="cartesian.2") {
        std::string origin = request.find("origin").asString();
        std::string movement = request.find("movement").asString();
        std::string px = request.find("px").asString();
        std::string py = request.find("py").asString();
        std::string pz = request.find("pz").asString();
        std::string oyP = request.find("oyP").asString();
        std::string ozPP = request.find("ozPP").asString();
        std::vector<double> targets;
        targets.resize(5);
        targets[0] = stringToDouble(px);
        targets[1] = stringToDouble(py);
        targets[2] = stringToDouble(pz);
        targets[3] = stringToDouble(oyP);
        targets[4] = stringToDouble(ozPP);
        std::printf("Going to move%s\n", origin.c_str());
        if(origin == std::string("abs_base")) {
            if(movement == std::string("movj")) {
                if(simCart) simCart->movj(targets);
                if(realCart) realCart->movj(targets);
            }else if(movement == std::string("movl")) {
                if(simCart) simCart->movl(targets);
                if(realCart) realCart->movl(targets);
            }
        } else if (origin == std::string("rel_base")) {
            std::vector<double> cartCoords;
            int state;
            if(simCart) simCart->stat(state, cartCoords);
            if(realCart) simCart->stat(state, cartCoords); // REAL OVERWRITES COORDS
            std::printf("At: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);
            cartCoords[0]+= targets[0];
            cartCoords[1]+= targets[1];
            cartCoords[2]+= targets[2];
            cartCoords[3]+= targets[3];
            cartCoords[4]+= targets[4];
            std::printf("To: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);
            if(movement == std::string("movj")) {
                if(simCart) simCart->movj(cartCoords);
                if(realCart) realCart->movj(cartCoords);
            }else if(movement == std::string("movl")) {
                if(simCart) simCart->movl(cartCoords);
                if(realCart) realCart->movl(cartCoords);
            }
        }
        return response.write(*out);
    } else if (code=="video") {
        std::string camHost = yarp::os::Network::queryName("/ravebot/asibot_tip/img:o").getHost();
        int camPort = yarp::os::Network::queryName("/ravebot/asibot_tip/img:o").getPort();
        std::string camSocket = "http://";
        camSocket += camHost + ":" + intToString(camPort) + "/?action";
        std::printf("\nCam running at: %s\n\n", camSocket.c_str());
        std::string str = readHtml("video.html");
        replaceAll(str, "<SIMCAMIP>", camSocket.c_str());
        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="capture.0") {
        std::vector<double> captureX;
        int state;
        if(simCart) simCart->stat(state, captureX);
        if(realCart) simCart->stat(state, captureX); // REAL OVERWRITES COORDS
        for (int i = 0; i < captureX.size(); i++)
            this->captureX[i] = captureX[i];
        std::printf("At: %f %f %f %f %f\n",captureX[0],captureX[1],captureX[2],captureX[3],captureX[4]);
        std::string coords("x=");
        coords += doubleToString(captureX[0]) + " y=";
        coords += doubleToString(captureX[1]) + " z=";
        coords += doubleToString(captureX[2]) + " rot(y')=";
        coords += doubleToString(captureX[3]) + " rot(z'')=";
        coords += doubleToString(captureX[4]);
        response.addString(coords.c_str());
        std::printf("Writing: %s\n",coords.c_str());
        return response.write(*out);
    } else if (code=="capture.1") {
        std::string pname = request.find("pname").asString();
        std::printf("capture.1 saving capture.0 captures: %f %f %f %f %f\n",captureX[0],captureX[1],captureX[2],captureX[3],captureX[4]);
        std::string captureStr(pname);
        captureStr += " ";
        captureStr += doubleToString(captureX[0]) + " ";
        captureStr += doubleToString(captureX[1]) + " ";
        captureStr += doubleToString(captureX[2]) + " ";
        captureStr += doubleToString(captureX[3]) + " ";
        captureStr += doubleToString(captureX[4]);
        appendToFile("points.ini",captureStr);
        response.addString(pname);
        return response.write(*out);
    } else if (code=="program") {
        std::string str = readHtml("program.html");

        //TCbegin
        std::string taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<POPTS>", "<script src='pTaskTab.js' type='text/javascript'></script>");
        }
        //TCend

        replaceAll(str, "<FNAME>", lastEditName.c_str());

        std::string fileList = fileListCreator();
        replaceAll(str, "<CARGARFICHEROS>", fileList.c_str());

        std::string pointsFile = userPath + "points.ini";
        std::string pointsButtons = pointButtonCreator(pointsFile);
        replaceAll(str, "<POINTS>", pointsButtons.c_str());

        std::string editFile = userPath + lastEditName + ".py";
        std::string contents = readFile(editFile);
        replaceAll(str, "<CENTRALPIECE>", contents.c_str());

        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="create.0") {
        std::string nfile = request.find("nfile").asString();
        lastEditName = nfile;
        nfile += ".py";
        std::string templatePath = rf.findFileByName(std::string("user/") + "template.py");
        std::string str = readFile(templatePath);
        appendToFile(nfile,str);
        std::printf("create.0 %s file.\n",nfile.c_str());
        response.addString(str);
        return response.write(*out);
    } else if (code=="delete.0") {
        std::string dfile = userPath;
        dfile += request.find("dfile").asString();
        response.addString(request.find("dfile").asString());
        dfile += ".py";
        deleteFile(dfile);
        std::printf("delete.0 %s file.\n",dfile.c_str());
        return response.write(*out);
    } else if (code=="delete.1") {
        std::string dfile = userPath;
        dfile += request.find("dfile").asString();
        response.addString(request.find("dfile").asString());
        dfile += ".task";
        deleteFile(dfile);
        std::printf("delete.0 %s file.\n",dfile.c_str());
        return response.write(*out);
    } else if (code=="edit.0") {
        std::string efile = userPath;
        efile += request.find("efile").asString();
        efile += ".py";
        std::printf("edit.0 %s file.\n",efile.c_str());
        std::string str = readFile(efile);
        lastEditName = request.find("efile").asString();
        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="save.0") {
        std::string sfile = request.find("sfile").asString();
        response.addString(request.find("sfile").asString());
        sfile += ".py";
        std::printf("save.0 %s file.\n",sfile.c_str());
        std::string lstr = request.find("lstr").asString().c_str();
        replaceAll(lstr, "<br>", "\n");
        replaceAll(lstr, "<equal>", "=");
        replaceAll(lstr, "<numsign>", "#");
        rewriteFile(sfile,lstr.c_str());
        return response.write(*out);
    } else if (code=="compile.0") {
        std::string program = request.find("program").asString();
        std::printf("compile %s program.\n",program.c_str());
        std::string cmd("python -m py_compile ");
        cmd += userPath + program + ".py";
        cmd += " 2>&1"; // redirect stderr to stdout (see py_compile module)
        std::string res = pipedExec(cmd);
        res == "" ? response.clear() : response.addString(res);
        return response.write(*out);
    } else if (code=="speech") {
        std::string str = readHtml("speech.html");
        //TCbegin
        std::string taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<SOPTS>", "<script src='sTaskTab.js' type='text/javascript'></script>");
        }
        //TCend
        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="speech.0") {
        std::string word = request.find("word").asString();
        appendToFile("words.ini",word);
        response.addString(word.c_str());
        return response.write(*out);
    } else if (code=="launcher") {
        std::string str = readHtml("launcher.html");

        std::string taskList = taskButtonCreator();
        replaceAll(str, "<BOTONESTAREAS>", taskList.c_str());

        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="assigner") {
        std::string str = readHtml("assigner.html");

        //TCbegin
        std::string taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<AOPTS>", "<script src='aTaskTab.js' type='text/javascript'></script>");
        }
        //TCend

        std::string fileList = fileListCreator();
        replaceAll(str, "<CARGARFICHEROS>", fileList.c_str());

        std::string wordsFile = userPath + "words.ini";
        std::string wordOptions = wordOptionCreator(wordsFile);
        replaceAll(str, "<WORDS>", wordOptions.c_str());

        std::string taskList = taskListCreator();
        replaceAll(str, "<CARGARTAREAS>", taskList.c_str());

        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="docking") {
        response.addString(readHtml("docking.html").c_str());
        return response.write(*out);
    } else if (code=="jTaskTab.js") {
        response.addString(readHtml("jTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="cTaskTab.js") {
        response.addString(readHtml("cTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="pTaskTab.js") {
        response.addString(readHtml("pTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="aTaskTab.js") {
        response.addString(readHtml("aTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="sTaskTab.js") {
        response.addString(readHtml("sTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="assigner.0") {
        std::string tname = request.find("tname").asString();
        response.addString(tname);
        tname += ".task";
        std::printf("assign.0 %s file.\n",tname.c_str());
        std::string pfile = request.find("pfile").asString();
        std::string swords = request.find("swords").asString();
        std::string iname = request.find("iname").asString();
        std::string lstr(pfile);
        lstr += "\n";
        lstr += swords + "\n";
        lstr += iname + "\n";
        rewriteFile(tname,lstr.c_str());
        return response.write(*out);
    } else if (code=="launch.0") {
        std::string program = request.find("program").asString();
        std::printf("execute %s program.\n",program.c_str());
        response.addString(program.c_str());
        std::string programPath = userPath + program;
        std::string cmd("python ");
        cmd += programPath;
        cmd += ".py";
        int i=system (cmd.c_str());
        std::printf ("The value returned was: %d.\n",i);
        return response.write(*out);
    }

    std::string prefix = "<html>\n<head>\n<title>YARP web test</title>\n";
    prefix += "<link href=\"style.css\" media=\"screen\" rel=\"stylesheet\" type=\"text/css\" />\n";
    prefix += "</head>\n<body>\n";

    if (code=="push") {
        prefix += "<h1>Counter count</h1>\n";
        prefix += "<div>(<a href='/test'>back</a>)</div>\n";

        response.addString(prefix);
        response.addString("stream");
        response.addInt(1);
        return response.write(*out);
    }

    std::string postfix = "</body>\n</html>";

    std::string txt = prefix;
    txt += std::string("<h1>") + code + "</h1>\n";
    txt += "<div>Is this working for you? <a href='/yes'>yes</a> <a href='/no'>no</a></div>\n";
    if (!request.check("day")) {
        txt += "<div>By the way, what day is it?</div>\n<form><input type='text' id='day' name='day' value='Sunday' /><input type='submit' value='tell me' /></form>\n";
    } else {
        txt += std::string("<div>So today is ") + request.find("day").asString() + ", is it? Hmm. I don't think I'm going to bother remembering that.</div>\n";
    }
    txt += "<div><a href='/push'>How many counter counts?</a> (streaming example)</div>\n";
    txt += postfix;

    response.addString(txt);
    return response.write(*out);
}

