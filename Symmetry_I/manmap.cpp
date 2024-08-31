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

#include "manmap.h"
#include "mem.h"

#include "file.h"

#include "error.h"

using namespace Lib;

NAMESPACE_LIB(ManualMap)

const static unsigned char g_pExecStub[] =
{
	0x68, 0x00, 0x00, 0x00, 0x00,	// push lpReserved
	0x68, 0x01, 0x00, 0x00, 0x00,	// push dwReason
	0x68, 0x00, 0x00, 0x00, 0x00,	// push hModule
	0xFF, 0x54, 0x24, 0x10,			// call [esp + 10h]
	0xC3							// ret
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Map(dword dwPID, string sData, vector<string>* mapped)
///
/// \brief	Manually maps a DLL into a process.  The process is described by dwPID (the
/// 		processes' ID) and the DLL's data should be fully contained in sData.
///
/// \param	dwPID		  	Processes' ID.
/// \param	sData		  	The DLL data.
/// \param [in,out]	mapped	A list of alread mapped modules, this should be null and is
/// 						only used as an argument by the recursive imports loader.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

HANDLE Map(dword dwPID, string sData)
{
	IMAGE_DOS_HEADER*	dosHeader;
	IMAGE_NT_HEADERS*	peHeader;

	uchar* data = (unsigned char*)sData.data();

	// Placeholder for calls that need a pointer to a DWORD.  Such as
	// VirtualProtect
	dword bytes = 0;

	// DOS header should be at the very start of a DLL.
	// It's not really needed, but it does point to something we
	// need (PE header)
	dosHeader = (IMAGE_DOS_HEADER*)data;

	// Make sure the DOS header is valid
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		SetLastErrorA(MAP_BAD_DOS);
		return INVALID_HANDLE_VALUE;
	}

	peHeader = MakePtr(IMAGE_NT_HEADERS*, data, dosHeader->e_lfanew);

	// Make sure the PE header is valid
	if (peHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		SetLastErrorA(MAP_BAD_PE);
		return INVALID_HANDLE_VALUE;
	}
		

	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (!proc)
	{
		SetLastErrorA(MAP_BAD_PROCESS);
		return INVALID_HANDLE_VALUE;
	}
		
	// Allocate a chunk of memory in the target application for the dll
	void* dllBase = VirtualAllocEx(proc, NULL, peHeader->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!dllBase)
	{
		SetLastErrorA(MAP_BAD_MEMORY);
		return INVALID_HANDLE_VALUE;
	}
		

	// Allocate a chunk of memory in the target appliction for our entry point stub
	void* stubBase = VirtualAllocEx(proc, NULL, sizeof(g_pExecStub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!stubBase || !VirtualProtect((void*)g_pExecStub, sizeof(g_pExecStub), PAGE_EXECUTE_READWRITE, &bytes))
	{
		SetLastErrorA(MAP_BAD_MEMORY);
		return INVALID_HANDLE_VALUE;
	}
		
	IMAGE_IMPORT_DESCRIPTOR* imports = (IMAGE_IMPORT_DESCRIPTOR*)GetPtrFromRVA(
		GETSECTION(peHeader, IMAGE_DIRECTORY_ENTRY_IMPORT).VirtualAddress,
		peHeader, data);

	// Fix the imports section (function's need resolving)
	if (!FixImports(dwPID, data, peHeader, imports))
	{
		SetLastErrorA(MAP_BAD_IMPORTS);
		return INVALID_HANDLE_VALUE;
	}
		

	IMAGE_BASE_RELOCATION* baseReloc = (IMAGE_BASE_RELOCATION*)GetPtrFromRVA(
		GETSECTION(peHeader, IMAGE_DIRECTORY_ENTRY_BASERELOC).VirtualAddress,
		peHeader, data);

	// Fix the base relocations so that the module can (in theory)
	// load at any address in the remote process
	FixRelocations(data, dllBase, peHeader, baseReloc, GETSECTION(peHeader, IMAGE_DIRECTORY_ENTRY_BASERELOC).Size);

	// Map each section into the target process
	MapSections(proc, data, dllBase, peHeader);

	// Patch the stub's CALL argument with the address
	// of the module's entry point
	*MakePtr(dword*, g_pExecStub, 0x0B) = (dword)dllBase;

	// Write the stub into the target process
	WriteProcessMemory(proc, stubBase, g_pExecStub, sizeof(g_pExecStub), &bytes);

	// Execute the stub
	HANDLE thread = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)stubBase,
		MakePtr(void*, dllBase, peHeader->OptionalHeader.AddressOfEntryPoint),
		0, NULL);

	return (HANDLE)dllBase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool MapLL(dword dwPID, string sLibName)
