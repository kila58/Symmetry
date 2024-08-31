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

#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL

// Bool casting performance warning - can't do a lot about this in a templated method
#pragma warning(disable:4800)

////////////////////////////////////////////////////////////////////////////////////////////////////
///									== OP CODE DEFINITIONS  ==
////////////////////////////////////////////////////////////////////////////////////////////////////

// Jumps
#define OP_JO		0x70
#define OP_JNO		0x71
#define OP_JB		0x72
#define OP_JNB		0x73
#define OP_JZ		0x74
#define OP_JNZ		0x75
#define OP_JBE		0x76
#define OP_JA		0x77
#define OP_JS		0x78
#define OP_JNS		0x79
#define OP_JP		0x7A
#define OP_JNP		0x7B
#define OP_JL		0x7C
#define OP_JNL		0x7D
#define OP_JLE		0x7E
#define OP_JNLE		0x7F
#define OP_JCXZ		0xE3
#define OP_JMP		0xE9
#define OP_JMP_FAR	0xEA
#define OP_JMP_SHRT 0xEB

// Returns
#define OP_RETN		0xC2
#define OP_RET		0xC3

// Other
#define OP_INT3		0xCC

////////////////////////////////////////////////////////////////////////////////////////////////////
///											==   ==
////////////////////////////////////////////////////////////////////////////////////////////////////

NAMESPACE_LIB_B

class Function
{
private:

	// mbi instance
	MEMORY_BASIC_INFORMATION m_mbi;

	// Original protection of the region
	uint32	m_iProtection;

	// Pointer to the original function
	dword*	m_wpBase;

public:

	Function(dword*);
	Function();

	// Memory
	bool		ProtectRegion(uint32);
	bool		RestoreRegion();

	uint32		GetRegionProtection();
	uint32		GetOrigRegionProtection();

	void		SetMemory(uint32, uchar*, uint32 offset = 0);
	void		GetMemory(char*, uint32, uint32 offset = 0);

	// Hashing
	string		GetHash(uint32 iSize = 64);

	// Other
	dword*		GetReference();
	dword		GetDereferenced();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	template <class T> T Function::Call(uint32 iArgs, ...)
	///
	/// \brief	Calls the underlying function, passes each one of this function's arguments
	/// 		to the underlying function.  The underlying function MUST be a __stdcall
	/// 		or a __thiscall that does not use ECX (no this-> calls).
	///
	/// \tparam	T		Return type
	/// \param	iArgs	Number of arguments to push to the underlying function.
	///
	/// \return	EAX register after the underlying function is called.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	T			Call(uint32 iArgs, ...)
	{
		if (!this->m_wpBase)
			return (T)NULL;

		// Reverse order for __stdcall, __cdecl and __thiscall
		for (uint32 i = iArgs; i > 0; i--)
		{
			dword w = (dword)*(&iArgs + i);
			__asm PUSH w;
		}

		dword _retn;
		dword* func = this->m_wpBase;
		
		// Call the underlying function and move EAX -> wReturn
		__asm
		{
			CALL func
			MOV _retn, EAX
		}

		// Cast and return
		return (T)_retn;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	template <class T> T Function::CallClean(uint32 iArgs, ...)
	///
	/// \brief	Calls the underlying function.  This function also cleans the stack by pop'ing
	/// 		iArgs times (into EBX) making it compatible with __cdecl.  The above function 
	/// 		would work also, but would cause a stack overflow eventually.
	///
	/// \tparam	T		Return type
	/// \param	iArgs	Number of arguments to push to the underlying function.
	///
	/// \return	EAX register after the underlying function is called.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	T			CallClean(uint32 iArgs, ...)
	{
		if (!this->m_wpBase)
			return (T)NULL;

		/// Reverse order for __stdcall, __cdecl and __thiscall
		for (uint32 i = iArgs; i > 0; i--)
		{
			dword w = (dword)*(&iArgs + i);
			__asm PUSH w;
		}

		dword* _retn;
		dword* func = this->m_wpBase;

		// Call the underlying function and move EAX -> wReturn
		__asm
		{
			CALL func
			MOV _retn, EAX
		}

		// Clean the stack
		for (uint32 i = 0; i < iArgs; i++)
			__asm POP EBX;

		// Cast and return
		return (T)_retn;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	template <class T> T Function::CallMethod(void* vpInstPtr, uint32 iArgs, ...)
	///
	/// \brief	Calls the underlying function as if it was a class member function.  The class instance
	/// 		is (first paramater) is put in the ECX register.
	///
	/// \tparam	T					Return type
	/// \param [in,out]	vpInstPtr	Class instance pointer.
	/// \param	iArgs			 	Number of arguments to push to the underlying function.
	///
	/// \return	EAX register after the underlying function is called.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	T			CallMethod(void* vpInstPtr, uint32 iArgs, ...)
	{
		if (!this->m_wpBase)
			return (T)NULL;

		// Reverse order for __stdcall, __cdecl and __thiscall
		for (uint32 i = iArgs; i > 0; i--)
		{
			dword w = (dword)*(&iArgs + i);
			__asm PUSH w;
		}

		dword* _retn;
		dword* func = this->m_wpBase;

		// move vpInstPtr into ECX, call the underlying function and move EAX into _retn
		__asm
		{
			MOV ECX, vpInstPtr
			CALL func
			MOV _retn, EAX
		}

		// Pop the arguments off the stack
		for (uint32 i = 0; i < iArgs; i++)
			__asm POP EBX;

		// Cast and return
		return (T)_retn;
	}
};

NAMESPACE_END1