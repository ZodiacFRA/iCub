#ifndef LOGIC_CONTROLLER_HPP
#define LOGIC_CONTROLLER_HPP

#include "common.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


class LogicController {
private:
	std::map<std::string, Vector> _movementsDict;
public:
	int init();
	int think(std::vector<Vector> &, std::vector<Vector> &, std::map<std::string, Vector> &, std::vector<std::string> &);
	int getNearestFace(std::vector<Vector> &, Vector &);
};

#endif

// HEAD
	// _setPoints[0]  // Pitch
	// _setPoints[1]  // Roll
	// _setPoints[2]  // Yaw
	// _setPoints[3]  // Eye pitch
	// _setPoints[4]  // Eye yaw
	// _setPoints[5]  // Eye focus distance
// ARMS
	// _setPoints[0]  // Shoulder Y rot
	// _setPoints[1]  // Shoulder X rot
	// _setPoints[2]  // Shoulder Z rot
	// _setPoints[3]  // Elbow Y rot
	// _setPoints[4]  // Elbow Z rot ?
	// _setPoints[5]  // Wrist supination
	// _setPoints[6]  // Wrist Karate
	// _setPoints[7]  // Spread fingers
	// _setPoints[8] // Thumb go median
	// _setPoints[9]  // Thumb Forward
	// _setPoints[10]  // Thumb Forward
	// _setPoints[11]  // Thumb Forward
	// _setPoints[12]  // Thumb Forward
	// _setPoints[13]  // Thumb Forward
	// _setPoints[14]  // Thumb Forward
	// _setPoints[15]  // Thumb Forward
