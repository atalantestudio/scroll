// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#include "scroll/ConsoleEscapeCode.hpp"
#include "scroll/Logger/Logger.hpp"
#include "scroll/LogLevel.hpp"

namespace scroll {
	class ConsoleLogger : public Logger {
		public:
			explicit ConsoleLogger(std::ostream& stream, LogLevel minLogLevel, view<char8> source);

			std::ostream& getOutputStream();

			void setOutputStream(std::ostream& stream);

			template<typename Argument>
			ConsoleLogger& operator<<(Argument&& argument);

			ConsoleLogger& padLeft(uint64 padding);

			ConsoleLogger& padRight(uint64 padding);

			template<typename Argument>
			void trace(view<char8> file, uint64 line, Argument&& argument);

			template<typename... Argument>
			void trace(view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments);

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
			#if ATL_OPERATING_SYSTEM == ATL_OPERATING_SYSTEM_WINDOWS
				void setConsoleMode(DWORD nStdHandle) {
					const HANDLE hConsoleHandle = GetStdHandle(nStdHandle);

					ATL_ASSERT(hConsoleHandle != NULL && hConsoleHandle != INVALID_HANDLE_VALUE);

					BOOL result = FALSE;
					DWORD mode;

					result = GetConsoleMode(hConsoleHandle, &mode);

					if (result == 0) {
						redirected = true;

						return;
					}

					result = SetConsoleMode(hConsoleHandle, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

					ATL_ASSERT(result > 0);
				}
			#endif

			template<typename... Argument>
			uint64 writeConsoleLog(view<char8> level, uint64 levelSize, view<char8> pattern, Argument&&... arguments);

			template<typename... Argument>
			void writeTraceLog(view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments);

			template<typename... Argument>
			void writeDebugLog(view<char8> pattern, Argument&&... arguments);

			template<typename... Argument>
			void writeInfoLog(view<char8> pattern, Argument&&... arguments);

			template<typename... Argument>
			void writeWarningLog(view<char8> pattern, Argument&&... arguments);

			template<typename... Argument>
			void writeErrorLog(view<char8> function, view<char8> file, uint64 line, view<char8> pattern, Argument&&... arguments);

			void flushLog(uint64 size) const;

		private:
			std::ostream* stream;
			bool redirected = false;
			bool writingEscapeCodes = false;
	};
}

#include "scroll/ConsoleLogger/ConsoleLogger.ipp"