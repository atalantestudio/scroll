#include "pch.hpp"

#include "Adapter/Logger.hpp"

namespace ProjectA {
	// TODO: Try to remove.
	std::ostream& operator <<(std::ostream& stream, const str8& value) {
		return stream.write(&value[0], value.count());
	}

	str8 Logger::argumentInjectionPattern = "[]";

	const str8& Logger::getArgumentInjectionPattern() {
		return argumentInjectionPattern;
	}

	void Logger::setArgumentInjectionPattern(const str8& pattern) {
		ASSERT(pattern.count() > 0);

		argumentInjectionPattern = pattern;
	}

	void Logger::writeTimestamp(std::ostream& stream) {
		const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		const std::time_t time = std::chrono::system_clock::to_time_t(now);
		const std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - std::chrono::time_point_cast<std::chrono::seconds>(now));

		std::tm dateTime;

		localtime_s(&dateTime, &time);

		stream << std::put_time(&dateTime, "%H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << milliseconds.count() << std::setfill(' ');
	}

	Logger::Logger(LogLevel minLogLevel, const str8& source) :
		minLogLevel(minLogLevel),
		source(source)
	{}

	uint16 Logger::getLogIndentation() const {
		uint16 indentation = static_cast<uint16>(14 + 1 + 9 + 1);

		if (source.count() > 0) {
			indentation += static_cast<uint16>(source.count()) + 1;
		}

		return indentation;
	}

	void Logger::write(std::ostream& stream, const str8& text) {
		stream << text;
	}

	void Logger::writeIndented(std::ostream& stream, const str8& text, uint64 indentation) {
		str8 indentationString(indentation);

		std::fill_n(&indentationString[0], indentation, ' ');

		uint64 previousOffset = 0;
		uint64 offset = 0;

		while (true) {
			offset = text.find('\n', previousOffset);

			if (offset == previousOffset) {
				break;
			}

			stream.write(&text[previousOffset], offset - previousOffset);

			if (offset >= text.count()) {
				break;
			}

			previousOffset = offset + 1;

			stream << '\n' << indentationString;
		}
	}
}