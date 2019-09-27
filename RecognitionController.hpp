#ifndef RECOGNITION_CONTROLLER_HPP
#define RECOGNITION_CONTROLLER_HPP

#include "common.hpp"

#include "/usr/include/opencv2/objdetect/objdetect.hpp"
#include "/usr/include/opencv2/highgui/highgui.hpp"
#include "/usr/include/opencv2/imgproc/imgproc.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


class RecognitionController {
private:
	Vector _oldTargetPos;
	cv::CascadeClassifier _cascade;

public:
	RecognitionController();
	int init();
	int recognize(ImageOf<PixelRgb> *, std::vector<Vector> &, std::vector<Vector> &);
	int getBlueTargetPosition(ImageOf<PixelRgb> *, std::vector<Vector> &);
	int getFacesPositions(cv::Mat &, cv::CascadeClassifier &, std::vector<Vector> &);
};

#endif
