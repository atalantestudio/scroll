#pragma once

// NOTE: Avoid confict with ERROR defined in Windows.h.
#undef ERROR

namespace ProjectA {
	enum class LogLevel : uint8 {
		// Enables all logs.
		ALL,

		// Enables logs with a level inferior than or equal to TRACE.
		TRACE,

		// Enables logs with a level inferior than or equal to DEBUG.
		DEBUG,

		// Enables logs with a level inferior than or equal to INFO.
		INFO,

		// Enables logs with a level inferior than or equal to WARNING.
		WARNING,

		// Enables logs with a level inferior than or equal to ERROR.
		ERROR,

		// Disables all logs.
		NONE,
	};
}