// Copyright 2025 Atalante Studio.
// Distributed under the MIT License.

#pragma once

#include "scroll/LogLevel.hpp"
#include "scroll/TextBuffer/TextBuffer.hpp"

namespace scroll {
	template<typename T = void>
	class BaseLogger {
		protected:
			static sequence<char8> argumentInjectionPattern;
			static TextBuffer formatBuffer;
	};

	class Logger : public BaseLogger<> {
		public:
			// Returns the current argument injection pattern.
			static view<char8> getArgumentInjectionPattern();

			// Replaces the current argument injection pattern.
			static void setArgumentInjectionPattern(view<char8> pattern);

			static sequence<char8> format(view<char8> pattern);

			template<typename CurrentArgument, typename... Argument>
			static sequence<char8> format(view<char8> pattern, CurrentArgument&& argument, Argument&&... arguments) {
				const uint64 argumentInjectionPatternOffset = view<char8>(pattern).find(argumentInjectionPattern, 0);

				if (argumentInjectionPatternOffset > 0) {
					formatBuffer << view<char8>(&pattern[0], argumentInjectionPatternOffset);
				}

				if (argumentInjectionPatternOffset >= pattern.count()) {
					const sequence<char8> text = formatBuffer.getText();

					formatBuffer.clear();

					return text;
				}

				formatBuffer << argument;

				const uint64 patternOffset = argumentInjectionPatternOffset + argumentInjectionPattern.count();

				if (patternOffset >= pattern.count()) {
					const sequence<char8> text = formatBuffer.getText();

					formatBuffer.clear();

					return text;
				}

				pattern = view<char8>(&pattern[patternOffset], pattern.count() - patternOffset);

				return format(pattern, std::forward<Argument>(arguments)...);
			}

		private:
			static sequence<char8> createIndentationString(uint64 indentation) {
				sequence<char8> indentationString(indentation);

				for (char8& character : indentationString) {
					character = ' ';
				}

				return indentationString;
			}

		public:
			LogLevel getMinLogLevel() const;

			view<char8> getSource() const;

		protected:
			explicit Logger(LogLevel minLogLevel, view<char8> source);

			template<typename... Argument>
			void writeLog(uint64 offset, view<char8> bufferTemplate, view<char8> level, uint64 levelSize, view<char8> pattern, Argument&&... arguments) {
				#ifdef _MSC_VER
					const errno_t error = strncpy_s(&buffer.buffer[0], bufferTemplate.count() + 1, &bufferTemplate[0], bufferTemplate.count());

					ATL_ASSERT(error == 0);
				#else
					std::strncpy(&buffer.buffer[0], &bufferTemplate[0], bufferTemplate.count());
				#endif

				buffer.seek(offset);

				writeTimestamp();

				buffer.seek(bufferTemplate.count());

				uint64 indentation = 18;

				if (source.count() > 0) {
					buffer << source << ' ';

					indentation += source.count() + 1;
				}

				buffer << level;

				indentation += levelSize;

				const sequence<char8> indentationString = createIndentationString(indentation);

				writeFormattedArguments(pattern, indentationString, std::forward<Argument>(arguments)...);
			}

		private:
			void writeIndented(view<char8> pattern, const sequence<char8>& indentationString) {
				uint64 previousLineOffset = 0;
				uint64 lineOffset = 0;

				while (true) {
					lineOffset = pattern.find('\n', previousLineOffset);

					if (lineOffset > previousLineOffset) {
						buffer << view<char8>(&pattern[previousLineOffset], lineOffset - previousLineOffset);
					}

					if (lineOffset >= pattern.count()) {
						break;
					}

					buffer << '\n' << indentationString;

					previousLineOffset = lineOffset + 1;
				}
			}

			void writeFormattedArguments(view<char8> pattern, const sequence<char8>& indentationString);

			template<typename Argument, typename... PackedArgument>
			void writeFormattedArguments(view<char8> pattern, const sequence<char8>& indentationString, Argument&& argument, PackedArgument&&... arguments) {
				const uint64 argumentInjectionPatternOffset = pattern.find(argumentInjectionPattern, 0);

				if (argumentInjectionPatternOffset > 0) {
					writeIndented(view<char8>(&pattern[0], argumentInjectionPatternOffset), indentationString);
				}

				if (argumentInjectionPatternOffset >= pattern.count()) {
					return;
				}

				const std::string formattedArgument = toString(argument);

				writeIndented(&formattedArgument[0], indentationString);

				const uint64 patternOffset = argumentInjectionPatternOffset + argumentInjectionPattern.count();

				if (patternOffset >= pattern.count()) {
					return;
				}

				pattern = view<char8>(&pattern[patternOffset], pattern.count() - patternOffset);

				writeFormattedArguments(pattern, indentationString, std::forward<PackedArgument>(arguments)...);
			}

			void writeTimestamp() {
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

				std::string formatted = toString(dateTime->tm_hour);

				buffer.padLeft(&formatted[0], 2);
				buffer.jump(1);

				formatted = toString(dateTime->tm_min);

				buffer.padLeft(&formatted[0], 2);
				buffer.jump(1);

				formatted = toString(dateTime->tm_sec);

				buffer.padLeft(&formatted[0], 2);
				buffer.jump(1);

				formatted = toString(us);

				buffer.padLeft(&formatted[0], 6);
			}

		protected:
			LogLevel minLogLevel;
			view<char8> source;
			TextBuffer buffer;
	};
}

#include "scroll/Logger/Logger.ipp"