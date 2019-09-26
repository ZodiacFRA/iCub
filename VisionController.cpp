#include "VisionController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


VisionController::VisionController()
	: _receiveFlag(false)
{}

int VisionController::init()
{
	// open image input port
	_imagePort.open("/videoStream/in");
	// send robot left eye cam stream to imagePort
	Network::connect("/icubSim/cam/left", "/videoStream/in");
	return SUCCESS;
}

// Read the input image buffer, store it in image
int VisionController::getRobotView(ImageOf<PixelRgb> *image)
{
	image = _imagePort.read();

	if (image == NULL) { // check we actually got something
		if (_receiveFlag) {
			printf("%sNo video stream%s\n", COLOR_YELLOW, COLOR_RESET);
			_receiveFlag = false;
		}
		return FAILURE;
	}
	if (!_receiveFlag) {
		printf("%sReceiving image: %dpx x %dpx %s\n",
			COLOR_GREEN,
			image->width(),
			image->height(),
			COLOR_RESET
		);
		_receiveFlag = true;
	}
	return SUCCESS;
}
