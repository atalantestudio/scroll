#pragma once

#include "base.hpp"
#include "ConsoleEscapeCode.hpp"
#include "Logger/Logger.hpp"
#include "LogLevel.hpp"

namespace USER_NAMESPACE {
	class ConsoleLogger : public Logger {
		private:
			template<typename... Argument>
			static void writeStreamEscapeCodes(std::ostream& stream, Argument&&... codes);

			template<typename... Argument>
			static void writeStreamEscapeCodesInternal(std::ostream& stream, ConsoleEscapeCode code, Argument&&... codes);

			static void writeStreamEscapeCodesInternal(std::ostream& stream, ConsoleEscapeCode code);

		private:
			static void writeLogHeader(std::ostream& stream, view<char8> source, view<char8> levelName, ConsoleEscapeCode levelBackgroundColor, ConsoleEscapeCode levelForegroundColor);

		public:
			explicit ConsoleLogger(std::ostream& stream, LogLevel minLogLevel, view<char8> source);

			std::ostream& getOutputStream();

			void setOutputStream(std::ostream& stream);

			template<typename... Code>
			void writeEscapeCodes(Code&&... codes);

			// TODO: Move into Logger?
			void write(view<char8> text);

			void title(view<char8> text);

			void writeLink(view<char8> url);
			void writeLink(view<char8> url, view<char8> placeholder);

			void listItem(view<char8> text);

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
	};
}

#include "ConsoleLogger.ipp"