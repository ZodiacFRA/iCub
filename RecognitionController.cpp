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

	// output video stream to screen
	if (!_imagePortOut.open("/videoStream/out")) {
		printf("%sCould not init video out stream%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	}
	Network::connect("/videoStream/out", "/view/left");

	return SUCCESS;
}

int RecognitionController::recognize(ImageOf<PixelRgb> *image,
	std::vector<Vector> &faces, std::vector<Vector> &objects)
{
	getTargetPosition(image, objects);

	// Transform yarp image into openCV Mat
	cv::Mat frame((IplImage*)image->getIplImage());

	filterImage(image);

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


/* ------------------------------------------------------- */

// convert yarp image to opencv data type
cv::Mat ToMat(const ImageOf<PixelRgb>& imageIn)
{
	return cv::Mat((IplImage*)imageIn.getIplImage());
}

// convert opencv to yarp data type
ImageOf<PixelBgr> ToPixelBgr(const cv::Mat& imageIn)
{
	IplImage image(imageIn);
	ImageOf<PixelBgr> imageOut;
	imageOut.wrapIplImage(&image);
	return imageOut;
}

int RecognitionController::filterImage(ImageOf<PixelRgb> *imageYarp)
{

  cv::Mat image, image_gray, grad;
  // char* window_name = "Sobel Demo - Simple Edge Detector";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  int c;

	// image = cvarrToMat(static_cast<IplImage*>((**imageYarp).getIplImage()));
	image = ToMat(*imageYarp);

  if (!image.data) { return -1; }

  cv::GaussianBlur( *image, image_gray, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );

  /// Convert it to gray
  cv::cvtColor( image_gray, image_gray, cv::CV_BGR2GRAY );

  /// Create window
  // namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /// Generate grad_x and grad_y
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  cv::Sobel(image_gray, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
  cv::convertScaleAbs(grad_x, abs_grad_x);

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  cv::Sobel(image_gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
  cv::convertScaleAbs(grad_y, abs_grad_y);

  /// Total Gradient (approximate)
  cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

  // imshow( window_name, grad );
	//**imageYarp = ToPixelRgb(grad);

	// write processed eye-view
  ImageOf<PixelBgr> &camOutObj = _imagePortOut.prepare();
  camOutObj.copy(ToPixelBgr(grad));
  _imagePortOut.write();
	printf("Writing image in /videoStream/out port.\n", COLOR_BLUE);

  cv::waitKey(0);

  return 0;
}
