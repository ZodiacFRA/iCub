#include "LogicController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


int LogicController::think(std::vector<Vector> &faces,
	std::vector<Vector> &objects, std::map<std::string, Vector> &moves,
	std::vector<std::string> &interfaces)
{
	Vector target;
	target.resize(3);
	// Check if we have eyes, if not: move something
	if (std::find(interfaces.begin(), interfaces.end(), "/icubSim/head") != interfaces.end()) {
		// target faces in priority
		if (faces.size() > 0) {
			if (faces.size() == 1)
				target = faces[0];
			else if (faces.size() > 1)
				getNearestFace(faces, target);
		} else if (objects.size() > 0) {  // Handle objects
			// Multiple objects handling not implemented yet
			target = objects[0];
		} else {  // No faces nor objects
			target[0] = 320 / 2;
			target[1] = 420 / 2;
			target[2] = 0;
		}
		target[0] -= 320 / 2;
		target[1] = (240 - target[1]) - (240 / 2);
		moves["/icubSim/head"] = target;
	}
	if (std::find(interfaces.begin(), interfaces.end(), "/icubSim/right_arm") != interfaces.end() ||
	std::find(interfaces.begin(), interfaces.end(), "/icubSim/left_arm") != interfaces.end()) {
		// Blind robot idle animation
		target[0] = rand() % 240 - 120;
		target[1] = rand() % 120 - 60;
		target[2] = 1;
		moves["/icubSim/right_arm"] = target;
		moves["/icubSim/left_arm"] = target;
	}
	// No head nor arm interface
	return SUCCESS;
}

int LogicController::getNearestFace(std::vector<Vector> &faces, Vector &targetFace)
{
	int max = 0;
	for (auto face : faces) {
		if (face[2] > max) {
			max = face[2];
			targetFace = face;
		}
	}
	return SUCCESS;
}
