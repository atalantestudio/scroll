// Copyright 2025 Atalante.
// Licensed under MIT.

namespace USER_NAMESPACE {
	inline void ConsoleLogger::writeLogHeader(std::ostream& stream, view<char8> source, view<char8> levelName, ConsoleEscapeCode levelBackgroundColor, ConsoleEscapeCode levelForegroundColor) {
		stream << "\033[" << ConsoleEscapeCode::DIMMED << "m[" << timestamp() << "]\033[" << ConsoleEscapeCode::RESET_BRIGHT << "m ";

		if (source.count() > 0) {
			stream << source << ' ';
		}

		stream << "\033[" << levelBackgroundColor << ';' << levelForegroundColor << "m " << levelName << ' ';
		stream << "\033[" << ConsoleEscapeCode::RESET_BACKGROUND_COLOR << ';' << ConsoleEscapeCode::RESET_FOREGROUND_COLOR << "m ";
	}

	inline ConsoleLogger::ConsoleLogger(std::ostream& stream, LogLevel minimumLogLevel, view<char8> source) :
		Logger(minimumLogLevel, source),
		stream(&stream)
	{}

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
	ConsoleLogger& ConsoleLogger::operator<<(ConsoleEscapeCode&& code) {
		if (!writingEscapeCodes) {
			writingEscapeCodes = true;

			stream->write("\033[", 3);
		}
		else {
			stream->write(";", 1);
		}

		*stream << static_cast<uint16>(code);

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

	inline void ConsoleLogger::writeLink(view<char8> url) {
		writeLink(url, url);
	}

	inline void ConsoleLogger::writeLink(view<char8> url, view<char8> placeholder) {
		*stream << ConsoleEscapeCode::FOREGROUND_COLOR_CYAN << "\033]8;;" << url << "\033\\" << placeholder << "\033]8;;\033\\" << ConsoleEscapeCode::RESET_FOREGROUND_COLOR;
	}

	template<typename Argument>
	void ConsoleLogger::trace(view<char8> file, uint32 line, Argument&& argument) {
		if (LogLevel::TRACE < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "TRACE", ConsoleEscapeCode::BACKGROUND_COLOR_WHITE, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format("[]\nat []:[]", format(std::forward<Argument>(argument)), file, line), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::trace(view<char8> file, uint32 line, view<char8> pattern, Argument&&... arguments) {
		if (LogLevel::TRACE < minLogLevel) {
			return;
		}

		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "TRACE", ConsoleEscapeCode::BACKGROUND_COLOR_WHITE, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format("[]\nat []:[]", format(pattern, std::forward<Argument>(arguments)...), file, line), getLogIndentation() - 2);

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
		writeIndented(stream, format("[]\nat [] ([]:[])", format(std::forward<Argument>(argument)), function, file, line), getLogIndentation() - 2);
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
		writeIndented(stream, format("[]\nat [] ([]:[])", format(pattern, std::forward<Argument>(arguments)...), function, file, line), getLogIndentation() - 2);
		stream << "\033[" << ConsoleEscapeCode::RESET_FOREGROUND_COLOR << "m\n";
	}
}