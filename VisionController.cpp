/*
 * The controller reads the image from the input port. Then applies a linear
 * filter to a copy of this image. The result of this transformation is sent to
 * the output port after.
 *
 * @author Cesar Gutierrez Carrero
 *
 */


#include "VisionController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace cv;


VisionController::VisionController()
: _receiveFlag(false)
{}

	int VisionController::init()
	{
		// Open image input port
		if (!_imagePort.open("/videoStream/in")) {
			printf("%sCould not init video stream%s\n", COLOR_RED, COLOR_RESET);
			return FAILURE;
		}
		// Send robot left eye cam stream to imagePort
		Network::connect("/icubSim/cam/left", "/videoStream/in");

		// Output video stream to separate window
		if (!_imagePortOut.open("/videoStream/out")) {
			printf("%sCould not init video out stream%s\n", COLOR_RED, COLOR_RESET);
			return FAILURE;
		}
		Network::connect("/videoStream/out", "/view/left");

		return SUCCESS;
	}

	// Read the input image buffer, store it in image
	int VisionController::getRobotView(ImageOf<PixelRgb> **image)
	{
		*image = _imagePort.read();

		if (*image == NULL) { // Check we actually got something
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

	ImageOf<PixelRgb> *auxImg = new ImageOf<PixelRgb>();
	auxImg->copy(**image);

	filterImage(auxImg);

	return SUCCESS;
}


// Convert yarp image to opencv data type
Mat ToMat(const ImageOf<PixelRgb>& imageIn)
{
	return Mat((IplImage*)imageIn.getIplImage());
}

// Convert openCV to yarp data type
ImageOf<PixelBgr> ToPixelBgr(const Mat& imageIn)
{
	IplImage image(imageIn);
	ImageOf<PixelBgr> imageOut;
	imageOut.wrapIplImage(&image);
	return imageOut;
}

int VisionController::filterImage(ImageOf<PixelRgb> *imageYarp)
{
	img = ToMat(*imageYarp);
	if (!img.data)
		return FAILURE;
	Mat img, img_gray, grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	int c;

	GaussianBlur( img, img_gray, Size(3,3), 0, 0, BORDER_DEFAULT );
	cvtColor( img_gray, img_gray, CV_BGR2GRAY );  // Convert it to grayscale

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	Sobel(img_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	/// Gradient Y
	Sobel(img_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	// write processed eye-view
	ImageOf<PixelBgr> &camOutObj = _imagePortOut.prepare();
	camOutObj.copy(ToPixelBgr(grad));
	_imagePortOut.write();
	// printf("%sWriting image in /videoStream/out port.%s\n", COLOR_BLUE, COLOR_RESET);

	return SUCCESS;
}
