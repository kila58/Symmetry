/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "jmp.h"

// Depracted, I now use MinHook because it's free and supports
// trampolines.  Something I didn't think I'd need until late in
// the project when I found that my method was very slow, too
// slow for the application.

/*
NAMESPACE_LIB(Detours)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn JMP::JMP(Function* fFunc)
///
/// \brief Constructor.
///
/// \param fFunc Target function.
////////////////////////////////////////////////////////////////////////////////////////////////////

JMP::JMP(Function* fFunc)
{
	this->m_fOrigFunc	= fFunc;
	this->m_cOrigBytes	= 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn JMP::JMP(uint32 iFunc)
///
/// \brief Constructor.
///
/// \param wFunc Target function.
////////////////////////////////////////////////////////////////////////////////////////////////////

JMP::JMP(dword* wpFunc)
{
	this->m_fOrigFunc	= new Function(wpFunc, true);
	this->m_cOrigBytes	= 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn JMP::~JMP()
///
/// \brief Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

JMP::~JMP()
{
	this->UnHook();
	
	delete this->m_fNewFunc;
	delete this->m_fOrigFunc;
	delete this->m_cOrigBytes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn dword* JMP::GetFunctionAddress()
///
/// \brief Gets the detoured function's address.
///
/// \return The detoured function's address.
////////////////////////////////////////////////////////////////////////////////////////////////////

dword* JMP::GetFunctionAddress()
{
	return this->m_fOrigFunc->GetReference();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool JMP::Hook()
///
/// \brief Sets the callback for this hook.  fNewFunc will be called INSTEAD of m_fOrigFunc.
////////////////////////////////////////////////////////////////////////////////////////////////////

void JMP::Hook(Function* fNewFunc)
{
	this->m_fNewFunc = fNewFunc;
	this->ReHook();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void JMP::Hook(uint32 iFunc)
///
/// \brief Helper function.  Calls above constructing a function from a dword (dword).
////////////////////////////////////////////////////////////////////////////////////////////////////

void JMP::Hook(dword* wpFunc)
{
	this->Hook(new Function(wpFunc, true));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void JMP::UnHook()
///
/// \brief Restores the original stored bytes in the detoured function.  This function has no
/// 	safe guards, if the function was never detoured it will still be "restored".
////////////////////////////////////////////////////////////////////////////////////////////////////

void JMP::UnHook()
{
	if (this->m_cOrigBytes == NULL) return;

	this->m_fOrigFunc->ProtectRegion(PAGE_EXECUTE_READWRITE);
		this->m_fOrigFunc->SetMemory( 5, this->m_cOrigBytes );
	this->m_fOrigFunc->RestoreRegion();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void JMP::ReHook()
///
/// \brief Hooks the function again.  Actual memory modification is done in here.
////////////////////////////////////////////////////////////////////////////////////////////////////

void JMP::ReHook()
{
	dword wAddr = (dword)this->m_fNewFunc->GetReference() - (dword)this->m_fOrigFunc->GetReference() - 5;

	uchar JMP[] = { 0xE9, 0x90, 0x90, 0x90, 0x90 };
	memcpy(&JMP[1], &wAddr, 4);

	this->m_fOrigFunc->ProtectRegion(PAGE_EXECUTE_READWRITE);
	
		this->m_cOrigBytes = (uchar*)this->m_fOrigFunc->GetMemory(5);
		this->m_fOrigFunc->SetMemory( 5, JMP );

	this->m_fOrigFunc->RestoreRegion();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn Function* JMP::GetFunction()
///
/// \brief Returns the original function that was passed in through the constructor.
/// 	   If the hook's still active, expect a function with modified memory to be returned.
///
/// \return .
////////////////////////////////////////////////////////////////////////////////////////////////////

Function* JMP::GetFunction()
{
	return this->m_fOrigFunc;
}

NAMESPACE_END3

*/