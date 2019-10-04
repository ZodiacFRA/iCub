#include "VisionController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
// using namespace yarp::cv;
using namespace cv;


VisionController::VisionController()
	: _receiveFlag(false)
{}

int VisionController::init()
{
	// open image input port
	if (!_imagePort.open("/videoStream/in")) {
		printf("%sCould not init video stream%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	}
	// send robot left eye cam stream to imagePort
	Network::connect("/icubSim/cam/left", "/videoStream/in");
	return SUCCESS;
}

// Read the input image buffer, store it in image
int VisionController::getRobotView(ImageOf<PixelRgb> **image)
{
	*image = _imagePort.read();

	if (*image == NULL) { // check we actually got something
		printf("%sNo video stream%s\n", COLOR_YELLOW, COLOR_RESET);
		_receiveFlag = false;
		return FAILURE;
	}
	if (!_receiveFlag) {
		printf("%sReceiving image:\t%dpx\t\t%dpx %s\n",
			COLOR_BLUE,
			(*image)->width(),
			(*image)->height(),
			COLOR_RESET
		);
		_receiveFlag = true;
	}

	filterImage(image);

	return SUCCESS;
}

// convert yarp image to opencv data type
Mat ToMat(const ImageOf<PixelRgb>& imageIn)
{
	return Mat((IplImage*)imageIn.getIplImage());
}

// convert opencv to yarp data type
ImageOf<PixelRgb> ToPixelRgb(const Mat& imageIn)
{
	IplImage image(imageIn);
	ImageOf<PixelRgb> imageOut;
	imageOut.wrapIplImage(&image);
	return imageOut;
}

int VisionController::filterImage(ImageOf<PixelRgb> **imageYarp)
{

  Mat image, image_gray, grad;
  // char* window_name = "Sobel Demo - Simple Edge Detector";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  int c;

	// image = cvarrToMat(static_cast<IplImage*>((**imageYarp).getIplImage()));
	image = ToMat(**imageYarp);

  if (!image.data) { return -1; }

  GaussianBlur( image, image, Size(3,3), 0, 0, BORDER_DEFAULT );

  /// Convert it to gray
  cvtColor( image, image_gray, CV_BGR2GRAY );

  /// Create window
  // namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel(image_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs(grad_x, abs_grad_x);

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel(image_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs(grad_y, abs_grad_y);

  /// Total Gradient (approximate)
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

  // imshow( window_name, grad );
	**imageYarp = ToPixelRgb(grad);

	// output video stream to screen
  BufferedPort<ImageOf<PixelBgr> > imagePortOut;
  imagePortOut.open("/videoStream/out");
  Network::connect("/videoStream/out", "/icubSim/cam/left");

	// write processed eye-view
  ImageOf<PixelBgr> &camOutObj = imagePortOut.prepare();
  camOutObj.copy(ToPixelRgb(grad));
  imagePortOut.write();

  waitKey(0);

  return 0;
}
