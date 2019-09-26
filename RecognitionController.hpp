#ifndef RECOGNITION_CONTROLLER_HPP
#define RECOGNITION_CONTROLLER_HPP

#include "common.hpp"

class RecognitionController {
private:
	/* data */

public:
	// RecognitionController (arguments);
	// virtual ~RecognitionController ();
	int init();
	int recognize(ImageOf<PixelRgb> &, std::vector<Vector> &, std::vector<Vector> &)
	int getBlueTargetPosition(std::vector<Vector> &);
};

#endif
