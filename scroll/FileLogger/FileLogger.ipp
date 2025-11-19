// Copyright 2025 Atalante.
// Licensed under MIT.

namespace scroll {
	inline FileLogger::FileLogger(std::ofstream& stream, LogLevel minLogLevel, view<char8> source) :
		Logger(minLogLevel, source),
		stream(stream)
	{
		ASSERT(stream.is_open());
	}

	template<typename Argument>
	void FileLogger::trace(view<char8> file, uint32 line, Argument&& argument) {
		if (LogLevel::TRACE < minLogLevel) {
			return;
		}

		writeLogHeader("TRACE");
		writeIndented(stream, format("[]\nat []:[]", format(std::forward<Argument>(argument)), file, line), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::trace(view<char8> file, uint32 line, view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::TRACE < minLogLevel) {
			return;
		}

		writeLogHeader("TRACE");
		writeIndented(stream, format("[]\nat []:[]", format(pattern, std::forward<Argument>(arguments)...), file, line), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename Argument>
	void FileLogger::debug(Argument&& argument) {
		if (LogLevel::DEBUG < minLogLevel) {
			return;
		}

		writeLogHeader("DEBUG");
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::debug(view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::DEBUG < minLogLevel) {
			return;
		}

		writeLogHeader("DEBUG");
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename Argument>
	void FileLogger::info(Argument&& argument) {
		if (LogLevel::INFO < minLogLevel) {
			return;
		}

		writeLogHeader("INFO");
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 5);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::info(view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::INFO < minLogLevel) {
			return;
		}

		writeLogHeader("INFO");
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 5);

		stream << '\n';
	}

	template<typename Argument>
	void FileLogger::warning(Argument&& argument) {
		if (LogLevel::WARNING < minLogLevel) {
			return;
		}

		writeLogHeader("WARNING");
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::warning(view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::WARNING < minLogLevel) {
			return;
		}

		writeLogHeader("WARNING");
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename Argument>
	void FileLogger::error(view<char8> function, view<char8> file, uint64 line, Argument&& argument) {
		if (LogLevel::ERROR < minLogLevel) {
			return;
		}

		writeLogHeader("ERROR");
		writeIndented(stream, format("[]\nat [] ([]:[])", format(std::forward<Argument>(argument)), function, file, line), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::error(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::ERROR < minLogLevel) {
			return;
		}

		writeLogHeader("ERROR");
		writeIndented(stream, format("[]\nat [] ([]:[])", format(pattern, std::forward<Argument>(arguments)...), function, file, line), getLogIndentation() - 4);

		stream << '\n';
	}

	inline void FileLogger::writeLogHeader(view<char8> levelName) {
		stream << '[' << timestamp() << "] ";

		if (source.count() > 0) {
			stream << source << ' ';
		}

		stream << levelName << ' ';
	}
}