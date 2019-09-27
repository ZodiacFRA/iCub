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
		if (_remoteTarget.find("head") != -1) {
			_setPoints[0] = targetPos[1] / 2;  // Pitch
			_setPoints[1] = 0;  // Roll
			_setPoints[2] = -targetPos[0] / 2;  // Yaw
			_setPoints[3] = targetPos[1] / 2;  // Eye pitch
			_setPoints[4] = targetPos[0] / 2;  // Eye yaw
			_setPoints[5] = 0;  // Eye focus distance
		} else if (_remoteTarget.find("arm") != -1) {
			_setPoints[0] = targetPos[0];  // Shoulder Y rot
			_setPoints[1] = targetPos[1];  // Shoulder X rot
			_setPoints[2] = targetPos[0];  // Shoulder Z rot
			_setPoints[3] = targetPos[1];  // Elbow Y rot
			_setPoints[4] = targetPos[0];  // Elbow Z rot ?
			_setPoints[5] = targetPos[1];  // Wrist supination
			_setPoints[6] = targetPos[0];  // Wrist Karate
			_setPoints[7] = targetPos[1];  // Spread fingers
			_setPoints[8] = targetPos[0];  // Thumb go median
			_setPoints[9] = targetPos[1];  // Thumb Forward
			// > 11:  all fingers
		}
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

	Vector tmp;
	tmp.resize(_jntsNbr);
	for (int i = 0; i < _jntsNbr; i++)
		tmp[i] = 22.0;
	_pos->setRefSpeeds(tmp.data());

    for (int i = 0; i < _jntsNbr; i++)
        tmp[i] = 1000.0;
    _pos->setRefAccelerations(tmp.data());

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
