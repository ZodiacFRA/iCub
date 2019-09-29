#include "OrganController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;



OrganController::OrganController(std::string &remoteTarget,
	double speed, double acceleration)
	: _remoteTarget(remoteTarget), _refSpeed(speed),
		_refAcceleration(acceleration)
{}


OrganController::~OrganController()
{
	_driver->close();
	delete _driver;
}


int OrganController::move(Vector &moveVector)
{
	// reset all move points (n = joints nmbr)
	if (moveVector.size() != _jntsNbr)
		printf("Invalid move command");
	for (int i = 0; i < moveVector.size(); i++) {
		_setPoints[i] = moveVector[i];
	}
	_pos->positionMove(_setPoints.data());
	return SUCCESS;
}


int OrganController::init()
{
	// create the options struct needed by the _driver constructor
	Property options;
	options.put("device", "remote_controlboard");
	options.put("local", "/test/client" + _remoteTarget);
	options.put("remote", _remoteTarget);

	_driver = new PolyDriver(options);
	if (!_driver->isValid()) {
		printf("%sCannot connect to %s%s\n",
			COLOR_RED,
			_remoteTarget.c_str(),
			COLOR_RESET
		);
		return FAILURE;
	}

	_driver->view(_pos);
	_driver->view(_vel);
	_driver->view(_enc);
	// Check if the driver failed to set the values
	if (_pos == NULL || _vel == NULL || _enc == NULL) {
		printf("%sCannot get %s's interface%s\n",
			COLOR_RED,
			_remoteTarget.c_str(),
			COLOR_RESET
		);
		return FAILURE;
	}
	_pos->getAxes(&_jntsNbr);
	_setPoints.resize(_jntsNbr);

	Vector tmp;
	tmp.resize(_jntsNbr);
	for (int i = 0; i < _jntsNbr; i++)
		tmp[i] = _refSpeed;
	_pos->setRefSpeeds(tmp.data());

    for (int i = 0; i < _jntsNbr; i++)
        tmp[i] = _refAcceleration;
    _pos->setRefAccelerations(tmp.data());

	return SUCCESS;
}
