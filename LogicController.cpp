#include "LogicController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


int LogicController::think(std::vector<Vector> &faces,
	std::vector<Vector> &objects, std::map<std::string, Vector> &moves,
	std::vector<std::string> &interfaces)
{
	// Check if we have eyes, if not: move something
	if (std::find(interfaces.begin(), interfaces.end(), "/icubSim/head") != interfaces.end()) {
		Vector visualTarget;
		visualTarget.resize(3);
		Vector moveVector;
		moveVector.resize(6);
		for (int i = 0 ; i < 6 ; i++)
			moveVector[i] = 0;
		// target faces in priority
		if (faces.size() > 0) {
			if (faces.size() == 1)
				visualTarget = faces[0];
			else if (faces.size() > 1)
				getNearestFace(faces, visualTarget);
		} else if (objects.size() > 0) {  // Handle objects
			// Multiple objects handling not implemented yet
			visualTarget = objects[0];
		} else {  // No faces nor objects
			visualTarget[0] = 320 / 2;
			visualTarget[1] = 240 / 2;
			visualTarget[2] = 0;
		}
		double x = visualTarget[0] - (320 / 2);
		double y = -(visualTarget[1] - (240 / 2));
		moveVector[4] = x / 2;
		moveVector[3] = y / 2;
		moveVector[0] = y / 2;
		moveVector[2] = -x / 2;
		moves["/icubSim/head"] = moveVector;
	}
	if (std::find(interfaces.begin(), interfaces.end(), "/icubSim/right_arm") != interfaces.end() ||
	std::find(interfaces.begin(), interfaces.end(), "/icubSim/left_arm") != interfaces.end()) {
		// Blind robot idle animation
		Vector moveVector;
		moveVector.resize(16);
		// moveVector[0] = rand() % 240 - 120;
		// moveVector[1] = rand() % 120 - 60;
		// moveVector[2] = 1;
		// moves["/icubSim/right_arm"] = moveVector;
		// moves["/icubSim/left_arm"] = moveVector;
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
