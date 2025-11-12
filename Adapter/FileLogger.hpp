#pragma once

#include "Logger.hpp"
#include "LogLevel.hpp"

namespace ProjectA {
	class FileLogger : public Logger {
		public:
			explicit FileLogger(std::ofstream& stream, LogLevel minLogLevel, const str8& source);

			template<typename Argument>
			void trace(const str8& file, uint32 line, Argument&& argument);

			template<typename... Argument>
			void trace(const str8& file, uint32 line, const str8& pattern, Argument&&... arguments);

			template<typename Argument>
			void debug(Argument&& argument);

			template<typename... Argument>
			void debug(const str8& pattern, Argument&&... arguments);

			template<typename Argument>
			void info(Argument&& argument);

			template<typename... Argument>
			void info(const str8& pattern, Argument&&... arguments);

			template<typename Argument>
			void warning(Argument&& argument);

			template<typename... Argument>
			void warning(const str8& pattern, Argument&&... arguments);

			template<typename Argument>
			void error(const str8& function, const str8& file, uint64 line, Argument&& argument);

			template<typename... Argument>
			void error(const str8& function, const str8& file, uint64 line, const str8& pattern, Argument&&... arguments);

		private:
			void writeLogHeader(const str8& levelName);

		public:
			std::ofstream& stream;
	};
}

#include "FileLogger.ipp"