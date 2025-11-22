// Copyright 2025 Atalante.
// Licensed under MIT.

namespace scroll {
	inline FileLogger::FileLogger(std::ofstream& stream, LogLevel minLogLevel, view<char8> source) :
		Logger(minLogLevel, source),
		stream(stream)
	{
		ATL_ASSERT(stream.is_open());
	}

	template<typename Argument>
	inline void FileLogger::trace(view<char8> file, uint64 line, Argument&& argument) {
		writeTraceLog(file, line, argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void FileLogger::trace(view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		writeTraceLog(file, line, pattern, std::forward<Argument>(arguments)...);
	}

	template<typename Argument>
	inline void FileLogger::debug(Argument&& argument) {
		writeDebugLog(argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void FileLogger::debug(view<char8> pattern, Argument&&... arguments) {
		writeDebugLog(pattern, std::forward<Argument>(arguments)...);
	}

	template<typename Argument>
	inline void FileLogger::info(Argument&& argument) {
		writeInfoLog(argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void FileLogger::info(view<char8> pattern, Argument&&... arguments) {
		writeInfoLog(pattern, std::forward<Argument>(arguments)...);
	}

	template<typename Argument>
	inline void FileLogger::warning(Argument&& argument) {
		writeWarningLog(argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void FileLogger::warning(view<char8> pattern, Argument&&... arguments) {
		writeWarningLog(pattern, std::forward<Argument>(arguments)...);
	}

	template<typename Argument>
	inline void FileLogger::error(view<char8> function, view<char8> file, uint64 line, Argument&& argument) {
		writeErrorLog(function, file, line, argumentInjectionPattern, std::forward<Argument>(argument));
	}

	template<typename... Argument>
	inline void FileLogger::error(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		writeErrorLog(function, file, line, pattern, std::forward<Argument>(arguments)...);
	}

	template<typename... Argument>
	inline uint64 FileLogger::writeFileLog(view<char8> level, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_BUFFER_TEMPLATE = "[00:00:00.000000] ";

		return writeLog(1, LOG_BUFFER_TEMPLATE, level, level.count(), pattern, std::forward<Argument>(arguments)...);
	}

	template<typename... Argument>
	inline void FileLogger::writeTraceLog(view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "TRACE ";

		if (LogLevel::TRACE < minLogLevel) {
			return;
		}

		uint64 offset = writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);

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
	inline void FileLogger::writeDebugLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "DEBUG ";

		if (LogLevel::DEBUG < minLogLevel) {
			return;
		}

		uint64 offset = writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);

		write('\n', offset);

		flushLog(offset);
	}

	template<typename... Argument>
	inline void FileLogger::writeInfoLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "INFO ";

		if (LogLevel::INFO < minLogLevel) {
			return;
		}

		uint64 offset = writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);

		write('\n', offset);

		flushLog(offset);
	}

	template<typename... Argument>
	inline void FileLogger::writeWarningLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "WARNING ";

		if (LogLevel::WARNING < minLogLevel) {
			return;
		}

		uint64 offset = writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);

		write('\n', offset);

		flushLog(offset);
	}

	template<typename... Argument>
	inline void FileLogger::writeErrorLog(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "ERROR ";

		if (LogLevel::ERROR < minLogLevel) {
			return;
		}

		uint64 offset = writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);

		write(" (in ", offset);
		write(function, offset);
		write(", at ", offset);
		write(file, offset);
		write(':', offset);

		const std::string formattedLine = toString(line);

		write(&formattedLine[0], offset);
		write(')', offset);
		write('\n', offset);

		flushLog(offset);
	}

	inline void FileLogger::flushLog(uint64 size) {
		ATL_ASSERT(size <= MAX_LOG_BUFFER_SIZE);

		stream.write(&logBuffer[0], size);
	}
}