#include "Robot.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


Robot::Robot()
{
	std::vector<std::string> interfaces;
	interfaces.push_back("/icubSim/head");
	interfaces.push_back("/icubSim/left_arm");
	interfaces.push_back("/icubSim/right_arm");
	_organs = new OrgansController(interfaces);
}

Robot::~Robot()
{
	// _image is already deleted by the destructor of _imagePort which is
	// called by the default destructor of _visionController
	if (_organs)
		delete _organs;
	// Yarp still leaks 32 bytes of memory, cannot be fixed from here
}

int Robot::init()
{
	if (!_vision.init()) {
		printf("%sVision Controller init failed%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	} else if (!_recognizer.init()) {
		printf("%Recognition Controller init failed%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
		// Logic class does not need any init (so far)
	} else if (!_organs->init()) {
		// Error message is printed in class to get the interface failure name
		return FAILURE;
	} else {
		return SUCCESS;
	}
}

// Get robot's view, search target and look at it if found
int Robot::launch()
{
	bool flag = 0;
	std::vector<std::string> interfaces = _organs->getInterfaces();
	while (1) {
		_image = NULL;
		this->_vision.getRobotView(&_image);

		_faces.clear();
		_objects.clear();
		this->_recognizer.recognize(_image, _faces, _objects);

		resetMoves();
		// _organs is given to get all available interfaces
		this->_logic.think(_faces, _objects, _moves, interfaces);
		this->_organs->move(_moves);
		flag = true;
	}
}

void Robot::resetMoves()
{
	for (auto it : _moves) {
		it.second[0] = 0;
		it.second[1] = 0;
	}
}
