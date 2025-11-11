#include "pch.hpp"

#include "Adapter/FileLogger.hpp"

namespace ProjectA {
	FileLogger::FileLogger(std::ofstream& stream, LogLevel minLogLevel, const str8& source) :
		Logger(minLogLevel, source),
		stream(stream)
	{
		ASSERT(stream.is_open());
	}
}