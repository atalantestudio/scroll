// Copyright 2025 Atalante.
// Licensed under MIT.

namespace scroll {
	template<typename T>
	sequence<char8> BaseLogger<T>::argumentInjectionPattern = "[]";

	template<typename T>
	TextBuffer BaseLogger<T>::formatBuffer;

	inline view<char8> Logger::getArgumentInjectionPattern() {
		return argumentInjectionPattern;
	}

	inline void Logger::setArgumentInjectionPattern(view<char8> pattern) {
		ATL_ASSERT(pattern.count() > 0);

		argumentInjectionPattern = pattern;
	}

	inline sequence<char8> Logger::format(view<char8> pattern) {
		formatBuffer << pattern;

		const sequence<char8> text = formatBuffer.getText();

		formatBuffer.clear();

		return text;
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

	inline void Logger::writeFormattedArguments(view<char8> pattern, const sequence<char8>& indentationString) {
		writeIndented(pattern, indentationString);
	}
}