#include "OrgansController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


OrgansController::OrgansController(std::vector<std::string> &interfaces)
{
	for (auto it : _interfaces)
		_interfaces.push_back(it);
}

OrgansController::~OrgansController()
{
	for (auto it : _organs) {
		if (it.second)
			delete it.second;
	}
}

int OrgansController::init()
{
	for (auto it : _interfaces)
		_organs.emplace(it, new OrganController(it));
	for (auto it : _organs) {
		if (!it.second->init())
			printf("%s%s Organ Controller init failed%s\n", it.first.c_str(), COLOR_RED, COLOR_RESET);
			return FAILURE;
	}
	return SUCCESS;
}

std::vector<std::string> &OrgansController::getInterfaces()
{
	return _interfaces;
}

int OrgansController::move(std::map<std::string, Vector> &moves)
{
	return SUCCESS;
}
