/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "symmetry.h"

#include "global.h"

// == DarthTon's C++EH fix for manually mapped binaries == //

typedef _EXCEPTION_DISPOSITION(__cdecl *_pexcept_handler)
(
	_EXCEPTION_RECORD *ExceptionRecord,
	void* EstablisherFrame,
	_CONTEXT *ContextRecord,
	void * DispatcherContext
);

#pragma warning(disable:4733)
LONG CALLBACK VectoredHandler32(_In_ PEXCEPTION_POINTERS ExceptionInfo)
{
	EXCEPTION_REGISTRATION_RECORD  *pFs = (EXCEPTION_REGISTRATION_RECORD*)__readfsdword(0);
	EXCEPTION_DISPOSITION            res = ExceptionContinueSearch;

	// Prevent CRT from calling handlers in chain with EH_UNWINDING
	for (; res == ExceptionContinueSearch && pFs && pFs != (EXCEPTION_REGISTRATION_RECORD*)0xffffffff; pFs = pFs->Next, __writefsdword(0, (DWORD)pFs))
	{
		ExceptionInfo->ExceptionRecord->ExceptionFlags &= ~EXCEPTION_UNWIND;

		if (pFs->Handler)
		{
			// Last frame contains special handler with __stdcall convention
			if (pFs->Next != (EXCEPTION_REGISTRATION_RECORD*)0xffffffff)
				res = ((_pexcept_handler)pFs->Handler)(ExceptionInfo->ExceptionRecord, pFs, ExceptionInfo->ContextRecord, NULL);
			else
				res = pFs->Handler(ExceptionInfo->ExceptionRecord, pFs, ExceptionInfo->ContextRecord, NULL);

			// Unwind stack properly
			if (res == ExceptionContinueSearch)
			{
				ExceptionInfo->ExceptionRecord->ExceptionFlags |= EXCEPTION_UNWIND;

				if (pFs->Next != (EXCEPTION_REGISTRATION_RECORD*)0xffffffff)
					res = ((_pexcept_handler)pFs->Handler)(ExceptionInfo->ExceptionRecord, pFs, ExceptionInfo->ContextRecord, NULL);
				else
					res = pFs->Handler(ExceptionInfo->ExceptionRecord, pFs, ExceptionInfo->ContextRecord, NULL);
			}
		}
	}

	// We are screwed if got here
	return EXCEPTION_CONTINUE_SEARCH;
}

// == ================================================ == //

BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	Core::PreCRTInit();

	// Skeleton code from MSDN
	if (fdwReason == DLL_PROCESS_ATTACH || fdwReason == DLL_THREAD_ATTACH)
		if (!_CRT_INIT(hinstDLL, fdwReason, lpReserved))
			return(FALSE);

	if (fdwReason == DLL_PROCESS_DETACH || fdwReason == DLL_THREAD_DETACH)
		if (!_CRT_INIT(hinstDLL, fdwReason, lpReserved))
			return(FALSE);

	// Darthton's fix
	AddVectoredExceptionHandler(0, &VectoredHandler32);

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		Core::PostCRTInit();
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		// nothing here
	}

	return(TRUE);
}

