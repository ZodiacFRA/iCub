#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "common.hpp"

#include "VisionController.hpp"
#include "RecognitionController.hpp"
#include "LogicController.hpp"
#include "OrgansController.hpp"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;


class Robot {
private:
	Network _yarp;  // Yarp instance
	ImageOf<PixelRgb> *_image;  // image storage, Vision->Attention
	std::vector<Vector> _faces;  // Detected faces positions
	std::vector<Vector> _objects;  // Detected objects positions
	std::map<std::string, movStruct> _moves;  // Moves to be sent

	VisionController _vision;  // Gets robot's vision
	RecognitionController _recognizer;  // Recognition driver
	LogicController _logic;
	OrgansController *_organs;  // Motors drivers

public:
	Robot();
	~Robot();
	int init();
	int launch();
};


#endif
