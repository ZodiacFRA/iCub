#ifndef ORGAN_CONTROLLER_HPP
#define ORGAN_CONTROLLER_HPP

#include "common.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


class OrganController {
private:
	PolyDriver *		_driver;  // motor interface
	IPositionControl *	_pos;
	IVelocityControl *	_vel;
	IEncoders *			_enc;
	int 				_jntsNbr;
	Vector 				_setPoints;
	std::string 		_remoteTarget;
	double				_refSpeed;
	double				_refAcceleration;

public:
	OrganController(std::string &, double, double);
	virtual ~OrganController();

	int init();
	int move(Vector &);
};

#endif
