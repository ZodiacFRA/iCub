#ifndef VISION_CONTROLLER
#define VISION_CONTROLLER

#include "common.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

class VisionController {
private:
	bool _receiveFlag;  // only used to reduce console I/O
	BufferedPort<ImageOf<PixelRgb> > _imagePort;  // image input port

public:
	VisionController();
	int init();
	int getRobotView(ImageOf<PixelRgb> **);
};

#endif
