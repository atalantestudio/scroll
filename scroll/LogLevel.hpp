// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#ifdef ERROR
	// NOTE: Avoid confict with macro defined in Windows.h.
	#undef ERROR
#endif

namespace USER_NAMESPACE {
	enum class LogLevel : uint8 {
		// Enable all logs (alias for TRACE).
		ALL,

		// Enable logs with a level greater than or equal to TRACE.
		TRACE,

		// Enable logs with a level greater than or equal to DEBUG.
		DEBUG,

		// Enable logs with a level greater than or equal to INFO.
		INFO,

		// Enable logs with a level greater than or equal to WARNING.
		WARNING,

		// Enable logs with a level greater than or equal to ERROR.
		ERROR,

		// Disable all logs (alias for ERROR).
		NONE,
	};
}