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
	inline void FileLogger::writeFileLog(view<char8> level, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_BUFFER_TEMPLATE = "[00:00:00.000000] ";

		writeLog(1, LOG_BUFFER_TEMPLATE, level, level.count(), pattern, std::forward<Argument>(arguments)...);
	}

	template<typename... Argument>
	inline void FileLogger::writeTraceLog(view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "TRACE ";

		if (LogLevel::TRACE < minLogLevel) {
			return;
		}

		writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);
		buffer << " (at " << file << ':' << line << ')' << '\n';

		flush();
	}

	template<typename... Argument>
	inline void FileLogger::writeDebugLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "DEBUG ";

		if (LogLevel::DEBUG < minLogLevel) {
			return;
		}

		writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);
		buffer << '\n';

		flush();
	}

	template<typename... Argument>
	inline void FileLogger::writeInfoLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "INFO ";

		if (LogLevel::INFO < minLogLevel) {
			return;
		}

		writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);
		buffer << '\n';

		flush();
	}

	template<typename... Argument>
	inline void FileLogger::writeWarningLog(view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "WARNING ";

		if (LogLevel::WARNING < minLogLevel) {
			return;
		}

		writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);
		buffer << '\n';

		flush();
	}

	template<typename... Argument>
	inline void FileLogger::writeErrorLog(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		static constexpr view<char8> LOG_LEVEL = "ERROR ";

		if (LogLevel::ERROR < minLogLevel) {
			return;
		}

		writeFileLog(LOG_LEVEL, pattern, std::forward<Argument>(arguments)...);
		buffer << " (in " << function << ", at " << file << ':' << line << ')' << '\n';

		flush();
	}

	inline void FileLogger::flush() {
		stream.write(&buffer.buffer[0], buffer.offset);
	}
}