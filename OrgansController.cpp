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
			printf("%s%s Organ Controller init failed%s\n", it.first, COLOR_RED, COLOR_RESET);
			return FAILURE;
	}
	return SUCCESS;
}
