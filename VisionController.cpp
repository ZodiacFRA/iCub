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
	if (!_imagePort.open("/videoStream/in")) {
		printf("%sCould not init video stream%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	}
	// send robot left eye cam stream to imagePort
	Network::connect("/icubSim/cam/left", "/videoStream/in");
	return SUCCESS;
}

// Read the input image buffer, store it in image
int VisionController::getRobotView(ImageOf<PixelRgb> **image)
{
	*image = _imagePort.read();

	if (*image == NULL) { // check we actually got something
		printf("%sNo video stream%s\n", COLOR_YELLOW, COLOR_RESET);
		_receiveFlag = false;
		return FAILURE;
	}
	if (!_receiveFlag) {
		printf("%sReceiving image:\t%dpx\t\t%dpx %s\n",
			COLOR_BLUE,
			(*image)->width(),
			(*image)->height(),
			COLOR_RESET
		);
		_receiveFlag = true;
	}
	return SUCCESS;
}
