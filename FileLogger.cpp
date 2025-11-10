#pragma once

#include "Adapter/Adapter.hpp"

namespace ProjectA {
	FileLogger::FileLogger(std::ofstream& stream, LogLevel minLogLevel) :
		Logger(minLogLevel),
		stream(stream)
	{
		ASSERT(stream.is_open());
	}

	template<typename Argument>
	void FileLogger::trace(const str8& file, uint32 line, Argument&& argument) {
		writeLog(stream, LogLevel::TRACE, format(std::forward<Argument>(argument)), max<uint64>());
		writeIndented(stream, format("at []:[]", file, line), getLogIndentation(), true, max<uint64>());
	}

	template<typename... Argument>
	void FileLogger::trace(const str8& file, uint32 line, const str8& pattern, Argument&&... arguments) {
		writeLog(stream, LogLevel::TRACE, format(pattern, std::forward<Argument>(arguments)...), max<uint64>());
		writeIndented(stream, format("at []:[]", file, line), getLogIndentation(), true, max<uint64>());
	}

	template<typename Argument>
	void FileLogger::debug(Argument&& argument) {
		writeLog(stream, LogLevel::DEBUG, format(std::forward<Argument>(argument)), max<uint64>());
	}

	template<typename... Argument>
	void FileLogger::debug(const str8& pattern, Argument&&... arguments) {
		writeLog(stream, LogLevel::DEBUG, format(pattern, std::forward<Argument>(arguments)...), max<uint64>());
	}

	template<typename Argument>
	void FileLogger::info(Argument&& argument) {
		writeLog(stream, LogLevel::INFO, format(std::forward<Argument>(argument)), max<uint64>());
	}

	template<typename... Argument>
	void FileLogger::info(const str8& pattern, Argument&&... arguments) {
		writeLog(stream, LogLevel::INFO, format(pattern, std::forward<Argument>(arguments)...), max<uint64>());
	}

	template<typename Argument>
	void FileLogger::warning(Argument&& argument) {
		writeLog(stream, LogLevel::WARNING, format(std::forward<Argument>(argument)), max<uint64>());
	}

	template<typename... Argument>
	void FileLogger::warning(const str8& pattern, Argument&&... arguments) {
		writeLog(stream, LogLevel::WARNING, format(pattern, std::forward<Argument>(arguments)...), max<uint64>());
	}

	template<typename Argument>
	void FileLogger::error(const str8& function, const str8& file, uint64 line, Argument&& argument) {
		writeLog(stream, LogLevel::ERROR, format(std::forward<Argument>(argument)), max<uint64>());
		writeIndented(stream, format("at [] ([]:[])", function, file, line), getLogIndentation(), true, max<uint64>());
	}

	template<typename... Argument>
	void FileLogger::error(const str8& function, const str8& file, uint64 line, const str8& pattern, Argument&&... arguments) {
		writeLog(stream, LogLevel::ERROR, format(pattern, std::forward<Argument>(arguments)...), max<uint64>());
		writeIndented(stream, format("at [] ([]:[])", function, file, line), getLogIndentation(), true, max<uint64>());
	}
}