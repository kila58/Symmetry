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

#include "windows_headers.h"
#include "namespaces.h"
#include "function.h"
#include "detours.h"

// Depracted, I now use MinHook because it's free and supports
// trampolines.  Something I didn't think I'd need until late in
// the project when I found that my method was very slow, too
// slow for the application.

/*
NAMESPACE_LIB(Detours)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// JMP (0xE9) detours.
/// 
/// This class detours a single function by editing the first 5 bytes of a function, 
/// adding a 0xE9 instruction (JMP) and a 4byte pointer to an address in memory.  
/// 
/// When the function is called the EIP will jump to the new function.
/// 
/// This class supports a single function detour.  This class is depracted and is likely detected.
////////////////////////////////////////////////////////////////////////////////////////////////////

class JMP : DetourBase
{
private:

	// We can't store a pointer to the original function like with VMT hook because
	// we're modifying the "original" function directly, so we must back-up bytes
	// that get replaced
	uchar*			m_cOrigBytes;

	// Addresses to the original function and jump-to function (using function wrappers)
	Function*		m_fOrigFunc;
	Function*		m_fNewFunc;

public:

	// Constructors and destructor
	JMP(Function*);
	JMP(dword*);

	~JMP();

	// Sets m_dwpNewFunc and calls ReHook
	void		Hook(Function*);
	void		Hook(dword*);

	// Un-hooks and re-hooks a JMP detour, Hook() must be called before any of these two functions
	// can be used.
	void		UnHook();
	void		ReHook();

	// Overloaded from base
	dword*		GetFunctionAddress();
	
	// Gets the function that's being detoured
	Function*	GetFunction();

};

NAMESPACE_END3

*/