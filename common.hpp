#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdio.h>
#include <string>
#include <map>
#include <algorithm>

#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Property.h>
#include <yarp/os/all.h>
#include <yarp/os/Time.h>

#include <yarp/sig/Image.h>
#include <yarp/sig/all.h>

#include <yarp/dev/all.h>

#define FAILURE 0
#define SUCCESS 1
#define MOVING_TARGET 2

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

#endif
