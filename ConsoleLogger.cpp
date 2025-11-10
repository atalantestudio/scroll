#pragma once

#include "Adapter/Adapter.hpp"

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

	uint16 ConsoleLogger::getLogLineWidth() const {
		return getConsoleWidth() - getLogIndentation();
	}

	ConsoleLogger::ConsoleLogger(std::ostream& stream, LogLevel minimumLogLevel) :
		Logger(minimumLogLevel),
		stream(&stream)
	{
		#if OPERATING_SYSTEM == OPERATING_SYSTEM_LINUX
			// TODO
		#elif OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS
			ASSERT(SetConsoleOutputCP(CP_UTF8) != 0);
		#endif
	}

	void ConsoleLogger::setOutputStream(std::ostream& stream) {
		this->stream = &stream;
	}

	template<typename... Argument>
	void ConsoleLogger::writeEscapeCodes(Argument&&... codes) {
		writeStreamEscapeCodes(*stream, std::forward<Argument>(codes)...);
	}

	void ConsoleLogger::print(const str8& string) {
		*stream << string;
	}

	void ConsoleLogger::prefixPrint(const str8& prefix, const str8& string) {
		const uint16 consoleWidth = getConsoleWidth() - static_cast<uint16>(prefix.count()) - 1;

		uint64 previousCharacterOffset = 0;
		uint64 lineCharacterIndex;

		while (true) {
			const uint64 characterOffset = string.find('\n', previousCharacterOffset);
			const uint64 lineWidth = characterOffset - previousCharacterOffset;
			lineCharacterIndex = 0;

			while (true) {
				const uint64 width = min(consoleWidth, lineWidth - lineCharacterIndex);

				*stream << prefix << ' ';
				stream->write(&string[previousCharacterOffset + lineCharacterIndex], width);
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

			if (previousCharacterOffset >= string.count()) {
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