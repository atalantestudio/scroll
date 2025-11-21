// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#include "scroll/Logger/Logger.hpp"
#include "scroll/LogLevel.hpp"

namespace scroll {
	class FileLogger : public Logger {
		public:
			explicit FileLogger(std::ofstream& stream, LogLevel minLogLevel, view<char8> source);

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
			void writeLogHeader(view<char8> levelName);

		public:
			std::ofstream& stream;
	};
}

#include "scroll/FileLogger/FileLogger.ipp"