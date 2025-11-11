namespace ProjectA {
	template<typename... Argument>
	void ConsoleLogger::writeStreamEscapeCodes(std::ostream& stream, Argument&&... codes) {
		stream << "\033[";

		writeStreamEscapeCodesInternal(stream, std::forward<Argument>(codes)...);
	}

	template<typename... Argument>
	void ConsoleLogger::writeStreamEscapeCodesInternal(std::ostream& stream, ConsoleEscapeCode code, Argument&&... codes) {
		stream << static_cast<uint16>(code) << ';';

		writeStreamEscapeCodesInternal(stream, std::forward<Argument>(codes)...);
	}

	template<typename Argument>
	void ConsoleLogger::trace(const str8& file, uint32 line, Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);

		writeLog(stream, LogLevel::TRACE, format(std::forward<Argument>(argument)), getLogLineWidth());
		writeIndented(stream, format("at []:[]", file, line), getLogIndentation(), true, getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename... Argument>
	void ConsoleLogger::trace(const str8& file, uint32 line, const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);

		writeLog(stream, LogLevel::TRACE, format(pattern, std::forward<Argument>(arguments)...), getLogLineWidth());
		writeIndented(stream, format("at []:[]", file, line), getLogIndentation(), true, getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename Argument>
	void ConsoleLogger::debug(Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_GRAY);

		writeLog(stream, LogLevel::DEBUG, format(std::forward<Argument>(argument)), getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename... Argument>
	void ConsoleLogger::debug(const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_GRAY);

		writeLog(stream, LogLevel::DEBUG, format(pattern, std::forward<Argument>(arguments)...), getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename Argument>
	void ConsoleLogger::info(Argument&& argument) {
		std::ostream& stream = std::cout;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_BLUE);

		writeLog(stream, LogLevel::INFO, format(std::forward<Argument>(argument)), getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename... Argument>
	void ConsoleLogger::info(const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cout;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_BLUE);

		writeLog(stream, LogLevel::INFO, format(pattern, std::forward<Argument>(arguments)...), getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename Argument>
	void ConsoleLogger::warning(Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_YELLOW);

		writeLog(stream, LogLevel::WARNING, format(std::forward<Argument>(argument)), getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename... Argument>
	void ConsoleLogger::warning(const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_YELLOW);

		writeLog(stream, LogLevel::WARNING, format(pattern, std::forward<Argument>(arguments)...), getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename Argument>
	void ConsoleLogger::error(const str8& function, const str8& file, uint64 line, Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_RED, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);

		writeLog(stream, LogLevel::ERROR, format(std::forward<Argument>(argument)), getLogLineWidth());
		writeIndented(stream, format("at [] ([]:[])", function, file, line), getLogIndentation(), true, getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_BACKGROUND_COLOR, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	template<typename... Argument>
	void ConsoleLogger::error(const str8& function, const str8& file, uint64 line, const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_RED, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);

		writeLog(stream, LogLevel::ERROR, format(pattern, std::forward<Argument>(arguments)...), getLogLineWidth());
		writeIndented(stream, format("at [] ([]:[])", function, file, line), getLogIndentation(), true, getLogLineWidth());

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_BACKGROUND_COLOR, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}
}