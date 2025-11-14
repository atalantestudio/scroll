#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

#ifndef USER_NAMESPACE
	#define USER_NAMESPACE scroll
#endif

#include "Base/Base/Base.hpp"

#if OPERATING_SYSTEM == OPERATING_SYSTEM_LINUX
	#include <sys/ioctl.h>
	#include <unistd.h>
#endif

#include "str8.hpp"