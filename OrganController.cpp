#include "OrganController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


int OrganController::move(Vector &targetPos)
{
	// reset all move points (n = joints nmbr)
	for (int i = 0; i < _jntsNbr; i++)
		_setPoints[i] = 0;

	if (targetPos[2] > 0.5) {  	// Move only if enough confident
		printf("%sSending pos: x=%g\ty=%g to %s%s\n",
			COLOR_BLUE,
			targetPos[0],
			targetPos[1],
			_remoteTarget.c_str(),
			COLOR_RESET);
		_setPoints[3] = targetPos[1];
		_setPoints[4] = targetPos[0];
	} else {  // Reset eyes positions
		_setPoints[3] = 0;
		_setPoints[4] = 0;
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
	return SUCCESS;
}


OrganController::OrganController(std::string &remoteTarget)
	: _remoteTarget(remoteTarget)
{}


OrganController::~OrganController()
{
	_driver->close();
	delete _driver;
}
