// Copyright 2025 Atalante.
// Licensed under MIT.

#pragma once

#include "ConsoleEscapeCode.hpp"

namespace USER_NAMESPACE {
	std::ostream& operator<<(std::ostream& stream, view<char8> view);
	std::ostream& operator<<(std::ostream& stream, ConsoleEscapeCode code);
}

#include "Conversions.ipp"