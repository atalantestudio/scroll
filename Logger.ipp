#include "Adapter/Logger.hpp"

namespace ProjectA {
	template<typename Argument>
	str8 Logger::format(Argument&& argument) {
		std::ostringstream stream;

		stream << argument;

		// TODO: Don't use std::string.
		return stream.str();
	}

	template<typename HeadArgument, typename... Argument>
	str8 Logger::format(const str8& pattern, HeadArgument&& headArgument, Argument&&... arguments) {
		const uint64 argumentInjectionPatternOffset = pattern.find(argumentInjectionPattern, 0);

		if (argumentInjectionPatternOffset >= pattern.count()) {
			return pattern;
		}

		const str8 formattedArgument = format(headArgument);

		str8 updatedPattern(pattern.count() - argumentInjectionPattern.count() + formattedArgument.count());

		std::copy_n(&pattern[0], argumentInjectionPatternOffset, &updatedPattern[0]);
		std::copy(formattedArgument.begin(), formattedArgument.end(), &updatedPattern[argumentInjectionPatternOffset]);

		if (argumentInjectionPatternOffset + argumentInjectionPattern.count() < pattern.count()) {
			std::copy(&pattern[argumentInjectionPatternOffset + argumentInjectionPattern.count()], pattern.end(), &updatedPattern[argumentInjectionPatternOffset + formattedArgument.count()]);
		}

		return format(updatedPattern, std::forward<Argument>(arguments)...);
	}
}