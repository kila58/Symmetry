/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "mem.h"
#include "vmt.h"

#include "sha256.h"
#include "md5.h"
#include "aes.h"

NAMESPACE_LIB(Mem)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn Signature MakeSignature(short* spPattern, string sModuleName)
///
/// \brief Makes a signature using information from a module.  sModuleName
/// 	   should be the name of the module.
///
/// \param spPattern		  Placed directly into the Signature structure.
/// \param sModuleName		  Name of the module that the signature should reside in.
///
/// \return .
////////////////////////////////////////////////////////////////////////////////////////////////////

Signature MakeSignature(string sPattern, string sModuleName)
{
	uchar* cMask	= (uchar*)malloc(sPattern.length());
	uchar* cSig		= (uchar*)malloc(sPattern.length());

	int x = 0;
	for (uint32 i = 0; i < sPattern.length(); i += 3, x++)
	{
		string sub = sPattern.substr(i, 2);
		stringstream converter(sub);

		if (sub == "??")
		{
			cMask[x]	= '?';
			cSig[x]		= 0x69; // lol why not
		}
		else 
		{
			short s; converter >> hex >> s;

			cMask[x]	= 'x';
			cSig[x]		= (uchar)s;
		}
	}

	// Null-termimate
	cMask[x]	= '\0';
	cSig[x]		= '\0';

	const char* ccName = sModuleName == "" ? NULL : sModuleName.c_str();

	Signature sig = { 
		cSig,
		cMask,
		(uint64)Mem::GetModuleSize( sModuleName ), 
		(dword)GetModuleHandle( ccName )
	};

	return sig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool IsSignature(Signature sSig)
///
/// \brief Checks if a signature resolves at all.
///
/// \param sSig The signature.
///
/// \return True if it resolves, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsSignature(Signature sSig)
{
	return Mem::ResolveSignature(sSig) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn dword ResolveSignature(Signature sSig)
///
/// \brief Resolve signature.
///
/// \param sSig The signal.
///
/// \return .
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ComparePattern(uchar_c* pData, uchar_c* bMask, uchar_c* szMask)
{
	for (;*szMask; ++szMask, ++pData, ++bMask)
	{
		if ( *szMask == 'x' && *pData != *bMask )
			return false;
	}

	return (*szMask) == 0;
}

dword ResolveSignature(Signature sSig)
{
	for (dword i = 0; i < sSig.iSize; i++)
	{
		if (ComparePattern((uchar*)(sSig.wEntry + i), sSig.cPattern, sSig.cMask))
			return (dword)(sSig.wEntry + i);
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn char_c* CopyString(char_c* cpString)
///
/// \brief Copies the string into a new buffer and returns the buffer.
///
/// \param cpString The string to copy.
///
/// \return The copied string.
////////////////////////////////////////////////////////////////////////////////////////////////////

char_c*	CopyString(char_c* cpString, uint64 iSize)
{
	size_t size		= (size_t) (iSize == 0 ? (strlen(cpString) + 1) : iSize);
	char*  c		= (char*)malloc(size);

	memcpy( c, cpString, size );

	return c;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string SanitizeUserInput(string sInput)
///
/// \brief	Sanitizes user input by removing characters that have special effects. Characters 
/// 		removed include: 
/// 		
/// 		NUL, SOH, STX, ETX, EOT, ENQ, ACK, BEL, BS, TAB, LF, VT, FF, CR, SO, SI, DLE, DC1
/// 		DC2, DC3, DC4, NAK, SYN, ETB, CAN, EM, SUB, ESC, FS, GS, RS and US.
///
/// \param	sInput	The input.
///
/// \return	Sanitized input
////////////////////////////////////////////////////////////////////////////////////////////////////

string SanitizeUserInput(string sInput)
{
	string newInput;

	for (uint32 i = 0; i < sInput.length(); i++)
	{
		// Why not whitelist a list of valid characters rather than blackisting some?
		// because many other languages use extra or different characters 
		// (norwegian, swedish, danish, russian, arabic, etc..)
		if (((char)sInput[i]) > 0x1F)
			newInput += sInput[i];
	}

	return newInput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string ParseSpecialChars(string sInput)
///
/// \brief	Parse special characters.
///
/// \param	sInput	The input.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

string ParseSpecialChars(string sInput)
{
	string rebuilt;
	for (uint32 i = 0; i < sInput.length(); i++)
	{
		char c = sInput[i];
		if (c == '\\' && i != sInput.length())
		{
			switch (sInput[i + 1])
			{
			case 'n':
				rebuilt.append("\n"); break;
			case 't':
				rebuilt.append("\t"); break;
			case 'b':
				rebuilt.append("\b"); break;
			case 'r':
				rebuilt.append("\r"); break;
			case '0':
				rebuilt.append("\0"); break;
			default:
				rebuilt += c;
				i--;
				break;
			}

			i++;
		}
		else
			rebuilt += c;
	}
	
	return rebuilt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string ReplaceString(string sText, string sNeedle, string sReplacement)
///
/// \brief	Replaces all occurences of of sNeedle in sText with sReplacement.
///
/// \param	sText			The text.
/// \param	sNeedle			The needle.
/// \param	sReplacement	The replacement.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ReplaceString(string& sText, string sNeedle, string sReplacement)
{
	size_t pos = 0;
	while ((pos = sText.find(sNeedle, pos)) != std::string::npos) 
	{
		sText.replace(pos, sNeedle.length(), sReplacement);
		pos += sReplacement.length();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool IsRegexMatch(string sText, string sRegex)
///
/// \brief	Returns true if sRegex matches the regular expresion sRegex.
///
/// \param	sText 	The text.
/// \param	sRegex	The regular expression.
///
/// \return	See above.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsRegexMatch(string sText, string sRegex, 
	regex_constants::syntax_option_type iFlags)
{
	return regex_match(sText, regex(sRegex, iFlags));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool IsRegexMatchSub(string sText, string sRegex,
/// 	regex_constants::syntax_option_type iFlags)
///
/// \brief	Same as above but matches sub queries as well.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsRegexMatchSub(string sText, string sRegex,
	regex_constants::syntax_option_type iFlags)
{
	return regex_search(sText, regex(sRegex, iFlags));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	uint32 GetVMTOffset(void* vpVMT, dword* dwFunc)
///
/// \brief	Searches a virtual method table for a function.
///
/// \param [in,out]	vpVMT 	The virtual method table.
/// \param [in,out]	dwFunc	Function address.
///
/// \return	The VMT offset, or -1 if dwFunc wasn't found in vpVMT.
////////////////////////////////////////////////////////////////////////////////////////////////////

int GetVMTOffset(PDWORD* vpVMT, dword* dwFunc)
{
	// Use Detours::VMT to iterate over the virtual method table
	Detours::VMT* v = new Detours::VMT(vpVMT);

	for (uint32 i = 0; i < v->GetMethodCount(); i++)
	{
		if (v->GetMethod(i) == dwFunc)
		{
			delete v;
			return i;
		}		
	}

	delete v;
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool IsSectionNull(void* vSection, dword dwSize)
///
/// \brief	Query if every byte between vSection and vSection + dwSize is 0.
///
/// \param [in,out]	vSection	Memory address.
/// \param	dwSize				Amount of bytes.
///
/// \return	true if section null, false if not.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsSectionNull(void* vSection, dword dwSize)
{
	uchar* mem = (uchar*)vSection;

	// this section better fucking not be protected
	for (dword i = 0; i < dwSize; i++)
	{
		if (mem[i] != 0)
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	uint64 GetModuleSize(string sModuleName)
///
/// \brief	Returns the size of a module.
///
/// \param	sModuleName	Name of the module.
///
/// \return	The size of a module.
////////////////////////////////////////////////////////////////////////////////////////////////////

uint64 GetModuleSize(string sModuleName)
{
	MODULEENTRY32 module = GetRemoteModule(GetCurrentProcessId(), sModuleName);

	// The start of this structure shouldn't be 0, neither should the size
	if (module.dwSize == 0)
		return 0;

	return module.modBaseSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	uint32 GetProcessIDByName(string sProcName)
///
/// \brief	Returns the ID of a process by name.  This function will return 0
/// 		if it fails.  The system processes' ID is 0, but we should never need to use this
/// 		function to get the system processes' ID because it is always 0.
/// 		
/// 		This won't work for 64 bit proccesses.
///
/// \param	sProcName	Name of the process.
///
/// \return	The process ID, or 0.
////////////////////////////////////////////////////////////////////////////////////////////////////

uint32 GetProcessIDByName(string sProcName)
{
	PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, GetCurrentProcessId());

	if (Process32First(snapshot, &process))
	{
		while (Process32Next(snapshot, &process))
		{
			if (string(process.szExeFile) == sProcName)
			{
				CloseHandle(snapshot);
				return process.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	HMODULE GetRemoteModuleHandle(DWORD dwPID, string sModuleName)
///
/// \brief	Gets the handle to a module in a remote process.
///
/// \param	dwPID	   	The PID of the target process.
/// \param	sModuleName	Name of the module.
///
/// \return	The remote module handle.
////////////////////////////////////////////////////////////////////////////////////////////////////

MODULEENTRY32 GetRemoteModule(DWORD dwPID, string sModuleName)
{
	MODULEENTRY32 module = { sizeof(MODULEENTRY32) };
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if (Module32First(snapshot, &module))
	{
		while (Module32Next(snapshot, &module))
		{
			if (sModuleName == module.szModule)
			{
				CloseHandle(snapshot);
				return module;
			}
		}
	}

	CloseHandle(snapshot);
	return module;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	FARPROC GetRemoteProcAddress(DWORD dwPID, string sModuleName, string sProcName)
///
/// \brief	Gets the address of a function in a library in a remote process.
///
/// \param	dwPID	   	The PID of the target process.
/// \param	sModuleName	Name of the module.
/// \param	sProcName  	Name of the function.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

FARPROC GetRemoteProcAddress(DWORD dwPID, string sModuleName, string sProcName)
{
	HMODULE remoteModule = GetRemoteModuleHandle(dwPID, sModuleName);
	HMODULE localModule = GetModuleHandle(sModuleName.c_str());

	// Return the address of the function in our local library
	// plus the difference between where the module's mapped in our process
	// and the target process.
	uint32 diff = ((uint32)GetProcAddress(localModule, sProcName.c_str())) - (uint32)localModule;
	return (FARPROC)((dword)remoteModule + (dword)diff);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string GetModuleFullPath(string sModule)
///
/// \brief	Returns the full path to a module loaded in this process.
///
/// \param	sModule	The module's name.
///
/// \return	The module's full path.
////////////////////////////////////////////////////////////////////////////////////////////////////

string GetModuleFullPath(string sModule)
{
	MODULEENTRY32 entry = GetRemoteModule(GetCurrentProcessId(), sModule);

	// The start of this structure shouldn't be 0, neither should the size
	if (entry.dwSize == 0)
		return "";

	return entry.szExePath;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	HMODULE GetRemoteModuleHandle(DWORD dwPID, string sModuleName)
///
/// \brief	Gets the handle to a module in a remote process.
///
/// \param	dwPID	   	The PID of the target process.
/// \param	sModuleName	Name of the module.
///
/// \return	The remote module handle.
////////////////////////////////////////////////////////////////////////////////////////////////////

HMODULE GetRemoteModuleHandle(DWORD dwPID, string sModuleName)
{
	MODULEENTRY32 module = { sizeof(MODULEENTRY32) };
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if (Module32First(snapshot, &module))
	{
		while (Module32Next(snapshot, &module))
		{
			if (!_stricmp(sModuleName.c_str(), module.szModule))
			{
				CloseHandle(snapshot);
				return module.hModule;
			}
		}
	}

	CloseHandle(snapshot);
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CallRemoteFunction(uint32 iProcID, dword dwRemoteAddress, void* vpData,
/// 	uint32 iDataSize)
///
/// \brief	Calls a function in a remote process.  This function will halt until the remote
/// 		function returns.  vpData is a pointer to a block of data in memory and iDataSize is
/// 		the size of the data block.  The data block is passed to the remote function.
///
/// \param	iProcID		   	Process ID.
/// \param	dwRemoteAddress	The remote address.
/// \param [in,out]	vpData	Pointer to a block of memory that contains argument data.
/// \param	iDataSize	   	The length of the argument data.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CallRemoteFunction(uint32 iProcID, dword dwRemoteAddress, void* vpData, uint32 iDataSize)
{
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, iProcID);
	dword written;

	if (!proc || proc == INVALID_HANDLE_VALUE)
	{
		MessageBox(0, "proc invalid", 0, 0);
		return false;
	}


	// Allocate memory for our argument
	void* arg = VirtualAllocEx(proc, NULL, iDataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!arg)
	{
		MessageBox(0, "bad arg", 0, 0);
		return false;
	}

	// Write vpData into the virtually allocated memory in the remote process
	if (!WriteProcessMemory(proc, arg, vpData, iDataSize, &written))
	{
		MessageBox(0, "bad wpm", 0, 0);
		return false;
	}

	// Call the remote function
	HANDLE thread = CreateRemoteThread(proc, 0, 0,
		(LPTHREAD_START_ROUTINE)dwRemoteAddress, arg, 0, 0);

	WaitForSingleObject(thread, INFINITE);

	// Cleanup
	if (!CloseHandle(thread))
	{
		MessageBox(0, "bad closehandle(thread)", 0, 0);
		return false;
	}

	if (!CloseHandle(proc))
	{
		MessageBox(0, "bad closehandle(proc)", 0, 0);
		return false;
	}

	return written == iDataSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn string GetHash(string sData)
///
/// \brief Returns the hash of sData. Hashing algorithm is SHA256.
///
/// \param sData String of data to hash.
///
/// \return sData's hash.
////////////////////////////////////////////////////////////////////////////////////////////////////

string GetHash(string sData)
{
	// Replaced CryptoPP
	return sha256(sData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string GetMD5(string sData)
///
/// \brief	Returns the MD5 hash of a string.
///
/// \param	sData	The string.
///
/// \return	The MD5 hash.
////////////////////////////////////////////////////////////////////////////////////////////////////

string	GetMD5(string sData)
{
	return md5(sData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string AESDecrypt(string sData, uchar* cKey, uchar* cIV)
///
/// \brief	Helper function for decrypting an encrypted AES block of text.
///
/// \param	sData			The data to decrypt.
/// \param [in,out]	cKey	The AES key.
/// \param [in,out]	cIV 	The AES initialization vector.
///
/// \return	Decrypted text.
////////////////////////////////////////////////////////////////////////////////////////////////////

string	AESDecrypt(string sData, uchar* cKey, uchar* cIV)
{
	uchar iv[16];
	size_t len = sData.length();

	// IV is reset by aes_cbc_decrypt
	memcpy(iv, cIV, 16);

	// Temporary output buffer
	uchar* buf = (uchar*)malloc(len);

	// Decrypt all AES blocks
	aes_decrypt_ctx ctx;

	aes_decrypt_key128(cKey, &ctx);
	aes_cbc_decrypt((uchar*)sData.data(), buf, len, iv, &ctx);

	// PKCS7 padding adds n bytes where each byte is n
	// and 1 < n <= 16
	string s = string((char*)buf, len - (buf[len - 1]));

	// Free temporary buffer
	free(buf);

	return s;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string AESEncrypt(string sText, uchar* cKey, uchar* cIV)
///
/// \brief	Helper function for encrypting text.
///
/// \param	sText			The text to encrypt.
/// \param [in,out]	cKey	The AES key.
/// \param [in,out]	cIV 	The AES initialization vector.
///
/// \return	Encrypted data.
////////////////////////////////////////////////////////////////////////////////////////////////////

string	AESEncrypt(string sText, uchar* cKey, uchar* cIV)
{	
	aes_encrypt_ctx ctx;
	aes_encrypt_key128(cKey, &ctx);

	size_t len = sText.length();

	uchar* buf = (uchar*)malloc(len + (len % 16));
	aes_cbc_encrypt((uchar*)sText.data(), buf, len, cIV, &ctx);

	return string((char*)buf, len + (len % 16));
}

NAMESPACE_END2
