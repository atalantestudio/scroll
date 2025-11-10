#pragma once

#include "Adapter/Adapter.hpp"

namespace ProjectA {
	std::ostream& operator <<(std::ostream& stream, const str8& value) {
		if (value.count() == 0) {
			return stream << "";
		}

		return stream.write(&value[0], value.count());
	}

	str8 Logger::argumentInjectionPattern = "[]";

	uint16 Logger::maxSourceSize = 0;

	const str8& Logger::getArgumentInjectionPattern() {
		return argumentInjectionPattern;
	}

	void Logger::setArgumentInjectionPattern(const str8& pattern) {
		ASSERT(pattern.count() > 0);

		argumentInjectionPattern = pattern;
	}

	template<typename Argument>
	str8 Logger::format(Argument&& argument) {
		std::ostringstream stream;

		stream << argument;

		// TODO: Don't use std::string.
		return stream.str();
	}

	template<typename Argument, typename... PackedArgument>
	str8 Logger::format(const str8& pattern, Argument&& argument, PackedArgument&&... arguments) {
		const uint64 argumentInjectionPatternOffset = pattern.find(argumentInjectionPattern, 0);

		if (argumentInjectionPatternOffset >= pattern.count()) {
			return pattern;
		}

		const str8 formattedArgument = format(argument);

		str8 updatedPattern(pattern.count() - argumentInjectionPattern.count() + formattedArgument.count());

		std::copy_n(&pattern[0], argumentInjectionPatternOffset, &updatedPattern[0]);
		std::copy(formattedArgument.begin(), formattedArgument.end(), &updatedPattern[argumentInjectionPatternOffset]);

		if (argumentInjectionPatternOffset + argumentInjectionPattern.count() < pattern.count()) {
			std::copy(&pattern[argumentInjectionPatternOffset + argumentInjectionPattern.count()], pattern.end(), &updatedPattern[argumentInjectionPatternOffset + formattedArgument.count()]);
		}

		return format(updatedPattern, std::forward<PackedArgument>(arguments)...);
	}

	void Logger::writeTimestamp(std::ostream& stream) {
		const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		const std::time_t time = std::chrono::system_clock::to_time_t(now);
		const std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - std::chrono::time_point_cast<std::chrono::seconds>(now));

		std::tm dateTime;

		localtime_s(&dateTime, &time);

		stream << '[' << std::put_time(&dateTime, "%H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << milliseconds.count() << std::setfill(' ') << ']';
	}

	Logger::Logger(LogLevel minLogLevel) :
		minLogLevel(minLogLevel)
	{}

	uint16 Logger::getLogIndentation() const {
		uint16 indentation = static_cast<uint16>(LOG_INDENT + maxSourceSize);

		if (maxSourceSize > 0) {
			indentation += 2;
		}

		return indentation;
	}

	void Logger::write(std::ostream& stream, const str8& text) {
		stream << text;
	}

	void Logger::writeIndented(std::ostream& stream, const str8& text, uint64 indentation, bool indentFirstLine, uint64 maxLineWidth) {
		uint64 previousCharacterOffset = 0;
		std::string line;

		while (true) {
			const uint64 characterOffset = text.find('\n', previousCharacterOffset);
			uint64 lineWidth;

			// TODO: min(a, b)
			{
				lineWidth = characterOffset - previousCharacterOffset;

				if (lineWidth > maxLineWidth) {
					lineWidth = maxLineWidth;
				}
			}

			if (lineWidth == 0) {
				break;
			}

			line.assign(&text[previousCharacterOffset], lineWidth);

			if (previousCharacterOffset > 0 || (previousCharacterOffset == 0 && indentFirstLine)) {
				stream << std::setw(indentation + lineWidth);
			}

			stream << line << '\n';

			previousCharacterOffset += lineWidth;

			if (lineWidth < maxLineWidth) {
				previousCharacterOffset += 1;
			}
		}
	}

	void Logger::writeLog(std::ostream& stream, LogLevel level, const str8& text, uint64 maxLineWidth) const {
		ASSERT(source.count() <= maxSourceSize);

		if (level < minLogLevel || level > LogLevel::NONE) {
			return;
		}

		writeTimestamp(stream);

		if (maxSourceSize > 0) {
			stream << "  " << std::setw(maxSourceSize) << source << std::setw(0);
		}

		stream << "  " << std::setw(MAX_LOG_LEVEL_NAME_SIZE) << LOG_LEVEL_NAMES[static_cast<uint8>(level)] << "  ";

		writeIndented(stream, text, getLogIndentation(), false, maxLineWidth);
	}

	void Logger::setSource(const str8& source) {
		this->source = source;

		// TODO: max(a, b)
		if (source.count() > maxSourceSize) {
			// TODO: Size assertion?
			maxSourceSize = static_cast<uint16>(source.count());
		}
	}
}