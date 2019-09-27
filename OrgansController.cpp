#include "OrgansController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


OrgansController::OrgansController(std::vector<std::string> &interfaces)
{
	for (auto it : interfaces)
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
	int count = 0;
	std::vector<std::string> finalInterfaces;
	for (auto interfaceName : _interfaces) {
		OrganController *tmp = new OrganController(interfaceName);
		if (!tmp->init()) {
			printf("%s%s: link failed%s\n",
				COLOR_RED, interfaceName.c_str(), COLOR_RESET);
			delete tmp;
		} else {
			printf("%s%s: link operational%s\n",
				COLOR_GREEN, interfaceName.c_str(), COLOR_RESET);
			finalInterfaces.push_back(interfaceName);
			_organs.emplace(interfaceName, tmp);
			count++;
		}
	}
	if (count > 0)
		return SUCCESS;
	else
		return FAILURE;
}

std::vector<std::string> &OrgansController::getInterfaces()
{
	return _interfaces;
}

int OrgansController::move(std::map<std::string, Vector> &moves)
{
	return SUCCESS;
}

// Used to remove interfaces which failed to init, so LogicController won't
// use invalid interfaces
int OrgansController::deleteInterface(std::string interface)
{
	int pos = 0;
    for(auto it = _interfaces.begin(); it != _interfaces.end(); it++) {
		if (_interfaces[pos] == interface)
			_interfaces.erase(it);
		pos++;
	}
	return SUCCESS;
}
