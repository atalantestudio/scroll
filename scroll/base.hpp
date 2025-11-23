// Copyright 2025 Atalante Studio.
// Distributed under the MIT License.

#pragma once

#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>

#ifndef ATL_MODULE_BASE
	#include "Base/Base/Base.hpp"
#endif

#if ATL_OPERATING_SYSTEM == ATL_OPERATING_SYSTEM_WINDOWS
	#include <Windows.h>

	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 4
	#endif
#endif

namespace scroll {
	using atl::uint8;
	using atl::uint16;
	using atl::uint32;
	using atl::uint64;

	using atl::sint8;
	using atl::sint16;
	using atl::sint32;
	using atl::sint64;

	using atl::char8;

	using atl::float32;
	using atl::float64;

	using atl::sequence;
	using atl::view;

	using atl::copy;
	using atl::pointer_cast;
}