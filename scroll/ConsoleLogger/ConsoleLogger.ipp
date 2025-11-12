namespace ProjectA {
	inline void ConsoleLogger::writeLogHeader(std::ostream& stream, const str8& source, const str8& levelName, ConsoleEscapeCode levelBackgroundColor, ConsoleEscapeCode levelForegroundColor) {
		stream << '[';

		writeStreamEscapeCodes(stream, ConsoleEscapeCode::DIMMED);
		writeTimestamp(stream);
		writeStreamEscapeCodes(stream, ConsoleEscapeCode::RESET_BRIGHT);

		stream << "] ";

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

	inline uint16 ConsoleLogger::getConsoleWidth() {
		#if OPERATING_SYSTEM == OPERATING_SYSTEM_LINUX
			struct winsize ws;

			ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

			return ws.ws_row;
		#elif OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS
			const HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

			ASSERT(hConsoleOutput != INVALID_HANDLE_VALUE);

			CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo{};

			ASSERT(GetConsoleScreenBufferInfo(hConsoleOutput, &consoleScreenBufferInfo) != 0);

			return consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left + 1;
		#else
			return max<uint16>();
		#endif;
	}

	inline ConsoleLogger::ConsoleLogger(std::ostream& stream, LogLevel minimumLogLevel, const str8& source) :
		Logger(minimumLogLevel, source),
		stream(&stream)
	{
		#if OPERATING_SYSTEM == OPERATING_SYSTEM_LINUX
			// TODO
		#elif OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS
			ASSERT(SetConsoleOutputCP(CP_UTF8) != 0);
		#endif
	}

	inline std::ostream& ConsoleLogger::getOutputStream() {
		return *stream;
	}

	inline void ConsoleLogger::setOutputStream(std::ostream& stream) {
		this->stream = &stream;
	}

	inline void ConsoleLogger::write(const str8& text) {
		*stream << text;
	}

	inline void ConsoleLogger::prefixPrint(const str8& prefix, const str8& text) {
		const uint16 consoleWidth = getConsoleWidth() - static_cast<uint16>(prefix.count()) - 1;

		uint64 previousCharacterOffset = 0;
		uint64 lineCharacterIndex;

		while (true) {
			const uint64 characterOffset = text.find('\n', previousCharacterOffset);
			const uint64 lineWidth = characterOffset - previousCharacterOffset;
			lineCharacterIndex = 0;

			while (true) {
				uint64 width;

				// TODO: min(a, b)
				{
					width = lineWidth - lineCharacterIndex;

					if (width > consoleWidth) {
						width = consoleWidth;
					}
				}

				*stream << prefix << ' ';
				stream->write(&text[previousCharacterOffset + lineCharacterIndex], width);
				*stream << '\n';

				lineCharacterIndex += width;

				if (lineCharacterIndex >= lineWidth) {
					break;
				}
			}

			previousCharacterOffset = characterOffset;

			if (lineWidth <= consoleWidth) {
				previousCharacterOffset += 1;
			}

			if (previousCharacterOffset >= text.count()) {
				break;
			}
		}
	}

	inline void ConsoleLogger::title(const str8& text) {
		writeEscapeCodes(ConsoleEscapeCode::BRIGHT);

		*stream << text << '\n';

		writeEscapeCodes(ConsoleEscapeCode::RESET_BRIGHT);
	}

	inline void ConsoleLogger::comment(const str8& text) {
		writeEscapeCodes(ConsoleEscapeCode::DIMMED);
		prefixPrint("//", text);
		writeEscapeCodes(ConsoleEscapeCode::RESET_BRIGHT);
	}

	inline void ConsoleLogger::writeLink(const str8& url) {
		writeLink(url, url);
	}

	inline void ConsoleLogger::writeLink(const str8& url, const str8& placeholder) {
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_CYAN);

		*stream << "\033]8;;" << url << "\033\\" << placeholder << "\033]8;;\033\\";

		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	inline void ConsoleLogger::listItem(const str8& text) {
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