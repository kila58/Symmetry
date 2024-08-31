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
#include "generic.h"

// Signals sent from C++ to C#
enum SignalType
{
	SIGNAL_PRINT_GOOD,
	SIGNAL_PRINT_BAD,
	SIGNAL_PRINT_WARNING,
	SIGNAL_PRINT_DEBUG,
	SIGNAL_PRINT_NORMAL,
	SIGNAL_CLEAR,
	SIGNAL_ADDCOMMAND,
	SIGNAL_REMCOMMAND,
	SIGNAL_SUBEND,
};

// Typedef for C# SendSignal function
typedef void (__stdcall* SendSignal_t)(SignalType, const char*, int);

NAMESPACE_LIB(Signal)

void Send(SignalType, const char* cArg = "", int iLength = 0);

NAMESPACE_END2