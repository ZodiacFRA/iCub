#include "RecognitionController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


RecognitionController::RecognitionController()
{
	_oldTargetPos.resize(2);
}

int RecognitionController::init()
{
	// PreDefined trained XML classifiers with facial features
	// Load classifiers from the "opencv/data/haarcascades" directory
	_cascade.load( "/usr/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml");
	return SUCCESS;
}

int RecognitionController::recognize(ImageOf<PixelRgb> *image,
	std::vector<Vector> &faces, std::vector<Vector> &objects)
{
	getTargetPosition(image, objects);

	// Transform yarp image into openCV Mat
	cv::Mat frame((IplImage*)image->getIplImage());
    cv::cvtColor(frame, frame, CV_RGB2BGR);

	getFacesPositions(frame, _cascade, faces);
	return SUCCESS;
}


int RecognitionController::getFacesPositions(cv::Mat &img,
	cv::CascadeClassifier &cascade, std::vector<Vector> &faces)
{
	Vector tmp;
	tmp.resize(3);
    std::vector<cv::Rect> CVfaces;
    cv::Mat gray, smallImg;

    cvtColor(img, gray, cv::COLOR_BGR2GRAY); // Convert to Gray Scale

    // Resize the Grayscale Image
    resize(gray, smallImg, cv::Size(), 1, 1, cv::INTER_LINEAR);
    equalizeHist(smallImg, smallImg);
    // Detect faces of different sizes using cascade classifier
    cascade.detectMultiScale(smallImg, CVfaces, 1.1,2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	// printf("size=%d\n\0", CVfaces.size());
    // Draw circles around the faces
    for (auto r : CVfaces) {
        cv::Point center;
        int radius;
        center.x = cvRound((r.x + r.width*0.5));
        center.y = cvRound((r.y + r.height*0.5));
        radius = cvRound((r.width + r.height)*0.25);
		tmp[0] = center.x;
		tmp[1] = center.y;
		tmp[2] = radius;
		faces.push_back(tmp);
        printf("FACE x= %d, y= %d, radius= %d\n", center.x, center.y, radius);
    }
	return SUCCESS;
}


// Find position of the target in image (will fail with multiple targets)
int RecognitionController::getTargetPosition(ImageOf<PixelRgb> *image, std::vector<Vector> &objects)
{
	double xMean = 0;
	double yMean = 0;
	int pixelsCount = 0;
	Vector tmp;
	tmp.resize(3);

	for (int x = 0; x < image->width(); x++) {
		for (int y = 0; y < image->height(); y++) {
			PixelRgb& pixel = image->pixel(x, y);
			// check if blue level exceeds red and green by a factor of 2
			if (pixel.g > pixel.b * 1.2 + 10 && pixel.g > pixel.r * 1.2 + 10) {
				pixelsCount++;
				xMean += x;
				yMean += y;
			}
		}
	}
	if (pixelsCount > 0) {  // get the average location of blue pixels
		xMean /= pixelsCount;
		yMean /= pixelsCount;
	}
	// Check if the target is big enough
	if (pixelsCount > (image->width() / 20) * (image->height() / 20)) {
		// If the target moved (without decimals)
		tmp[0] = xMean;
		tmp[1] = yMean;
		tmp[2] = 1;
		objects.push_back(tmp);
		if (int(_oldTargetPos[0]) / 10 != int(xMean) / 10 ||
			int(_oldTargetPos[1]) /10 != int(yMean) / 10) {
				printf("%sTarget moving:\t\tx=%g\ty=%g%s\n",
				COLOR_BLUE,
				tmp[0],
				tmp[1],
				COLOR_RESET
			);
			_oldTargetPos[0] = xMean;
			_oldTargetPos[1] = yMean;
			return MOVING_TARGET;
		}
		return SUCCESS;
	}
	return FAILURE;
}
