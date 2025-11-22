// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#include "scroll/LogLevel.hpp"

namespace scroll {
	template<typename T = void>
	class BaseLogger {
		protected:
			static sequence<char8> argumentInjectionPattern;
	};

	class Logger : public BaseLogger<> {
		public:
			// Returns the current argument injection pattern.
			static view<char8> getArgumentInjectionPattern();

			// Replaces the current argument injection pattern.
			static void setArgumentInjectionPattern(view<char8> pattern);

			template<typename Argument>
			static std::string toString(Argument argument);

			static sequence<char8> format(const sequence<char8>& pattern);

			template<typename CurrentArgument, typename... Argument>
			static sequence<char8> format(const sequence<char8>& pattern, CurrentArgument&& argument, Argument&&... arguments) {
				const uint64 argumentInjectionPatternOffset = view<char8>(pattern).find(argumentInjectionPattern, 0);

				if (argumentInjectionPatternOffset >= pattern.count()) {
					return pattern;
				}

				const std::string formattedArgument = toString(argument);

				sequence<char8> updatedPattern(pattern.count() - argumentInjectionPattern.count() + formattedArgument.size());

				copy(&pattern[0], &pattern[argumentInjectionPatternOffset], &updatedPattern[0]);
				copy(formattedArgument.begin(), formattedArgument.end(), &updatedPattern[argumentInjectionPatternOffset]);

				if (argumentInjectionPatternOffset + argumentInjectionPattern.count() < pattern.count()) {
					copy(&pattern[argumentInjectionPatternOffset + argumentInjectionPattern.count()], pattern.end(), &updatedPattern[argumentInjectionPatternOffset + formattedArgument.size()]);
				}

				return format(updatedPattern, std::forward<Argument>(arguments)...);
			}

		private:
			static sequence<char8> createIndentationString(uint64 indentation) {
				sequence<char8> indentationString(indentation);

				for (char8& character : indentationString) {
					character = ' ';
				}

				return indentationString;
			}

		protected:
			static constexpr uint64 MAX_LOG_BUFFER_SIZE = 2048;

		public:
			LogLevel getMinLogLevel() const;

			view<char8> getSource() const;

		protected:
			explicit Logger(LogLevel minLogLevel, view<char8> source);

			void write(char8 character, uint64& offset);
			void write(view<char8> text, uint64& offset);
			void write(view<char8> text, uint64& offset, uint64 padding);

			template<typename... Argument>
			uint64 writeLog(uint64 offset, view<char8> bufferTemplate, view<char8> level, uint64 levelSize, view<char8> pattern, Argument&&... arguments) {
				#ifdef _MSC_VER
					const errno_t error = strncpy_s(&logBuffer[0], bufferTemplate.count() + 1, &bufferTemplate[0], bufferTemplate.count());

					ATL_ASSERT(error == 0);
				#else
					std::strncpy(&logBuffer[0], &bufferTemplate[0], bufferTemplate.count());
				#endif

				uint64 indentation = 18;

				writeTimestamp(offset);

				offset = bufferTemplate.count();

				if (source.count() > 0) {
					write(source, offset);
					write(' ', offset);

					indentation += source.count() + 1;
				}

				write(level, offset);

				indentation += levelSize;

				const sequence<char8> indentationString = createIndentationString(indentation);

				writeFormattedArguments(pattern, offset, indentationString, std::forward<Argument>(arguments)...);

				return offset;
			}

		private:
			uint64 writeIndented(view<char8> pattern, uint64& offset, const sequence<char8>& indentationString) {
				uint64 previousLineOffset = 0;
				uint64 lineOffset = 0;

				while (true) {
					lineOffset = pattern.find('\n', previousLineOffset);

					if (lineOffset == previousLineOffset) {
						break;
					}

					write(view<char8>(&pattern[previousLineOffset], lineOffset - previousLineOffset), offset);

					if (lineOffset >= pattern.count()) {
						break;
					}

					write('\n', offset);
					write(indentationString, offset);

					previousLineOffset = lineOffset + 1;
				}

				return offset;
			}

			uint64 writeFormattedArguments(view<char8> pattern, uint64& offset, const sequence<char8>& indentationString);

			template<typename Argument, typename... PackedArgument>
			uint64 writeFormattedArguments(view<char8> pattern, uint64& offset, const sequence<char8>& indentationString, Argument&& argument, PackedArgument&&... arguments) {
				const uint64 argumentInjectionPatternOffset = pattern.find(argumentInjectionPattern, 0);
				const view<char8> preformatted(&pattern[0], argumentInjectionPatternOffset);

				write(preformatted, offset);

				if (argumentInjectionPatternOffset >= pattern.count()) {
					return offset;
				}

				const std::string formattedArgument = toString(argument);

				writeIndented(&formattedArgument[0], offset, indentationString);

				const uint64 patternOffset = argumentInjectionPatternOffset + argumentInjectionPattern.count();

				if (patternOffset >= pattern.count()) {
					return offset;
				}

				pattern = view<char8>(&pattern[patternOffset], pattern.count() - patternOffset);

				return writeFormattedArguments(pattern, offset, indentationString, std::forward<PackedArgument>(arguments)...);
			}

			void writeTimestamp(uint64& offset) {
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

				write(&formatted[0], offset, 2);

				offset += 1;

				formatted = toString(dateTime->tm_min);

				write(&formatted[0], offset, 2);

				offset += 1;

				formatted = toString(dateTime->tm_sec);

				write(&formatted[0], offset, 2);

				offset += 1;

				formatted = toString(us);

				write(&formatted[0], offset, 6);
			}

		protected:
			LogLevel minLogLevel;
			view<char8> source;
			char8 logBuffer[MAX_LOG_BUFFER_SIZE]{};
	};
}

#include "scroll/Logger/Logger.ipp"