#pragma once

#include "base.hpp"
#include "LogLevel.hpp"

namespace ProjectA {
	class Logger {
		public:
			// Returns the current argument injection pattern.
			static const str8& getArgumentInjectionPattern();

			// Replaces the current argument injection pattern.
			static void setArgumentInjectionPattern(const str8& pattern);

			template<typename Argument>
			static str8 format(Argument&& argument);

			template<typename HeadArgument, typename... Argument>
			static str8 format(const str8& pattern, HeadArgument&& headArgument, Argument&&... arguments);

			static void writeTimestamp(std::ostream& stream);

		protected:
			static constexpr std::streamsize TIMESTAMP_SIZE = 14;
			static constexpr std::streamsize MAX_LOG_LEVEL_NAME_SIZE = 7;

		protected:
			static str8 argumentInjectionPattern;

		protected:
			explicit Logger(LogLevel minLogLevel, const str8& source);

			uint16 getLogIndentation() const;

			static void write(std::ostream& stream, const str8& text);

			// NOTE: `indentation` must not include the size of `text`.
			static void writeIndented(std::ostream& stream, const str8& text, uint64 indentation);

		protected:
			LogLevel minLogLevel;
			str8 source;
	};
}

#include "Logger.ipp"