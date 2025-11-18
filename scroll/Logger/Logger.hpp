// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#include "LogLevel.hpp"

namespace USER_NAMESPACE {
	class Logger {
		public:
			// Returns the current argument injection pattern.
			static view<char8> getArgumentInjectionPattern();

			// Replaces the current argument injection pattern.
			static void setArgumentInjectionPattern(view<char8> pattern);

			template<typename Argument>
			static sequence<char8> format(Argument&& argument);

			template<typename HeadArgument, typename... Argument>
			static sequence<char8> format(view<char8> pattern, HeadArgument&& headArgument, Argument&&... arguments);

			static sequence<char8> timestamp();

		protected:
			// NOTE: `indentation` must **not** include the size of `text`.
			static void writeIndented(std::ostream& stream, view<char8> text, uint64 indentation);

		private:
			static sequence<char8> argumentInjectionPattern;

			static constexpr uint64 TIMESTAMP_SIZE = 14;
			static constexpr uint64 MAX_LOG_LEVEL_NAME_SIZE = 7;

		public:
			LogLevel getMinLogLevel() const;

			view<char8> getSource() const;

		protected:
			explicit Logger(LogLevel minLogLevel, view<char8> source);

			uint16 getLogIndentation() const;

		protected:
			LogLevel minLogLevel;
			view<char8> source;
	};
}

#include "Logger.ipp"