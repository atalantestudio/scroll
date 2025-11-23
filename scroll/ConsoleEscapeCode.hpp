// Copyright 2025 Atalante Studio.
// Distributed under the MIT License.

#pragma once

namespace scroll {
	// See https://en.wikipedia.org/wiki/ANSI_escape_code#Select_Graphic_Rendition_parameters.
	enum class ConsoleEscapeCode : char8 {
		// All attributes become turned off.
		RESET_ALL = 0,

		// NOTE: May be implemented as a bold font weight.
		BRIGHT = 1,

		// NOTE: May be implemented as a thin font weight.
		DIMMED = 2,

		// Sometimes treated as BLINK or NEGATIVE.
		// NOTE: Not widely supported.
		ITALIC = 3,

		UNDERLINE = 4,

		// Sets blinking to less than 150 times per minute.
		SLOW_BLINK = 5,

		// Sets blinking to 150 times per minute or more.
		// NOTE: Not widely supported.
		FAST_BLINK = 6,

		// Swaps foreground and background colors.
		// NOTE: Inconsistent emulation.
		NEGATIVE = 7,

		// NOTE: Not widely supported.
		INVISIBLE = 8,

		// Characters legible but marked as if for deletion.
		// NOTE: Not supported in Terminal.app.
		STRIKETHROUGH = 9,

		RESET_FONT = 10,

		// Selects alternative font n - 10.
		ALTERNATIVE_FONT_0 = 11,
		ALTERNATIVE_FONT_1 = 12,
		ALTERNATIVE_FONT_2 = 13,
		ALTERNATIVE_FONT_3 = 14,
		ALTERNATIVE_FONT_4 = 15,
		ALTERNATIVE_FONT_5 = 16,
		ALTERNATIVE_FONT_6 = 17,
		ALTERNATIVE_FONT_7 = 18,
		ALTERNATIVE_FONT_8 = 19,

		// NOTE: Rarely supported.
		ALTERNATIVE_FONT_FRAKTUR = 20,

		// NOTE: Treated as RESET_BRIGHT on several terminals.
		DOUBLE_UNDERLINE = 21,

		RESET_BRIGHT = 22,

		// Also resets blackletter fonts.
		RESET_ITALIC = 23,

		// Also resets double underline.
		RESET_UNDERLINE = 24,

		RESET_BLINK = 25,

		// NOTE: Not known to be used on terminals.
		PROPORTIONAL_SPACING = 26,

		RESET_NEGATIVE = 27,
		RESET_INVISIBLE = 28,
		RESET_STRIKETHROUGH = 29,

		// Sets foreground color.
		FOREGROUND_COLOR_BLACK = 30,
		FOREGROUND_COLOR_RED = 31,
		FOREGROUND_COLOR_GREEN = 32,
		FOREGROUND_COLOR_YELLOW = 33,
		FOREGROUND_COLOR_BLUE = 34,
		FOREGROUND_COLOR_MAGENTA = 35,
		FOREGROUND_COLOR_CYAN = 36,
		FOREGROUND_COLOR_LIGHT_GRAY = 37,

		// Next arguments are either 5;n or 2;r;g;b.
		FOREGROUND_COLOR_N_OR_RGB = 38,

		// NOTE: Implementation defined (according to standard).
		RESET_FOREGROUND_COLOR = 39,

		// Sets background color.
		BACKGROUND_COLOR_BLACK = 40,
		BACKGROUND_COLOR_RED = 41,
		BACKGROUND_COLOR_GREEN = 42,
		BACKGROUND_COLOR_YELLOW = 43,
		BACKGROUND_COLOR_BLUE = 44,
		BACKGROUND_COLOR_MAGENTA = 45,
		BACKGROUND_COLOR_CYAN = 46,
		BACKGROUND_COLOR_LIGHT_GRAY = 47,

		// Next arguments are either 5;n or 2;r;g;b.
		BACKGROUND_COLOR_N_OR_RGB = 48,

		// NOTE: Implementation defined (according to standard).
		RESET_BACKGROUND_COLOR = 49,

		RESET_PROPORTIONAL_SPACING = 50,

		// NOTE: Implemented as [emoji variation selector](https://en.wikipedia.org/wiki/Variation_Selectors_(Unicode_block)) in mintty.
		FRAMED = 51,
		ENCIRCLED = 52,

		// NOTE: Not supported in Terminal.app.
		OVERLINE = 53,

		RESET_FRAMED_AND_ENCIRCLED = 54,

		RESET_OVERLINE = 55,

		// Next arguments are either 5;n or 2;r;g;b.
		// NOTE: Not in standard. Implemented in Kitty, VTE, mintty and iTerm2.
		UNDERLINE_COLOR_N_OR_RGB = 58,

		// NOTE: Not in standard. Implemented in Kitty, VTE, mintty and iTerm2.
		RESET_UNDERLINE_COLOR = 59,

		// NOTE: Rarely supported.
		IDEOGRAM_UNDERLINE_OR_RIGHT_SIDE_LINE = 60,
		IDEOGRAM_DOUBLE_UNDERLINE_OR_DOUBLE_RIGHT_SIDE_LINE = 61,
		IDEOGRAM_OVERLINE_OR_LEFT_SIDE_LINE = 62,
		IDEOGRAM_DOUBLE_OVERLINE_OR_DOUBLE_LEFT_SIDE_LINE = 63,
		IDEOGRAM_STRESS_MARKING = 64,

		// Resets all IDEOGRAM_* values.
		RESET_IDEOGRAM = 65,

		// NOTE: Implemented in mintty.
		SUPERSCRIPT = 73,
		SUBSCRIPT = 74,
		RESET_SUPERSCRIPT_AND_SUBSCRIPT = 75,

		// Sets bright foreground color.
		// NOTE: Not in standard. Originally implemented by aixterm.
		FOREGROUND_COLOR_GRAY = 90,
		FOREGROUND_COLOR_LIGHT_RED = 91,
		FOREGROUND_COLOR_LIGHT_GREEN = 92,
		FOREGROUND_COLOR_LIGHT_YELLOW = 93,
		FOREGROUND_COLOR_LIGHT_BLUE = 94,
		FOREGROUND_COLOR_LIGHT_MAGENTA = 95,
		FOREGROUND_COLOR_LIGHT_CYAN = 96,
		FOREGROUND_COLOR_WHITE = 97,

		// Sets bright background color.
		// NOTE: Not in standard. Originally implemented by aixterm.
		BACKGROUND_COLOR_GRAY = 100,
		BACKGROUND_COLOR_LIGHT_RED = 101,
		BACKGROUND_COLOR_LIGHT_GREEN = 102,
		BACKGROUND_COLOR_LIGHT_YELLOW = 103,
		BACKGROUND_COLOR_LIGHT_BLUE = 104,
		BACKGROUND_COLOR_LIGHT_MAGENTA = 105,
		BACKGROUND_COLOR_LIGHT_CYAN = 106,
		BACKGROUND_COLOR_WHITE = 107,
	};
}