#pragma once

#include "base.hpp"
#include "ConsoleEscapeCode.hpp"
#include "Logger/Logger.hpp"
#include "LogLevel.hpp"

namespace ProjectA {
	class ConsoleLogger : public Logger {
		private:
			template<typename... Argument>
			static void writeStreamEscapeCodes(std::ostream& stream, Argument&&... codes);

			template<typename... Argument>
			static void writeStreamEscapeCodesInternal(std::ostream& stream, ConsoleEscapeCode code, Argument&&... codes);

			static void writeStreamEscapeCodesInternal(std::ostream& stream, ConsoleEscapeCode code);

			static uint16 getConsoleWidth();

		private:
			static void writeLogHeader(std::ostream& stream, const str8& source, const str8& levelName, ConsoleEscapeCode levelBackgroundColor, ConsoleEscapeCode levelForegroundColor);

		public:
			explicit ConsoleLogger(std::ostream& stream, LogLevel minLogLevel, const str8& source);

			std::ostream& getOutputStream();

			void setOutputStream(std::ostream& stream);

			template<typename... Code>
			void writeEscapeCodes(Code&&... codes);

			// TODO: Move into Logger?
			void write(const str8& text);

			// TODO: Move into Logger?
			void prefixPrint(const str8& prefix, const str8& text);

			void title(const str8& text);

			void comment(const str8& text);

			void writeLink(const str8& url);
			void writeLink(const str8& url, const str8& placeholder);

			void listItem(const str8& text);

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
			std::ostream* stream;
	};
}

#include "ConsoleLogger.ipp"