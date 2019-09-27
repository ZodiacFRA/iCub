#ifndef LOGIC_CONTROLLER_HPP
#define LOGIC_CONTROLLER_HPP

#include "common.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


class LogicController {
public:
	int think(std::vector<Vector> &, std::vector<Vector> &, std::map<std::string, Vector> &, std::vector<std::string> &);
	int getNearestFace(std::vector<Vector> &, Vector &);
};

#endif