///
/// \brief	Maps a module in a remote process using LoadLibrary.  This is mainly used for dependancies
/// 		in the manually mapping method above.
/// 		
/// 		Kernel32 is mapped at the same address in every process all we need to do is call the 
/// 		function in the remote process.  We'll do that by WPM'ing our module name into the 
/// 		target process, and calling LoadLibrary with that WPM'd memory chunk using
///			CreateRemoteThread.
///
/// \param	dwPID   	The PID of the target process.
/// \param	sLibName	Name of the library.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool MapLL(dword dwPID, string sLibName)
{
	const char* name = sLibName.c_str();
	dword		addr = (dword)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA");
	HANDLE		proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	dword wrote;

	if (!addr || !proc)
		return false;

	// Allocate a chunk of memory in the remote process for the module name
	void* nameMem = VirtualAllocEx(proc, NULL, sLibName.length(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!nameMem)
		return false;

	// Write the module name into that chunk of allocated memory
	WriteProcessMemory(proc, nameMem, name, sLibName.length(), &wrote);

	if (!wrote)
		return false;

	// Call LoadLibrary passing the module name
	HANDLE thread = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)addr, nameMem, 0, NULL);

	// Wait until the thread loads the module before continuing (max wait is infinite because
	// this code is required to run before imports can be fixed)
	WaitForSingleObject(thread, INFINITE);

	// Don't need the handle anymore
	CloseHandle(thread);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void FixImports(dword dwPID, void* vpBase, IMAGE_NT_HEADERS* pPE,
/// 	IMAGE_IMPORT_DESCRIPTOR* pImports)
///
/// \brief	Resolves each function in the IAT.
///
/// \param	dwPID				The PID of the target process.
/// \param [in,out]	vpBase  	Base of the PE module.
/// \param [in,out]	pPE			Pointer to the PE header.
/// \param [in,out]	pImports	The import descriptor.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool FixImports(dword dwPID, void* vpBase, IMAGE_NT_HEADERS* pPE, IMAGE_IMPORT_DESCRIPTOR* pImports)
{
	for (char* module; (module = (char*)GetPtrFromRVA(pImports->Name, pPE, (uchar*)vpBase)); pImports++)
	{
		// We need to load the library so we can calculate the difference
		// between the modules base address and one of it's exported functions.
		HMODULE localMod = LoadLibrary(module);

		// We also need to load it in the remote process
		if (!Mem::GetRemoteModuleHandle(dwPID, module))
		{
			if (!MapLL(dwPID, module))
				return false;
		}

		// We assume the function we need are NOT forwarded, I can't
		// be asked supporting forwarded functions
		IMAGE_THUNK_DATA *itd =
			(IMAGE_THUNK_DATA *)GetPtrFromRVA((DWORD)(pImports->FirstThunk), pPE, (PBYTE)vpBase);

		int numImports = 0;
		while (itd->u1.AddressOfData)
		{
			IMAGE_IMPORT_BY_NAME *iibn;
			iibn = (IMAGE_IMPORT_BY_NAME *)GetPtrFromRVA((DWORD)(itd->u1.AddressOfData), pPE, (PBYTE)vpBase);

			itd->u1.Function = MakePtr(dword, Mem::GetRemoteProcAddress(dwPID, module, iibn->Name), 0);
			itd++;

			numImports++;
		}

		// Free the temporarily loaded libray?
		FreeLibrary(localMod);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void FixRelocations(void* vpBase, void *vpRemoteBase, IMAGE_NT_HEADERS* pe,
/// 	IMAGE_BASE_RELOCATION* reloc, uint32 iSize)
///
/// \brief	Fix base relocations.
///
/// \param [in,out]	vpBase			The data from the binary, loaded into a string,
/// \param [in,out]	vpRemoteBase	The data allocated in the remote process.
/// \param [in,out]	pe				The PE header.
/// \param [in,out]	reloc			Base relocation table.
/// \param	iSize					The size of the base relocation table.
////////////////////////////////////////////////////////////////////////////////////////////////////

void FixRelocations(void* vpBase, void *vpRemoteBase, IMAGE_NT_HEADERS* pe, IMAGE_BASE_RELOCATION* reloc, uint32 iSize)
{
	// ImageBase is the preferred 
	dword base = pe->OptionalHeader.ImageBase;
	dword delta = (dword)vpRemoteBase - (dword)base;
	dword read = 0;

	int numRelocations = 0;

	while (true)
	{
		if (read >= iSize) break;

		dword*	locBase = (dword*)GetPtrFromRVA((dword)reloc->VirtualAddress, pe, (uchar*)vpBase);
		uint32	nRelocs = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(word);
		word*	loc = MakePtr(word*, reloc, sizeof(IMAGE_BASE_RELOCATION));

		// Add delta to each base relocation after the IMAGE_BASE_RELOCATION header
		for (uint32 i = 0; i < nRelocs; i++)
		{
			// Other relocation types (in 32 bit PE files)
			// are just padding
			if ((*loc >> 12) & IMAGE_REL_BASED_HIGHLOW)
			{
				*MakePtr(dword*, locBase, (*loc & 0x0FFF)) += delta;
				numRelocations++;
			}


			loc++;
		}

		read += reloc->SizeOfBlock;
		reloc = (IMAGE_BASE_RELOCATION*)loc;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void MapSections(HANDLE hProc, void* vpBase, void* vpRemoteBase, IMAGE_NT_HEADERS* pe)
///
/// \brief	Maps each section of the PE module into the remote process (vpRemoteBase should be a
/// 		memory location in the target remote process).
///
/// \param	hProc					A handle of the target process.
/// \param [in,out]	vpBase			The PE file's base.
/// \param [in,out]	vpRemoteBase	The memory location in the remote process.
/// \param [in,out]	pe				The PE header.
////////////////////////////////////////////////////////////////////////////////////////////////////

void MapSections(HANDLE hProc, void* vpBase, void* vpRemoteBase, IMAGE_NT_HEADERS* pe)
{
	IMAGE_SECTION_HEADER* header = IMAGE_FIRST_SECTION(pe);
	uint32 writtenBytes = 0, n = 0;

	for (uint32 i = 0; i < pe->FileHeader.NumberOfSections; i++)
	{
		// We don't need to incude any extra debug sections if
		// they exist
		if (writtenBytes >= pe->OptionalHeader.SizeOfImage)
			break;

		// Write the section into the target process
		WriteProcessMemory(hProc,
			MakePtr(void*, vpRemoteBase, header->VirtualAddress),
			MakePtr(void*, vpBase, header->PointerToRawData),
			header->SizeOfRawData,
			(LPDWORD)&n);

		uint32 virtualSize = header->VirtualAddress;
		header++;
		writtenBytes += (header->VirtualAddress - virtualSize);

		// Apply protection flags on the written section
		VirtualProtectEx(hProc, MakePtr(LPVOID, vpRemoteBase, header->VirtualAddress), virtualSize,
			header->Characteristics & 0x00FFFFFF, NULL);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	dword* ResolveExport(string sData, char* cFunctionName)
///
/// \brief	Gets the RVA of an exported function in a binary.
///
/// \param	sData				 	The DLL.
/// \param [in,out]	cFunctionName	Name of the exported function.
///
/// \return	null if it fails, the RVA of the function if it succeeds.
////////////////////////////////////////////////////////////////////////////////////////////////////

dword* ResolveExport(string sData, char* cFunctionName)
{
	IMAGE_DOS_HEADER*	dosHeader;
	IMAGE_NT_HEADERS*	peHeader;

	uchar* data = (unsigned char*)sData.data();

	// Placeholder for calls that need a pointer to a DWORD.  Such as
	// VirtualProtect
	dword bytes = 0;

	// DOS header should be at the very start of a DLL.
	// It's not really needed, but it does point to something we
	// need (PE header)
	dosHeader = (IMAGE_DOS_HEADER*)data;

	// Make sure the DOS header is valid
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return NULL;

	peHeader = MakePtr(IMAGE_NT_HEADERS*, data, dosHeader->e_lfanew);

	// Resolve exports
	IMAGE_EXPORT_DIRECTORY* exportDir = (IMAGE_EXPORT_DIRECTORY*)GetPtrFromRVA(
		GETSECTION(peHeader, IMAGE_DIRECTORY_ENTRY_EXPORT).VirtualAddress,
		peHeader, data);

	if (exportDir)
	{
		PVOID* addresses = (PVOID*)GetPtrFromRVA(exportDir->AddressOfFunctions, peHeader, data);
		WORD*  nameordinal = (WORD*)GetPtrFromRVA(exportDir->AddressOfNameOrdinals, peHeader, data);
		char ** names = (char**)GetPtrFromRVA(exportDir->AddressOfNames, peHeader, data);

		for (uint32 i = 0; i < exportDir->NumberOfNames; i++)
		{
			string name = (char*)GetPtrFromRVA((DWORD)names[i], peHeader, data);
			if (name == cFunctionName)
				return (dword*)addresses[nameordinal[i]];

		}
	}

	return NULL;
}

// ====================================================================================================================== //

// Matt Pietrek's functions
PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(DWORD rva, PIMAGE_NT_HEADERS pNTHeader)
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);
	unsigned int i;

	for (i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++)
	{
		// This 3 line idiocy is because Watcom's linker actually sets the
		// Misc.VirtualSize field to 0.  (!!! - Retards....!!!)
		DWORD size = section->Misc.VirtualSize;
		if (0 == size)
			size = section->SizeOfRawData;

		// Is the RVA within this section?
		if ((rva >= section->VirtualAddress) &&
			(rva < (section->VirtualAddress + size)))
			return section;
	}

	return 0;
}

// This function is also Matt's function
LPVOID GetPtrFromRVA(DWORD rva, IMAGE_NT_HEADERS *pNTHeader, PBYTE imageBase)
{
	PIMAGE_SECTION_HEADER pSectionHdr;
	INT delta;

	pSectionHdr = GetEnclosingSectionHeader(rva, pNTHeader);
	if (!pSectionHdr)
		return 0;

	delta = (INT)(pSectionHdr->VirtualAddress - pSectionHdr->PointerToRawData);
	return (PVOID)(imageBase + rva - delta);
}

NAMESPACE_END2