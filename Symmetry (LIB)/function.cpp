/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "function.h"
#include "mem.h"

NAMESPACE_LIB_B

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	Function::Function()
///
/// \brief	Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

Function::Function()
{
	this->m_wpBase = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	Function::Function(dword* wpFuncPtr, bool bPrepare)
///
/// \brief	Constructor.
///
/// \param [in,out]	wpFuncPtr	A pointer to the target function.
/// \param	bPrepare		 	If this is true, PrepareDisassembler will be called.
////////////////////////////////////////////////////////////////////////////////////////////////////

Function::Function(dword* wpFuncPtr)
{
	VirtualQuery( wpFuncPtr, &this->m_mbi, sizeof( this->m_mbi ) );

	// Initialize values
	this->m_wpBase = wpFuncPtr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool Function::ProtectRegion(uint32 iProtect)
///
/// \brief Protects the function's memory region.  (Had to use a dummy variable as VirtualProtect
/// 	   would have returned false & still function)
///
/// \param iProtect Protection settingss
///
/// \return true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool Function::ProtectRegion(uint32 iProtect)
{
	dword dwProt;
	this->m_iProtection = iProtect;

	return VirtualProtect(this->m_mbi.BaseAddress, this->m_mbi.RegionSize, iProtect, &dwProt) == 1 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool Function::RestoreRegion()
///
/// \brief Restores the region's protection settings.
///
/// \return true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool Function::RestoreRegion()
{
	return this->ProtectRegion(this->m_mbi.Protect);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint32 Function::GetRegionProtection()
///
/// \brief Gets the function's region's protection settings.
///
/// \return The function's region's protection settings.
////////////////////////////////////////////////////////////////////////////////////////////////////

uint32 Function::GetRegionProtection()
{
	return this->m_iProtection;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint32 Function::GetOrigRegionProtection()
///
/// \brief Gets the function's region's original protection settings.
///
/// \return The function's region's original protection settings.
////////////////////////////////////////////////////////////////////////////////////////////////////

uint32 Function::GetOrigRegionProtection()
{
	return this->m_mbi.Protect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void Function::ModifyMemory(uint32 iSize, char* cData, uint32 iOffset)
///
/// \brief Modifies the function's memory, starting at the base of the function + iOffset.
/// 	   iOffset defaults to 0, iSize is the amount of bytes to change, cData is the data
/// 	   to replace with.
///
/// \param iSize		  Size of cData
/// \param cData		  Data to write
/// \param iOffset		  Offset from the base to start writing, defaults 0 
////////////////////////////////////////////////////////////////////////////////////////////////////

void Function::SetMemory(uint32 iSize, uchar* cData, uint32 iOffset)
{
	if (!this->m_wpBase)
		return;

	memcpy( this->m_wpBase + iOffset, cData, iSize );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn char* Function::GetMemory(uint32 iSize, uint32 iOffset)
///
/// \brief Returns iSize bytes from the base address + iOffset.
///
/// \param iSize   The amount of data to read.
/// \param iOffset Offset from the base address, where the function copies memory from.
/// 			   Defaults 0.
///
/// \return Read memory
////////////////////////////////////////////////////////////////////////////////////////////////////

void Function::GetMemory(char* cBuffer, uint32 iSize, uint32 iOffset)
{
	if (!this->m_wpBase)
		return;

	memcpy(cBuffer, this->m_wpBase + iOffset, iSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn string Function::GetHash()
///
/// \brief Returns a hash of the function, see Mem::GetHash for more details.
///
/// \return The SHA-256 hash of GetFunctionData (see Function::GetFunctionData for more information
/// 		about what exactly this returns.)
////////////////////////////////////////////////////////////////////////////////////////////////////

string Function::GetHash(uint32 iSize)
{
	void* buf = malloc(iSize);
	this->GetMemory((char*)buf, iSize, 0);

	string hash = Mem::GetHash(string((char*)buf, iSize));
	free(buf);

	return hash;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint32* Function::GetReference()
///
/// \brief Returns a pointer to the function in memory.
///
/// \return .
////////////////////////////////////////////////////////////////////////////////////////////////////

dword* Function::GetReference()
{
	return this->m_wpBase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn uint32 Function::GetDereferenced()
///
/// \brief Gets the dereferenced pointer to the function in memory.
///
/// \return .
////////////////////////////////////////////////////////////////////////////////////////////////////

dword Function::GetDereferenced()
{
	if (!this->m_wpBase)
		return NULL;

	return *this->m_wpBase;
}

NAMESPACE_END1