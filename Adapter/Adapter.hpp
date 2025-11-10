#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "Base/Base.hpp"

#if OPERATING_SYSTEM == OPERATING_SYSTEM_LINUX
	#include <sys/ioctl.h>
	#include <unistd.h>
#elif OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS
	#undef FOREGROUND_RED
	#undef FOREGROUND_GREEN
	#undef FOREGROUND_BLUE
	#undef BACKGROUND_RED
	#undef BACKGROUND_GREEN
	#undef BACKGROUND_BLUE
	#undef ERROR
#endif

#include "str8.hpp"

#include "ConsoleLogger.hpp"
#include "FileLogger.hpp"