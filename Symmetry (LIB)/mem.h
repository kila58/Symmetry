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

NAMESPACE_LIB(Mem)

////////////////////////////////////////////////////////////////////////////////////////////////////
///		=						 = STRUCTURES/ENUMS/TYPEDEFS/MACROS ==
////////////////////////////////////////////////////////////////////////////////////////////////////

#define SetReg(reg, val) __asm MOV reg, val;
#define GetReg(reg, val) __asm MOV val, reg;

// For pointer arithmatic
#define MakePtr(cast, ptr, add) (cast)((DWORD_PTR)(ptr) + (DWORD_PTR)(add))

// Delete and make an object 0 (null)
// seems a fitting to place to make sure we're not deleting 0 either
#define DeleteAndNull(object) if (object != NULL) delete object; object = NULL
#define FreeAndNull(object) if (object != NULL) free(object); object = NULL

////////////////////////////////////////////////////////////////////////////////////////////////////
///		=								= SIGNATURE STUFF ==
////////////////////////////////////////////////////////////////////////////////////////////////////

// Basic signature structure
struct Signature
{
	uchar*	cPattern;	// Memory pattern, use codestyle.  (\x12\x34\x56\x78\x90)
	uchar*	cMask;		// The signature mask, a string if x's and ?'s
	uint64	iSize;		// The size of the pattern, or the range.  Searches are performed from wEntry to wEntry + iSize
	dword	wEntry;		// Memory address to start searching from
};

// Signature functions
Signature	MakeSignature(string, string);
bool		IsSignature(Signature);
dword		ResolveSignature(Signature);

////////////////////////////////////////////////////////////////////////////////////////////////////
///		=							   = MEMORY STUFF ==
////////////////////////////////////////////////////////////////////////////////////////////////////

// String related
char_c*	CopyString(char_c*, uint64 iSize = 0);

string	SanitizeUserInput(string);
string	ParseSpecialChars(string);
void	ReplaceString(string&, string, string);
bool	IsRegexMatch(string, string, 
	regex_constants::syntax_option_type iFlags = regex_constants::icase);
bool	IsRegexMatchSub(string, string,
	regex_constants::syntax_option_type iFlags = regex_constants::icase);

// Module/process related
uint64			GetModuleSize(string);
uint32			GetProcessIDByName(string);
MODULEENTRY32	GetRemoteModule(dword, string);
string			GetModuleFullPath(string);
FARPROC			GetRemoteProcAddress(dword, string, string);
HMODULE			GetRemoteModuleHandle(dword, string);
bool			CallRemoteFunction(uint32, dword, void*, uint32);

// Other
int		GetVMTOffset(PDWORD*, dword*);
bool	IsSectionNull(void*, dword);

////////////////////////////////////////////////////////////////////////////////////////////////////
///									  == CRYPTOGRAPHY ==
////////////////////////////////////////////////////////////////////////////////////////////////////

string	GetHash(string);
string	GetMD5(string);

string	AESDecrypt(string, uchar*, uchar*);
string	AESEncrypt(string, uchar*, uchar*);

NAMESPACE_END2