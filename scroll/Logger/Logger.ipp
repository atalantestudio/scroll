// Copyright 2025 Atalante.
// Licensed under MIT.

namespace scroll {
	template<typename T>
	sequence<char8> ArgumentInjectionPattern<T>::argumentInjectionPattern = "[]";

	template<typename Argument>
	sequence<char8> Logger::format(Argument&& argument) {
		static std::ostringstream stream;

		stream << argument;

		const sequence<char8> formattedArgument = stream.str().c_str();

		stream.str({});

		return formattedArgument;
	}

	template<typename HeadArgument, typename... Argument>
	sequence<char8> Logger::format(view<char8> pattern, HeadArgument&& headArgument, Argument&&... arguments) {
		const uint64 argumentInjectionPatternOffset = pattern.find(argumentInjectionPattern, 0);

		if (argumentInjectionPatternOffset >= pattern.count()) {
			return pattern;
		}

		const sequence<char8> formattedArgument = format(headArgument);

		sequence<char8> updatedPattern(pattern.count() - argumentInjectionPattern.count() + formattedArgument.count());

		copy(&pattern[0], &pattern[argumentInjectionPatternOffset], &updatedPattern[0]);
		copy(formattedArgument.begin(), formattedArgument.end(), &updatedPattern[argumentInjectionPatternOffset]);

		if (argumentInjectionPatternOffset + argumentInjectionPattern.count() < pattern.count()) {
			copy(&pattern[argumentInjectionPatternOffset + argumentInjectionPattern.count()], pattern.end(), &updatedPattern[argumentInjectionPatternOffset + formattedArgument.count()]);
		}

		return format(updatedPattern, std::forward<Argument>(arguments)...);
	}

	inline view<char8> Logger::getArgumentInjectionPattern() {
		return argumentInjectionPattern;
	}

	inline void Logger::setArgumentInjectionPattern(view<char8> pattern) {
		ATL_ASSERT(pattern.count() > 0);

		argumentInjectionPattern = &pattern[0];
	}

	inline sequence<char8> Logger::timestamp() {
		static std::ostringstream usStream;

		const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		const uint64 us = std::chrono::duration_cast<std::chrono::microseconds>(now - std::chrono::time_point_cast<std::chrono::seconds>(now)).count();
		const std::time_t time = std::chrono::system_clock::to_time_t(now);

		std::tm* dateTime;

		#ifdef _MSC_VER
			std::tm _dateTime{};

			const errno_t error = localtime_s(&_dateTime, &time);

			ATL_ASSERT(error == 0);

			dateTime = &_dateTime;
		#else
			dateTime = std::localtime(&time);
		#endif

		// Include null-terminating character.
		sequence<char8> hmsString(9);

		const uint64 hmsCharacterCount = std::strftime(&hmsString[0], hmsString.count(), "%H:%M:%S", dateTime);

		ATL_ASSERT(hmsCharacterCount > 0);

		usStream << std::setw(6) << std::setfill('0') << us;

		const sequence<char8> timestamp = format("[].[]", hmsString, usStream.str());

		usStream.str({});

		return timestamp;
	}

	inline LogLevel Logger::getMinLogLevel() const {
		return minLogLevel;
	}

	inline view<char8> Logger::getSource() const {
		return source;
	}

	inline Logger::Logger(LogLevel minLogLevel, view<char8> source) :
		minLogLevel(minLogLevel),
		source(source)
	{}

	inline uint16 Logger::getLogIndentation() const {
		uint16 indentation = static_cast<uint16>(TIMESTAMP_SIZE + MAX_LOG_LEVEL_NAME_SIZE + 6);

		if (source.count() > 0) {
			indentation += static_cast<uint16>(source.count()) + 1;
		}

		return indentation;
	}

	inline void Logger::writeIndented(std::ostream& stream, view<char8> text, uint64 indentation) {
		// TODO: Create string with fixed indentation and write parts of it.
		sequence<char8> indentationString(indentation);

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