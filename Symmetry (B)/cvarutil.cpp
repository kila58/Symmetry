/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "cvarutil.h"
#include "global.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CvarUtil::SetCvar(ConVar* pConVar, string sCvarValue)
///
/// \brief	Sets a cvar manually, setting m_fValue, m_nValue, m_pszString and 
/// 		m_StringLength.  No callbacks are called.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CvarUtil::SetCvar(ConVar* pConVar, string sCvarValue)
{
	// Convar is definitely okay if this code runs
	pConVar->m_fValue = (float)atof(sCvarValue.c_str());
	pConVar->m_nValue = (int)pConVar->m_fValue;

	if (!(pConVar->m_nFlags & FCVAR_NEVER_AS_STRING))
	{
		int len = (int)sCvarValue.length() + 1;

		// Change string value
		if (len != pConVar->m_StringLength)
		{
			if (pConVar->m_pszString)
			{
				Src::MemAlloc->Free(pConVar->m_pszString);
			}

			pConVar->m_pszString = (char*)Src::MemAlloc->Alloc(len);
			pConVar->m_StringLength = len;
		}

		// Copy
		memcpy(pConVar->m_pszString, sCvarValue.c_str(), len);
	}
}