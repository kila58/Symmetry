/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "global.h"

#include "symconvar.h"

#ifdef BUILDMODE_BETA
SymConVar<short> sym_debug("sym_debug", "Print debug messages", 1);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ConsoleIO::Write(bool bNewLine, char* cText, ...)
///
/// \brief	Sends text via IPC to the injector, the text is written to the injector's console.
///
/// \param	bNewLine	 	True to print a new line after the text.
/// \param [in,out]	cText	The text to write.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleIO::Write(PrintLevel eLevel, char* cText, ...)
{
	if (eLevel == LEVEL_DEBUG)
	{
#ifndef BUILDMODE_BETA
		return;
#else
		if (!sym_debug.GetValue())
			return;
#endif
	}

	char buf[0xFFF];
	va_list args;

	va_start(args, cText);
		int size = vsnprintf_s(buf, sizeof(buf), cText, args);
	va_end(args);

	// Send 
	if (Class::Network)
		Class::Network->Send(Lib::IO::IpcPrint((int)eLevel, buf));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ConsoleIO::Clear()
///
/// \brief	Clears the console, the actual clearing is done by the injector itself.  This just
/// 		calls the injector's version of this function using IPC.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleIO::Clear()
{
	// Send clear IPC message
	if (Class::Network)
		Class::Network->Send(Lib::IO::IpcClear());
}

#ifdef BUILDMODE_BETA

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ConsoleIO::SpewErrorF()
///
/// \brief	Spews an error using MessageBox, used for debugging purposes only.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleIO::SpewErrorF(char* cText, ...)
{
	char buf[0xFF];
	va_list args;

	va_start(args, cText);
	int size = vsnprintf_s(buf, sizeof(buf), cText, args);
	va_end(args);

	MessageBox(0, buf, "Symmetry Debug Error", 0);
}

#endif