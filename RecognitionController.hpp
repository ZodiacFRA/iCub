#ifndef RECOGNITION_CONTROLLER_HPP
#define RECOGNITION_CONTROLLER_HPP

#include "common.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


class RecognitionController {
private:
	/* data */

public:
	// RecognitionController (arguments);
	// virtual ~RecognitionController ();
	int init();
	int recognize(ImageOf<PixelRgb> *, std::vector<Vector> &, std::vector<Vector> &);
	int getBlueTargetPosition(ImageOf<PixelRgb> *, std::vector<Vector> &);
};

#endif
