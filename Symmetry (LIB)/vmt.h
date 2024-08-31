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
#include "function.h"
#include "detours.h"

NAMESPACE_LIB(Detours)

////////////////////////////////////////////////////////////////////////////////////////////////////
///	Virtual method detours.
///	
/// The method requires a pointer to a virtual classes virtual method table, detours are done
/// there by replacing the entire virtual method table with another.  Entries in the new VMT
/// are replaced with new pointers to redirect the flow of a function call to another
/// same-signatued function.
/// 
/// Unhooking will unhook all functions, it's important rehooking is done afterwards.
/// 
/// This class supports a single detour for every index in the virtual method table.
////////////////////////////////////////////////////////////////////////////////////////////////////

class VMT : DetourBase
{
private:

	// Double pointer to original vmt base
	PDWORD*	m_ppdwVMTBase;

	// New and old VMT tables
	PDWORD	m_pdwNew, m_pdwOld;

	// Number of VMT entries (cached GetMethodCount)
	dword	m_dwSize;

public:

	// Constructor and destructor
	VMT(PDWORD*);
	~VMT();

	// Detours a virtual method at base + uiIndex
	dword		Hook(dword, uint32 uiIndex);

	// Unhooks all detours
	void		UnHook();

	// Rehooks all detours
	void		ReHook();

	// Returns method address
	dword*		GetMethod(uint32);

	// Returns the number of functions in the VMT
	uint32		GetMethodCount();

};

NAMESPACE_END2