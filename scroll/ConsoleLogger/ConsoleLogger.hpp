// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#include "scroll/ConsoleEscapeCode.hpp"
#include "scroll/Logger/Logger.hpp"
#include "scroll/LogLevel.hpp"

namespace scroll {
	class ConsoleLogger : public Logger {
		private:
			static void writeLogHeader(std::ostream& stream, view<char8> source, view<char8> levelName, ConsoleEscapeCode levelBackgroundColor, ConsoleEscapeCode levelForegroundColor);

		public:
			explicit ConsoleLogger(std::ostream& stream, LogLevel minLogLevel, view<char8> source);

			~ConsoleLogger();

			std::ostream& getOutputStream();

			void setOutputStream(std::ostream& stream);

			template<typename Argument>
			ConsoleLogger& operator<<(Argument&& argument);

			ConsoleLogger& padLeft(uint64 padding);

			ConsoleLogger& padRight(uint64 padding);

			template<typename Argument>
			void trace(view<char8> file, uint32 line, Argument&& argument);

			template<typename... Argument>
			void trace(view<char8> file, uint32 line, view<char8> pattern, Argument&&... arguments);

			template<typename Argument>
			void debug(Argument&& argument);

			template<typename... Argument>
			void debug(view<char8> pattern, Argument&&... arguments);

			template<typename Argument>
			void info(Argument&& argument);

			template<typename... Argument>
			void info(view<char8> pattern, Argument&&... arguments);

			template<typename Argument>
			void warning(Argument&& argument);

			template<typename... Argument>
			void warning(view<char8> pattern, Argument&&... arguments);

			template<typename Argument>
			void error(view<char8> function, view<char8> file, uint64 line, Argument&& argument);

			template<typename... Argument>
			void error(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments);

		private:
			std::ostream* stream;
			bool writingEscapeCodes = false;
	};
}

#include "scroll/ConsoleLogger/ConsoleLogger.ipp"