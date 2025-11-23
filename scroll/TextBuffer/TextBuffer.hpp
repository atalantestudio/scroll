#pragma once

namespace scroll {
	template<typename Argument>
	std::string toString(Argument argument) {
		// should never be here
		ATL_ASSERT(false);

		return "";
	}

	class TextBuffer {
		protected:
			static constexpr uint64 MAX_LOG_BUFFER_SIZE = 2048;

		public:
			void jump(uint64 offset);
			void seek(uint64 offset);

			template<typename Argument>
			TextBuffer& operator<<(Argument argument);

			TextBuffer& padLeft(view<char8> text, uint64 padding);
			TextBuffer& padRight(view<char8> text, uint64 padding);

			void clear();

		public:
			char8 buffer[MAX_LOG_BUFFER_SIZE]{};
			uint64 offset = 0;
	};
}

#include "scroll/TextBuffer/TextBuffer.ipp"