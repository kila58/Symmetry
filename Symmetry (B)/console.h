/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
///										  == CLASSES ==
////////////////////////////////////////////////////////////////////////////////////////////////////

enum PrintLevel
{
	LEVEL_GOOD,   // [GREEN*] msg
	LEVEL_BAD,    // [RED*] msg
	LEVEL_WARNING,// [YELLOW*] msg
	LEVEL_DEBUG,  // [YELLOW*] msg (only printed in debug buildmode && sym_debug = 1)
	LEVEL_NORMAL, // [DARK_GREY*] msg
};

// Static console functions
class ConsoleIO
{
public:

	void static Write(PrintLevel, char*, ...);
	void static Clear();

#ifdef BUILDMODE_BETA
	void static SpewErrorF(char*, ...);
#endif
};

////////////////////////////////////////////////////////////////////////////////////////////////////
///										   == MACROS ==
////////////////////////////////////////////////////////////////////////////////////////////////////

// Colour characters
#define CONSOLE_DARK_BLUE	"\x81"
#define CONSOLE_DARK_GREEN	"\x82"
#define CONSOLE_DARK_CYAN	"\x83"
#define CONSOLE_DARK_RED	"\x84"
#define CONSOLE_DARK_PINK	"\x85"
#define CONSOLE_DARK_YELLOW	"\x86"
#define CONSOLE_DARK_GREY	"\x88"

#define CONSOLE_GREY		"\x87"
#define CONSOLE_BLUE		"\x89"
#define CONSOLE_GREEN		"\x8A"
#define CONSOLE_CYAN		"\x8B"
#define CONSOLE_RED			"\x8C"
#define CONSOLE_PINK		"\x8D"
#define CONSOLE_YELLOW		"\x8E"
#define CONSOLE_WHITE		"\x8F"

// Debug macros
#define PrintGood(text, ...) ConsoleIO::Write(LEVEL_GOOD, text, __VA_ARGS__)
#define PrintGoodL(text, ...) ConsoleIO::Write(LEVEL_GOOD, text, __VA_ARGS__)

#define PrintBad(text, ...) ConsoleIO::Write(LEVEL_BAD, text, __VA_ARGS__)
#define PrintBadL(text, ...) ConsoleIO::Write(LEVEL_BAD, text, __VA_ARGS__)

#define PrintWarning(text, ...) ConsoleIO::Write(LEVEL_WARNING, text, __VA_ARGS__)
#define PrintWarningL(text, ...) ConsoleIO::Write(LEVEL_WARNING, text, __VA_ARGS__)

#define PrintLine(text, ...) ConsoleIO::Write(LEVEL_NORMAL, text, __VA_ARGS__)

#ifdef BUILDMODE_BETA

#define PrintDebug(text, ...) ConsoleIO::Write(LEVEL_DEBUG, text, __VA_ARGS__)
#define PrintDebugL(text, ...) ConsoleIO::Write(LEVEL_DEBUG, text, __VA_ARGS__)
#else
#define PrintDebug(text, ...) ((void)0)
#define PrintDebugL(text, ...) ((void)0)
#endif

#ifdef BUILDMODE_BETA
#define SpewError(str, ...) ConsoleIO::SpewErrorF(str, __VA_ARGS__)
#else
#define SpewError(str, ...) ((void)0)
#endif