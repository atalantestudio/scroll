// Copyright 2025 Atalante Studio.
// Distributed under the MIT License.

namespace scroll {
	inline std::ostream& operator<<(std::ostream& stream, view<char8> view) {
		return stream.write(&view[0], view.count());
	}

	inline std::ostream& operator<<(std::ostream& stream, const sequence<char8>& sequence) {
		return stream << &sequence[0];
	}

	inline std::ostream& operator<<(std::ostream& stream, ConsoleEscapeCode code) {
		return stream << static_cast<uint16>(code);
	}
}