/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "generic.h"

#include "export_utils.h"
#include "virtualcontainer.h"

#include "mem.h"

#include "manmap.h"

using namespace Lib;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(bool) Rtn_CheckMap()
///
/// \brief	Manual map/injection CHECKING routine.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(bool) Rtn_CheckMap()
{
	// Currently, all we do here is check for the game process, we should probably check if it's
	// ready too, since as if you inject too early it crashes.  Haven't bothered investigating
	// as to why this happens
	
	return Mem::GetProcessIDByName(GAME_PROCNAME) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	EXPORT(bool) Rtn_Map(IO::VirtualContainer* vContainer)
///
/// \brief	Manual map/injection routine.
////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT(HANDLE) Rtn_Map(IO::VirtualContainer* vContainer)
{
	// We need AES keys
	if (Mem::IsSectionNull(&vContainer->GetKeySet(), sizeof(IO::AES256KeySet)))
		return INVALID_HANDLE_VALUE;

	dword dwProcID = Mem::GetProcessIDByName(GAME_PROCNAME);
	
	if (!dwProcID)
		return INVALID_HANDLE_VALUE;

	for (IO::VirtualFile* v : vContainer->GetList())
	{
		if (v->Mapped)
		{
			string data = v->Data, decoded = "";

			if (v->Encrypted)
			{
				// Decrypt if required
				data = Mem::AESDecrypt(v->Data, vContainer->GetKeySet().Key, 
					vContainer->GetKeySet().IV);
			}

			return ManualMap::Map(dwProcID, data);;
		}
	}

	// Nothing was mapped
	return false;
}