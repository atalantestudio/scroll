#include "pch.hpp"

#include "Adapter/ConsoleLogger.hpp"

namespace ProjectA {
	void ConsoleLogger::writeLogHeader(std::ostream& stream, const str8& source, const str8& levelName, ConsoleEscapeCode levelBackgroundColor, ConsoleEscapeCode levelForegroundColor) {
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

	void ConsoleLogger::writeStreamEscapeCodesInternal(std::ostream& stream, ConsoleEscapeCode code) {
		stream << static_cast<uint16>(code) << 'm';
	}

	uint16 ConsoleLogger::getConsoleWidth() {
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

	ConsoleLogger::ConsoleLogger(std::ostream& stream, LogLevel minimumLogLevel, const str8& source) :
		Logger(minimumLogLevel, source),
		stream(&stream)
	{
		#if OPERATING_SYSTEM == OPERATING_SYSTEM_LINUX
			// TODO
		#elif OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS
			ASSERT(SetConsoleOutputCP(CP_UTF8) != 0);
		#endif
	}

	std::ostream& ConsoleLogger::getOutputStream() {
		return *stream;
	}

	void ConsoleLogger::setOutputStream(std::ostream& stream) {
		this->stream = &stream;
	}

	void ConsoleLogger::write(const str8& text) {
		*stream << text;
	}

	void ConsoleLogger::prefixPrint(const str8& prefix, const str8& text) {
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

	void ConsoleLogger::title(const str8& text) {
		writeEscapeCodes(ConsoleEscapeCode::BRIGHT);

		*stream << text << '\n';

		writeEscapeCodes(ConsoleEscapeCode::RESET_BRIGHT);
	}

	void ConsoleLogger::comment(const str8& text) {
		writeEscapeCodes(ConsoleEscapeCode::DIMMED);

		prefixPrint("//", text);

		writeEscapeCodes(ConsoleEscapeCode::RESET_BRIGHT);
	}

	void ConsoleLogger::writeLink(const str8& url) {
		writeLink(url, url);
	}

	void ConsoleLogger::writeLink(const str8& url, const str8& placeholder) {
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_CYAN);

		*stream << "\033]8;;" << url << "\033\\" << placeholder << "\033]8;;\033\\";

		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);
	}

	void ConsoleLogger::listItem(const str8& text) {
		writeEscapeCodes(ConsoleEscapeCode::FOREGROUND_COLOR_YELLOW);

		*stream << '-';

		writeEscapeCodes(ConsoleEscapeCode::RESET_FOREGROUND_COLOR);

		*stream << ' ' << text << '\n';
	}
}