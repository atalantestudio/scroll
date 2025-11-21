// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#include "scroll/ConsoleEscapeCode.hpp"

namespace scroll {
	std::ostream& operator<<(std::ostream& stream, view<char8> view);
	std::ostream& operator<<(std::ostream& stream, ConsoleEscapeCode code);
}

#include "scroll/Conversions.ipp"