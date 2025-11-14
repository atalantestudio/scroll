namespace USER_NAMESPACE {
	inline void ConsoleLogger::writeLogHeader(std::ostream& stream, view<char8> source, view<char8> levelName, ConsoleEscapeCode levelBackgroundColor, ConsoleEscapeCode levelForegroundColor) {
		writeStreamEscapeCodes(stream, ConsoleEscapeCode::DIMMED);

		stream << '[' << timestamp() << ']';

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_BRIGHT);

		stream << ' ';

		if (source.count() > 0) {
			stream << source << ' ';
		}

		writeStreamEscapeCodes(stream, levelBackgroundColor, levelForegroundColor);

		stream << ' ' << levelName << ' ';

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_BACKGROUND_COLOR, ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		stream << ' ';
	}

	inline void ConsoleLogger::writeStreamEscapeCodesInternal(std::ostream& stream, ConsoleEscapeCode code) {
		stream << static_cast<uint16>(code) << 'm';
	}

	inline ConsoleLogger::ConsoleLogger(std::ostream& stream, LogLevel minimumLogLevel, view<char8> source) :
		Logger(minimumLogLevel, source),
		stream(&stream)
	{}

	inline std::ostream& ConsoleLogger::getOutputStream() {
		return *stream;
	}

	inline void ConsoleLogger::setOutputStream(std::ostream& stream) {
		this->stream = &stream;
	}

	inline void ConsoleLogger::write(view<char8> text) {
		*stream << text;
	}

	inline void ConsoleLogger::title(view<char8> text) {
		writeEscapeCodes(ConsoleEscapeCode::BRIGHT);

		*stream << text << '\n';

		writeEscapeCodes(ConsoleEscapeCode::RESET_BRIGHT);
	}

	inline void ConsoleLogger::writeLink(view<char8> url) {
		writeLink(url, url);
	}

	inline void ConsoleLogger::writeLink(view<char8> url, view<char8> placeholder) {
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_CYAN);

		*stream << "\033]8;;" << url << "\033\\" << placeholder << "\033]8;;\033\\";

		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	inline void ConsoleLogger::listItem(view<char8> text) {
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_YELLOW);

		*stream << '-';

		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		*stream << ' ' << text << '\n';
	}

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
	void ConsoleLogger::trace(view<char8> file, uint32 line, Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "TRACE", ConsoleEscapeCode::BACKGROUND_COLOR_WHITE, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeIndented(stream, format("[]\nat []:[]", format(std::forward<Argument>(argument)), file, line), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::trace(view<char8> file, uint32 line, view<char8> pattern, Argument&&... arguments) {
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
	void ConsoleLogger::debug(view<char8> pattern, Argument&&... arguments) {
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
	void ConsoleLogger::info(view<char8> pattern, Argument&&... arguments) {
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
	void ConsoleLogger::warning(view<char8> pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "WARNING", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_YELLOW, ConsoleEscapeCode::FOREGROUND_COLOR_BLACK);
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_YELLOW);
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation());
		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		stream << '\n';
	}

	template<typename Argument>
	void ConsoleLogger::error(view<char8> function, view<char8> file, uint64 line, Argument&& argument) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "ERROR", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_RED, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_RED);
		writeIndented(stream, format("[]\nat [] ([]:[])", format(std::forward<Argument>(argument)), function, file, line), getLogIndentation() - 2);
		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		stream << '\n';
	}

	template<typename... Argument>
	void ConsoleLogger::error(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments) {
		std::ostream& stream = std::cerr;

		writeLogHeader(stream, source, "ERROR", ConsoleEscapeCode::BACKGROUND_COLOR_LIGHT_RED, ConsoleEscapeCode::FOREGROUND_COLOR_WHITE);
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_LIGHT_RED);
		writeIndented(stream, format("[]\nat [] ([]:[])", format(pattern, std::forward<Argument>(arguments)...), function, file, line), getLogIndentation() - 2);
		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		stream << '\n';
	}
}