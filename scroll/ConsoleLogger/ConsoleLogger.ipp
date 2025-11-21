// Copyright 2025 Atalante.
// Licensed under MIT.

namespace scroll {
	inline void ConsoleLogger::writeLogHeader(std::ostream& stream, view<char8> source, view<char8> levelName, ConsoleEscapeCode levelBackgroundColor, ConsoleEscapeCode levelForegroundColor) {
		stream << "\033[" << ConsoleEscapeCode::DIMMED << "m[" << timestamp() << "]\033[" << ConsoleEscapeCode::RESET_BRIGHT << "m ";

		if (source.count() > 0) {
			stream << source << ' ';
		}

		stream << "\033[" << levelBackgroundColor << ';' << levelForegroundColor << "m " << levelName << ' ';
		stream << "\033[" << ConsoleEscapeCode::RESET_BACKGROUND_COLOR << ';' << ConsoleEscapeCode::RESET_FOREGROUND_COLOR << "m ";
	}

	inline ConsoleLogger::ConsoleLogger(std::ostream& stream, LogLevel minimumLogLevel, view<char8> source) :
		Logger(minimumLogLevel, source)
	{
		setOutputStream(stream);
	}

	inline ConsoleLogger::~ConsoleLogger() {
		if (writingEscapeCodes) {
			stream->write("m", 1);
		}
	}

	inline std::ostream& ConsoleLogger::getOutputStream() {
		return *stream;
	}

	inline void ConsoleLogger::setOutputStream(std::ostream& stream) {
		this->stream = &stream;

		#if ATL_OPERATING_SYSTEM == ATL_OPERATING_SYSTEM_WINDOWS
			const HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

			ATL_ASSERT(hConsoleHandle != NULL && hConsoleHandle != INVALID_HANDLE_VALUE);
			ATL_ASSERT(SetConsoleMode(hConsoleHandle, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING) > 0);
		#endif
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
	void ConsoleLogger::trace(view<char8> file, uint32 line, Argument&& argument) {
		if (LogLevel::TRACE < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "TRACE", ConsoleEscapeCode::BACKGROUND_COLOR_WHITE, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format("[] (at []:[])", format(std::forward<Argument>(argument)), file, line), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::trace(view<char8> file, uint32 line, view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::TRACE < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "TRACE", ConsoleEscapeCode::BACKGROUND_COLOR_WHITE, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format("[] (at []:[])", format(pattern, std::forward<Argument>(arguments)...), file, line), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename Argument>
	void ConsoleLogger::debug(Argument&& argument) {
		if (LogLevel::DEBUG < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "DEBUG", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_GRAY, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::debug(view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::DEBUG < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "DEBUG", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_GRAY, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename Argument>
	void ConsoleLogger::info(Argument&& argument) {
		if (LogLevel::INFO < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cout;

		writeLogHeader(stream, source, "INFO", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_BLUE, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 3);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::info(view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::INFO < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cout;

		writeLogHeader(stream, source, "INFO", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_BLUE, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 3);

		stream << '\n';
	}

	template<typename Argument>
	void ConsoleLogger::warning(Argument&& argument) {
		if (LogLevel::WARNING < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "WARNING", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_YELLOW, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		stream << "\033[" << ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_YELLOW << 'm';
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation());
		stream << "\033[" << ConsoleEscapeCode::RESET_FOREGROUND_COLOR << "m\n";
	}

	template<typename... Argument>
	void ConsoleLogger::warning(view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::WARNING < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "WARNING", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_YELLOW, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		stream << "\033[" << ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_YELLOW << 'm';
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation());
		stream << "\033[" << ConsoleEscapeCode::RESET_FOREGROUND_COLOR << "m\n";
	}

	template<typename Argument>
	void ConsoleLogger::error(view<char8> function, view<char8> file, uint64 line, Argument&& argument) {
		if (LogLevel::ERROR < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "ERROR", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_RED, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		stream << "\033[" << ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_RED << 'm';
		writeIndented(stream, format("[] (in [], at []:[])", format(std::forward<Argument>(argument)), function, file, line), getLogIndentation() - 2);
		stream << "\033[" << ConsoleEscapeCode::RESET_FOREGROUND_COLOR << "m\n";
	}

	template<typename... Argument>
	void ConsoleLogger::error(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::ERROR < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "ERROR", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_RED, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		stream << "\033[" << ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_RED << 'm';
		writeIndented(stream, format("[] (in [], at []:[])", format(pattern, std::forward<Argument>(arguments)...), function, file, line), getLogIndentation() - 2);
		stream << "\033[" << ConsoleEscapeCode::RESET_FOREGROUND_COLOR << "m\n";
	}
}