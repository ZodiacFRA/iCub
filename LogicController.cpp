#include "LogicController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


Vector getRandomVector(int jntsNbr, int max, int start, int end)
{
	Vector tmp;
	tmp.resize(jntsNbr);
	for (int i = 0 ; i < jntsNbr ; i++)
		tmp[i] = (i > start && i < end) ? rand() % max - (max / 2) : 0;
	return tmp;
}


int LogicController::think(std::vector<Vector> &faces,
	std::vector<Vector> &objects, std::map<std::string, movStruct> &moves,
	std::vector<std::string> &interfaces)
{
	Vector visualTarget;  // Only target position
	visualTarget.resize(3);
	Vector moveVector;
	moveVector.resize(16);
	moves["/icubSim/left_arm"] = movStruct(getRandomVector(16, 500, 5, 15),
										5.0, 100.0);
	moves["/icubSim/right_arm"] = movStruct(getRandomVector(16, 500, 5, 15),
										5.0, 100.0);

	for (int i = 0 ; i < 6 ; i++)  // Reset the vector
		moveVector[i] = 0;
	if (faces.size() > 0) {  // target faces in priority
		moves["/icubSim/right_arm"] = _movementsDict["hello"];
		if (faces.size() == 1)
			visualTarget = faces[0];
		else if (faces.size() > 1)  // Get the biggest (nearest) face
			getNearestFace(faces, visualTarget);
	} else if (objects.size() > 0) {
		visualTarget = objects[0];  // Multiple objects handling not implemented
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
	moves["/icubSim/head"] = movStruct(moveVector, 22.0, 200.0);
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
	_movementsDict["reset"] = movStruct(tmp, 20.0, 200.0);
	tmp[0] = -94.5;
	tmp[1] = 45;
	tmp[2] = -3.5;
	tmp[3] = 57;
	tmp[4] = 73.8;
	tmp[5] = -31.5;
	tmp[6] = -9;
	tmp[7] = 46.8;
	tmp[8] = 32.0;
	_movementsDict["hello"] = movStruct(tmp, 30.0, 200.0);
	return SUCCESS;
}
