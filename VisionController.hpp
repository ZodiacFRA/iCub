#ifndef VISION_CONTROLLER
#define VISION_CONTROLLER

#include "common.hpp"
// #include "opencv2/imgproc/imgproc.hpp"
// #include "opencv2/highgui/highgui.hpp"
#include "/usr/include/opencv2/highgui/highgui.hpp"
#include "/usr/include/opencv2/imgproc/imgproc.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

class VisionController {
private:
	bool _receiveFlag;  // only used to reduce console I/O
	BufferedPort<ImageOf<PixelRgb> > _imagePort;  // image input port
	int filterImage(ImageOf<PixelRgb> **);

public:
	VisionController();
	int init();
	int getRobotView(ImageOf<PixelRgb> **);
};

#endif
