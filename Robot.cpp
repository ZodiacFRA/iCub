#include "Robot.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


Robot::Robot()
{
	_organs.emplace("head",
					new OrganController("/icubSim/head"));
	// _organs.emplace("left_arm",
	// 				new OrganController("/icubSim/left_arm"));
}

Robot::~Robot()
{
	// if (_image)
	// 	delete _image;
	// if (_faces)
	// 	delete _faces;
	// if (_objects)
	// 	delete _objects;
	// if (_vision)
	// 	delete _vision;
	// if (_recognizer)
	// 	delete recognizer;
	// if (_logic)
	// 	delete _logic;
	for (auto it : _organs) {
		if (it.second)
			delete it.second;
	}
}

int Robot::init()
{
	if (!_vision.init()) {
		printf("%sVision Controller init failed%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	}
	if (!_recognizer.init()) {
		printf("%Recognition Controller init failed%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	}
	for (auto it : _organs) {
		if (!it.second->init())
			printf("%s%s Organ Controller init failed%s\n", it.first, COLOR_RED, COLOR_RESET);
			return FAILURE;
	}
	return SUCCESS;
}

// Get robot's view, search target and look at it if found
int Robot::launch()
{
	while (1) {
		_image = NULL;
		this->_vision.getRobotView(_image);

		_faces.clear();
		_objects.clear();
		this->_recognizer.recognize(_image, _faces, _objects);

		resetMoves();
		// _organs is given to get all available interfaces
		this->_logic.think(_faces, _objects, _moves, _organs);

		handleMoves()
}

void Robot::resetMoves()
{
	for (auto it : _moves) {
		it.second[0] = 0;
		it.second[1] = 0;
	}
}

void Robot::handleMoves()
{
	// send each move category to its Controller
}
