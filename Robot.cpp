#include "Robot.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
// using namespace cv;


Robot::Robot()
{
	_organs = new OrgansController();
}

Robot::~Robot()
{
	// _image is already deleted by the destructor of _imagePort which is
	// called by the default destructor of _visionController
	if (_organs)
		delete _organs;
	// Yarp still leaks 32 bytes of memory, cannot be fixed from here
}

int Robot::init()
{
	if (!_vision.init()) {
		printf("%sVision Controller init failed%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	} else if (!_recognizer.init()) {
		printf("%Recognition Controller init failed%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	} else if (!_logic.init()) {
		printf("%Logic Controller init failed%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	} else if (!_organs->init()) {
		// Error message is printed in class to get the interface failure name
		return FAILURE;
	} else {
		return SUCCESS;
	}

	// output video stream to screen
	if (!_imagePortOut.open("/videoStream/out")) {
		printf("%sCould not init video out stream%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	}
	Network::connect("/videoStream/out", "/view/left");
}

// Get robot's view, search target and look at it if found
int Robot::launch()
{
	std::vector<std::string> interfaces;
	_organs->getInterfaces(interfaces);
	while (1) {
		_image = NULL;
		this->_vision.getRobotView(&_image);

		_faces.clear();
		_objects.clear();
		this->_recognizer.recognize(_image, _faces, _objects);

		_moves.clear();
		// _organs is given all available interfaces
		this->_logic.think(_faces, _objects, _moves, interfaces);
		this->_organs->move(_moves);

		filterImage(_image);
	}
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

int Robot::filterImage(ImageOf<PixelRgb> *imageYarp)
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

  cv::GaussianBlur( image, image_gray, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );

  /// Convert it to gray
  cv::cvtColor( image_gray, image_gray, CV_BGR2GRAY );

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
