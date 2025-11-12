namespace ProjectA {
	template<typename Argument>
	void FileLogger::trace(const str8& file, uint32 line, Argument&& argument) {
		writeLogHeader("TRACE");
		writeIndented(stream, format("[]\nat []:[]", format(std::forward<Argument>(argument)), file, line), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::trace(const str8& file, uint32 line, const str8& pattern, Argument&&... arguments) {
		writeLogHeader("TRACE");
		writeIndented(stream, format("[]\nat []:[]", format(pattern, std::forward<Argument>(arguments)...), file, line), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename Argument>
	void FileLogger::debug(Argument&& argument) {
		writeLogHeader("DEBUG");
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::debug(const str8& pattern, Argument&&... arguments) {
		writeLogHeader("DEBUG");
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename Argument>
	void FileLogger::info(Argument&& argument) {
		writeLogHeader("INFO");
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 5);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::info(const str8& pattern, Argument&&... arguments) {
		writeLogHeader("INFO");
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 5);

		stream << '\n';
	}

	template<typename Argument>
	void FileLogger::warning(Argument&& argument) {
		writeLogHeader("WARNING");
		writeIndented(stream, format(std::forward<Argument>(argument)), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::warning(const str8& pattern, Argument&&... arguments) {
		writeLogHeader("WARNING");
		writeIndented(stream, format(pattern, std::forward<Argument>(arguments)...), getLogIndentation() - 2);

		stream << '\n';
	}

	template<typename Argument>
	void FileLogger::error(const str8& function, const str8& file, uint64 line, Argument&& argument) {
		writeLogHeader("ERROR");
		writeIndented(stream, format("[]\nat [] ([]:[])", format(std::forward<Argument>(argument)), function, file, line), getLogIndentation() - 4);

		stream << '\n';
	}

	template<typename... Argument>
	void FileLogger::error(const str8& function, const str8& file, uint64 line, const str8& pattern, Argument&&... arguments) {
		writeLogHeader("ERROR");
		writeIndented(stream, format("[]\nat [] ([]:[])", format(pattern, std::forward<Argument>(arguments)...), function, file, line), getLogIndentation() - 4);

		stream << '\n';
	}
}