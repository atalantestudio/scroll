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

	template<typename... Argument>
	void ConsoleLogger::writeEscapeCodes(Argument&&... codes) {
		writeStreamEscapeCodes(*stream, std::forward<Argument>(codes)...);
	}

	template<typename Argument>
	void ConsoleLogger::trace(const str8& file, uint32 line, Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "TRACE", ConsoleEscapeCode::BACKGROUND_COLOR_WHITE, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format("[]\nat []:[]", format(std::forward<Argument>(argument)), file, line), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::trace(const str8& file, uint32 line, const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "TRACE", ConsoleEscapeCode::BACKGROUND_COLOR_WHITE, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format("[]\nat []:[]", format(pattern, std::forward<Argument>(arguments)...), file, line), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename Argument>
	void ConsoleLogger::debug(Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "DEBUG", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_GRAY, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::debug(const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "DEBUG", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_GRAY, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename Argument>
	void ConsoleLogger::info(Argument&& argument) {
		std::ostream& stream = std::cout;

		writeLogHeader(stream, source, "INFO", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_BLUE, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 3);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::info(const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cout;

		writeLogHeader(stream, source, "INFO", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_BLUE, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 3);

		stream << '\n';
	}

	template<typename Argument>
	void ConsoleLogger::warning(Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "WARNING", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_YELLOW, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_YELLOW);
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation());
		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::warning(const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "WARNING", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_YELLOW, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_YELLOW);
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation());
		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		stream << '\n';
	}

	template<typename Argument>
	void ConsoleLogger::error(const str8& function, const str8& file, uint64 line, Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "ERROR", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_RED, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_RED);
		writeIndented(stream, format("[]\nat [] ([]:[])", format(std::forward<Argument>(argument)), function, file, line), getLogIndentation() - 2);
		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::error(const str8& function, const str8& file, uint64 line, const str8& pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "ERROR", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_RED, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_RED);
		writeIndented(stream, format("[]\nat [] ([]:[])", format(pattern, std::forward<Argument>(arguments)...), function, file, line), getLogIndentation() - 2);
		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		stream << '\n';
	}
}