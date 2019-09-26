#include "Robot.hpp"

int main(int argc, char const **argv) {
	Robot robot;
	if (robot.init() == FAILURE) {
		printf("%sRobot initialization failed%s\n", COLOR_RED, COLOR_RESET);
		return FAILURE;
	}
	printf("%sRobot is online%s\n", COLOR_GREEN, COLOR_RESET);
	return robot.launch();
}
