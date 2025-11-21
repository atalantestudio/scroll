// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>

#ifndef ATL_MODULE_BASE
	#include "Base/Base/Base.hpp"
#endif

#if ATL_OPERATING_SYSTEM == ATL_OPERATING_SYSTEM_WINDOWS
	#include <Windows.h>

	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif
#endif

namespace scroll {
	using atl::uint8;
	using atl::uint16;
	using atl::uint32;
	using atl::uint64;

	using atl::char8;

	using atl::sequence;
	using atl::view;

	using atl::copy;
}