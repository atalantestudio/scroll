// Copyright 2025 Atalante.
// Licensed under MIT.

namespace scroll {
	inline ConsoleLogger::ConsoleLogger(std::ostream& stream, LogLevel minLogLevel, view<char8> source) :
		Logger(minLogLevel, source)
	{
		#if ATL_OPERATING_SYSTEM == ATL_OPERATING_SYSTEM_WINDOWS
			setConsoleMode(STD_OUTPUT_HANDLE);
		#endif
	}

	inline std::ostream& ConsoleLogger::getOutputStream() {
		return *stream;
	}

	inline void ConsoleLogger::setOutputStream(std::ostream& stream) {
		this->stream = &stream;
	}

	template<typename Argument>
	ConsoleLogger& ConsoleLogger::operator<<(Argument&& argument) {
		if (writingEscapeCodes) {
			writingEscapeCodes = false;

			stream->write("m", 1);
		}

		*stream << argument;

		return *this;
	}

	template<>
	inline ConsoleLogger& ConsoleLogger::operator<<(ConsoleEscapeCode&& argument) {
		if (!writingEscapeCodes) {
			writingEscapeCodes = true;

			stream->write("\033[", 3);
		}
		else {
			stream->write(";", 1);
		}

		*stream << static_cast<uint16>(argument);

		return *this;
	}

	inline ConsoleLogger& ConsoleLogger::padLeft(uint64 padding) {
		*stream << std::right << std::setw(padding);

		return *this;
	}

	inline ConsoleLogger& ConsoleLogger::padRight(uint64 padding) {
		*stream << std::left << std::setw(padding);

		return *this;
	}

	template<typename Argument>
	inline void ConsoleLogger::trace(view<char8> file, uint64 line, Argument&& argument) {
		writeTraceLog(file, line, argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void ConsoleLogger::trace(view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		writeTraceLog(file, line, pattern, std::forward<Argument>(arguments)...);
	}

	template<typename Argument>
	inline void ConsoleLogger::debug(Argument&& argument) {
		writeDebugLog(argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void ConsoleLogger::debug(view<char8> pattern, Argument&&... arguments) {
		writeDebugLog(pattern, std::forward<Argument>(arguments)...);
	}

	template<typename Argument>
	inline void ConsoleLogger::info(Argument&& argument) {
		writeInfoLog(argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void ConsoleLogger::info(view<char8> pattern, Argument&&... arguments) {
		writeInfoLog(pattern, std::forward<Argument>(arguments)...);
	}

	template<typename Argument>
	inline void ConsoleLogger::warning(Argument&& argument) {
		writeWarningLog(argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void ConsoleLogger::warning(view<char8> pattern, Argument&&... arguments) {
		writeWarningLog(pattern, std::forward<Argument>(arguments)...);
	}

	template<typename Argument>
	inline void ConsoleLogger::error(view<char8> function, view<char8> file, uint64 line, Argument&& argument) {
		writeErrorLog(function, file, line, argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void ConsoleLogger::error(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		writeErrorLog(function, file, line, pattern, std::forward<Argument>(arguments)...);
	}

	template<typename... Argument>
	inline uint64 ConsoleLogger::writeConsoleLog(view<char8> level, uint64 levelSize, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_BUFFER_TEMPLATE = "\033[2m[00:00:00.000000]\033[22m ";

		return writeLog(5, LOG_BUFFER_TEMPLATE, level, levelSize, pattern, std::forward<Argument>(arguments)...);
	}

	template<typename... Argument>
	inline void ConsoleLogger::writeTraceLog(view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "\033[107;30m TRACE \033[49;39m ";

		if (LogLevel::TRACE < minLogLevel || redirected) {
			return;
		}

		uint64 offset = writeConsoleLog(LOG_LEVEL, 8, pattern, std::forward<Argument>(arguments)...);

		write(" (at ", offset);
		write(file, offset);
		write(':', offset);

		const std::string formattedLine = toString(line);

		write(&formattedLine[0], offset);
		write(')', offset);
		write('\n', offset);

		flushLog(offset);
	}

	template<typename... Argument>
	inline void ConsoleLogger::writeDebugLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "\033[47;30m DEBUG \033[49;39m ";

		if (LogLevel::DEBUG < minLogLevel || redirected) {
			return;
		}

		uint64 offset = writeConsoleLog(LOG_LEVEL, 8, pattern, std::forward<Argument>(arguments)...);

		write('\n', offset);

		flushLog(offset);
	}

	template<typename... Argument>
	inline void ConsoleLogger::writeInfoLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "\033[104;97m INFO \033[49;39m ";

		if (LogLevel::INFO < minLogLevel || redirected) {
			return;
		}

		uint64 offset = writeConsoleLog(LOG_LEVEL, 7, pattern, std::forward<Argument>(arguments)...);

		write('\n', offset);

		flushLog(offset);
	}

	template<typename... Argument>
	inline void ConsoleLogger::writeWarningLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "\033[103;30m WARNING \033[49;39m \033[93m";

		if (LogLevel::WARNING < minLogLevel || redirected) {
			return;
		}

		uint64 offset = writeConsoleLog(LOG_LEVEL, 9, pattern, std::forward<Argument>(arguments)...);

		write("\033[39m\n", offset);

		flushLog(offset);
	}

	template<typename... Argument>
	inline void ConsoleLogger::writeErrorLog(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "\033[101;97m ERROR \033[49;39m \033[91m";

		if (LogLevel::ERROR < minLogLevel || redirected) {
			return;
		}

		uint64 offset = writeConsoleLog(LOG_LEVEL, 8, pattern, std::forward<Argument>(arguments)...);

		write(" (in ", offset);
		write(function, offset);
		write(", at ", offset);
		write(file, offset);
		write(':', offset);

		const std::string formattedLine = toString(line);

		write(&formattedLine[0], offset);
		write(")\033[39m\n", offset);

		flushLog(offset);
	}

	inline void ConsoleLogger::flushLog(uint64 size) const {
		ATL_ASSERT(size <= MAX_LOG_BUFFER_SIZE);

		std::clog.write(&logBuffer[0], size);
	}
}