#include "OrgansController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


OrgansController::~OrgansController()
{
	for (auto it : _organs) {
		if (it.second)
			delete it.second;
	}
}

int OrgansController::init()
{
	int count = 0;
	count += initInterface("/icubSim/head",			22.0,	1000.0);
	count += initInterface("/icubSim/left_arm",		100.0,	1000.0);
	count += initInterface("/icubSim/right_arm",	100.0,	1000.0);
	if (count > 0)
		return SUCCESS;
	else
		return FAILURE;
}

int OrgansController::initInterface(std::string interfaceName, double speed, double acceleration)
{
	OrganController *tmp = new OrganController(interfaceName, speed, acceleration);
	if (!tmp->init()) {
		printf("%s%s: connection failed%s\n",
			COLOR_RED, interfaceName.c_str(), COLOR_RESET);
		delete tmp;
		return FAILURE;
	} else {
		printf("%s%s: connnection successful%s\n",
			COLOR_GREEN, interfaceName.c_str(), COLOR_RESET);
		_organs.emplace(interfaceName, tmp);
		return SUCCESS;
	}
}

void OrgansController::getInterfaces(std::vector<std::string> &interfaces)
{
	for (auto it : _organs)
		interfaces.push_back(it.first);
}

int OrgansController::move(std::map<std::string, Vector> &moves)
{
	for (auto interfaceMove : moves)
		_organs[interfaceMove.first]->move(interfaceMove.second);
	return SUCCESS;
}

// Used to remove interfaces which failed to init, so LogicController won't
// use invalid interfaces
int OrgansController::deleteInterface(std::string interface)
{
	auto it = _organs.find(interface);
	if (it->second)
		delete it->second;
	_organs.erase(it);
	return SUCCESS;
}
