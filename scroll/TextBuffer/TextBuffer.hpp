#pragma once

namespace scroll {
	template<typename Argument>
	std::string toString(Argument argument) {
		// NOTE: Being here means that a toString() specialization for `argument` was not provided.
		return "\xEF\xBF\xBD";
	}

	class TextBuffer {
		protected:
			static constexpr uint64 MAX_LOG_BUFFER_SIZE = 4096;

		public:
			sequence<char8> getText() const;

			void jump(uint64 offset);
			void seek(uint64 offset);

			template<typename Argument>
			TextBuffer& operator<<(Argument argument);

			TextBuffer& operator<<(char8 argument);
			TextBuffer& operator<<(const sequence<char8>& argument);
			TextBuffer& operator<<(const std::string& argument);
			TextBuffer& operator<<(char8* argument);
			TextBuffer& operator<<(const char8* argument);
			TextBuffer& operator<<(view<char8> argument);

			#if ATL_STANDARD >= ATL_STANDARD_CPP17
				TextBuffer& operator<<(std::string_view argument);
			#endif

			TextBuffer& padLeft(view<char8> text, uint64 padding);
			TextBuffer& padRight(view<char8> text, uint64 padding);

			void flush(std::ostream& stream);

			void clear();

		public:
			char8 buffer[MAX_LOG_BUFFER_SIZE]{};
			uint64 offset = 0;
	};
}

#include "scroll/TextBuffer/TextBuffer.ipp"