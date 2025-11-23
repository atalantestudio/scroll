namespace scroll {
	template<>
	inline std::string toString(bool argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(uint8 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(uint16 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(uint32 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(uint64 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(sint8 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(sint16 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(sint32 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(sint64 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(char8 argument) {
		return std::string(1, argument);
	}

	template<>
	inline std::string toString(float32 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(float64 argument) {
		return std::to_string(argument);
	}

	template<>
	inline std::string toString(const char8* argument) {
		return argument;
	}

	template<>
	inline std::string toString(const sequence<char8>& argument) {
		return std::string(&argument[0], argument.count());
	}

	template<>
	inline std::string toString(view<char8> argument) {
		return std::string(&argument[0], argument.count());
	}

	template<>
	inline std::string toString(const std::string& argument) {
		return argument;
	}

	inline void TextBuffer::jump(uint64 offset) {
		ATL_ASSERT(this->offset + offset < MAX_LOG_BUFFER_SIZE);

		this->offset += offset;
	}

	inline void TextBuffer::seek(uint64 offset) {
		ATL_ASSERT(offset < MAX_LOG_BUFFER_SIZE);

		this->offset = offset;
	}

	template<typename Argument>
	inline TextBuffer& TextBuffer::operator<<(Argument argument) {
		const std::string string = toString(argument);

		return operator<<(view<char8>(&string[0], string.size()));
	}

	template<>
	inline TextBuffer& TextBuffer::operator<<(char8 argument) {
		buffer[offset] = argument;

		jump(1);

		return *this;
	}

	template<>
	inline TextBuffer& TextBuffer::operator<<(view<char8> argument) {
		copy(argument.begin(), argument.end(), &buffer[offset]);
		jump(argument.count());

		return *this;
	}

	template<>
	inline TextBuffer& TextBuffer::operator<<(sequence<char8> argument) {
		return operator<<(view<char8>(&argument[0], argument.count()));
	}

	inline TextBuffer& TextBuffer::padLeft(view<char8> text, uint64 padding) {
		offset += ATL_MAX(0, padding - text.count());

		return operator<<(text);
	}

	inline TextBuffer& TextBuffer::padRight(view<char8> text, uint64 padding) {
		TextBuffer& textBuffer = operator<<(text);

		offset += ATL_MAX(0, padding - text.count());

		return textBuffer;
	}

	inline void TextBuffer::flush(std::ostream& stream) {
		stream.write(&buffer[0], offset);

		clear();
	}

	inline void TextBuffer::clear() {
		offset = 0;
	}
}