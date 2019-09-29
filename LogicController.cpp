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
		Vector visualTarget;  // Only target position
		visualTarget.resize(3);
		Vector moveVector;  // iCub Head has 6 different joints
		moveVector.resize(6);
		for (int i = 0 ; i < 6 ; i++)  // Reset the vector
			moveVector[i] = 0;
		if (faces.size() > 0) {  // target faces in priority
			if (faces.size() == 1)
				visualTarget = faces[0];
			else if (faces.size() > 1)  // Get the biggest (nearest) face
				getNearestFace(faces, visualTarget);
		} else if (objects.size() > 0) {
			// Multiple objects handling not implemented yet
			visualTarget = objects[0];
		} else {  // No targets, look at the center
			visualTarget[0] = 320 / 2;
			visualTarget[1] = 240 / 2;
			visualTarget[2] = 0;
		}
		// transform target position to eyes positions
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
		moves["/icubSim/right_arm"] = _movementsDict["hello"];//moveVector;
		moves["/icubSim/left_arm"] = _movementsDict["reset"];//moveVector;
	}
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

int LogicController::init()
{
	Vector tmp;
	tmp.resize(16);
	_movementsDict["reset"] = tmp;
	tmp[0] = -94.5;
	tmp[1] = 45;
	tmp[2] = -3.5;
	tmp[3] = 57;
	tmp[4] = 73.8;
	tmp[5] = -31.5;
	tmp[6] = -9;
	tmp[7] = 46.8;
	tmp[8] = 32.0;
	_movementsDict["hello"] = tmp;
	return SUCCESS;
}
