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

#include <TlHelp32.h>

#define GETSECTION(header, directory) (header->OptionalHeader.DataDirectory[directory])

NAMESPACE_LIB(ManualMap)

// Manual map function
HANDLE						Map(dword dwPID, string sData);
bool						MapLL(dword dwPID, string sModule);

// Map helpers
bool						FixImports(dword dwPID, void* dllbase, IMAGE_NT_HEADERS* pe, IMAGE_IMPORT_DESCRIPTOR* imports);
void						FixRelocations(void* vpBase, void *vpRemoteBase, IMAGE_NT_HEADERS* pe, IMAGE_BASE_RELOCATION* reloc, uint32 iSize);
void						MapSections(HANDLE proc, void* vpBase, void* vpRemoteBase, IMAGE_NT_HEADERS* pe);

// Matt Pietrek's functions
PIMAGE_SECTION_HEADER		GetEnclosingSectionHeader(DWORD rva, PIMAGE_NT_HEADERS pNTHeader);
LPVOID						GetPtrFromRVA(DWORD rva, IMAGE_NT_HEADERS *pNTHeader, PBYTE imageBase);

// Other functions
dword*						ResolveExport(string sData, char* cFunctionName);

// Errors
#define MAP_BAD_DOS			-1
#define MAP_BAD_PE			-2
#define MAP_BAD_PROCESS		-3
#define MAP_BAD_MEMORY		-4
#define MAP_BAD_IMPORTS		-5

NAMESPACE_END2