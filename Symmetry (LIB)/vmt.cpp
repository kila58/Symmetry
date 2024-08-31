/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "vmt.h"
#include "mem.h"

NAMESPACE_LIB(Detours)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn VMT::VMT(PDWORD* ppdwBase)
///
/// \brief Constructor.
///
/// \param [in,out] ppdwBase Pointer to the target VMT
////////////////////////////////////////////////////////////////////////////////////////////////////

VMT::VMT(PDWORD* ppdwBase)
{
	this->m_ppdwVMTBase = ppdwBase;

	this->m_dwSize		= this->GetMethodCount();

	this->m_pdwOld		= *ppdwBase;
	this->m_pdwNew		= new dword[this->m_dwSize];

	memcpy(this->m_pdwNew, this->m_pdwOld, sizeof( dword ) * this->m_dwSize);
	*ppdwBase = this->m_pdwNew;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn VMT::~VMT()
///
/// \brief Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

VMT::~VMT()
{
	delete[this->m_dwSize] this->m_pdwNew;
	this->UnHook();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn dword VMT::Hook(Function* fFunc, UINT uiIndex)
///
/// \brief Replaces a pointer to a function in the VMT.
///
/// \param [in,out] fFunc The new function.
/// \param uiIndex		  The index of the target function in the VMT.
///
/// \return Pointer to the old function.
////////////////////////////////////////////////////////////////////////////////////////////////////

dword VMT::Hook(dword wFunc, UINT uiIndex)
{
	if ( this->m_pdwNew && this->m_pdwOld && uiIndex <= this->m_dwSize )
	{
		this->m_pdwNew[ uiIndex ] = wFunc;
		return this->m_pdwOld[ uiIndex ];
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void VMT::UnHook()
///
/// \brief Restores the entire VMT to it's state before any hooks were made.  All hooks will not
/// 	   run after this is called.  Call ReHook to bring back the new VMT.
////////////////////////////////////////////////////////////////////////////////////////////////////

void VMT::UnHook()
{
	*this->m_ppdwVMTBase = this->m_pdwOld;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void VMT::ReHook()
///
/// \brief Restores the new VMT, call this after calling UnHook
////////////////////////////////////////////////////////////////////////////////////////////////////

void VMT::ReHook()
{
	*this->m_ppdwVMTBase = this->m_pdwNew;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn dword VMT::GetMethodCount()
///
/// \brief Returns the number of elements in the original VMT, using IsBadCodePtr to determine
/// 	   whether or not an index is invalid.
///
/// \return The number of elements in the original VMT
////////////////////////////////////////////////////////////////////////////////////////////////////

uint32 VMT::GetMethodCount()
{
	PDWORD pdwVMT = *this->m_ppdwVMTBase;

	for (dword dwIndex = 0; true; dwIndex++)
	{
		if ( IsBadCodePtr( (FARPROC)pdwVMT[dwIndex] ) )
		{
			return dwIndex;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	Function VMT::GetMethod(uint32 uiIndex)
///
/// \brief	Returns a function by index, in the VMT.
///
/// \param	uiIndex	The index.
///
/// \return	The method.
////////////////////////////////////////////////////////////////////////////////////////////////////

dword* VMT::GetMethod(uint32 uiIndex)
{
	if (uiIndex <= this->m_dwSize && this->m_pdwOld)
	{
		return (dword*)this->m_pdwOld[uiIndex];
	}

	return NULL;
}

NAMESPACE_END2