#ifndef ORGANS_CONTROLLER_HPP
#define ORGANS_CONTROLLER_HPP

#include "common.hpp"
#include "OrganController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


class OrgansController {
private:
	std::map<std::string, OrganController *> _organs;  // Organs drivers

public:
	~OrgansController();
	int init();
	int initInterface(std::string, double, double);
	void getInterfaces(std::vector<std::string> &);
	int move(std::map<std::string, Vector> &);
	int deleteInterface(std::string);
};

#endif
