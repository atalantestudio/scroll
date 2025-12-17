// Copyright 2025 Atalante Studio.
// Distributed under the MIT License.

namespace scroll {
	inline std::ostream& operator<<(std::ostream& stream, ConsoleEscapeCode code) {
		return stream << static_cast<uint16>(code);
	}
}