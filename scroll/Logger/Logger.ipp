// Copyright 2025 Atalante.
// Licensed under MIT.

namespace scroll {
	template<typename T>
	sequence<char8> BaseLogger<T>::argumentInjectionPattern = "[]";

	inline view<char8> Logger::getArgumentInjectionPattern() {
		return argumentInjectionPattern;
	}

	inline void Logger::setArgumentInjectionPattern(view<char8> pattern) {
		ATL_ASSERT(pattern.count() > 0);

		argumentInjectionPattern = pattern;
	}

	template<typename Argument>
	inline std::string Logger::toString(Argument argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string Logger::toString(const char8* argument) {
		return argument;
	}

	inline sequence<char8> Logger::format(const sequence<char8>& pattern) {
		return pattern;
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

	inline void Logger::write(char8 character, uint64& offset) {
		logBuffer[offset] = character;

		offset += 1;
	}

	inline void Logger::write(view<char8> text, uint64& offset) {
		copy(text.begin(), text.end(), &logBuffer[offset]);

		offset += text.count();
	}

	inline void Logger::write(view<char8> text, uint64& offset, uint64 padding) {
		copy(text.begin(), text.end(), &logBuffer[offset] + padding - text.count());

		offset += padding;
	}

	inline uint64 Logger::writeFormattedArguments(view<char8> pattern, uint64& offset, const sequence<char8>& indentationString) {
		return writeIndented(pattern, offset, indentationString);
	}
}