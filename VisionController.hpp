#ifndef VISION_CONTROLLER
#define VISION_CONTROLLER

#include "common.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

class VisionController {
private:
	BufferedPort<ImageOf<PixelRgb> > _imagePort;  // image input port
	bool _receiveFlag;  // only used to reduce console I/O

public:
	int init();
	int getRobotView();

};

#endif
