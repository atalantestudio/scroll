// Copyright 2025 Atalante Studio.
// Distributed under the MIT License.

#pragma once

#include "scroll/ConsoleEscapeCode.hpp"

namespace scroll {
	std::ostream& operator<<(std::ostream& stream, view<char8> view);
	std::ostream& operator<<(std::ostream& stream, ConsoleEscapeCode code);
}

#include "scroll/Conversions.ipp"