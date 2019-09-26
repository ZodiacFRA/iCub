#include "RecognitionController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


int RecognitionController::init()
{
	return SUCCESS;
}

int RecognitionController::recognize(ImageOf<PixelRgb> *image,
	std::vector<Vector> &faces, std::vector<Vector> &objects)
{
	getBlueTargetPosition(image, objects);
	return SUCCESS;
}

// Find position of the target in image (will fail with multiple targets)
int RecognitionController::getBlueTargetPosition(ImageOf<PixelRgb> *image, std::vector<Vector> &_objects)
{
	double xMean = 0;
	double yMean = 0;
	int bluePixelsCount = 0;
	Vector tmp;
	tmp.resize(3);
	for (int x = 0; x < image->width(); x++) {
		for (int y = 0; y < image->height(); y++) {
			PixelRgb& pixel = image->pixel(x, y);
			// check if blue level exceeds red and green by a factor of 2
			if (pixel.b > pixel.r * 1.2 + 10 && pixel.b > pixel.g * 1.2 + 10) {
				bluePixelsCount++;
				xMean += x;
				yMean += y;
			}
		}
	}
	if (bluePixelsCount > 0) {  // get the average location of blue pixels
		xMean /= bluePixelsCount;
		yMean /= bluePixelsCount;
	}
	// Check if the target is big enough
	if (bluePixelsCount > (image->width() / 20) * (image->height() / 20)) {
		// If the target moved (without decimals)
		if (int(tmp[0]) != int(xMean) || int(tmp[1]) != int(yMean)) {
			tmp[0] = xMean;
			tmp[1] = yMean;
			tmp[2] = 1;
			printf("%sTarget moving: x=%g\ty=%g%s\n",
				COLOR_GREEN,
				tmp[0],
				tmp[1],
				COLOR_RESET
			);
			_objects.push_back(tmp);
			return MOVING_TARGET;
		}
	}
}
