#ifndef ORGANS_CONTROLLER_HPP
#define ORGANS_CONTROLLER_HPP

#include "common.hpp"
#include "OrganController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


class OrgansController {
private:
	std::vector<std::string> _interfaces;
	std::map<std::string, OrganController *> _organs;  // Organs drivers

public:
	OrgansController(std::vector<std::string> &);
	~OrgansController();
	int init();
	std::vector<std::string> &getInterfaces();
	int move(std::map<std::string, Vector> &);
	int deleteInterface(std::string);
};

#endif
