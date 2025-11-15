#pragma once

#include "ConsoleEscapeCode.hpp"

namespace USER_NAMESPACE {
	std::ostream& operator <<(std::ostream& stream, view<char8> view);
	std::ostream& operator <<(std::ostream& stream, ConsoleEscapeCode code);
}

#include "Conversions.ipp"