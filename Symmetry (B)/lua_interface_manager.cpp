/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "lua_interface_manager.h"

#include "global.h"

#include "src_luashared.h"

NAMESPACE_LIB(Lua)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaInterfaceManager::AddInterface(void* vpInterface, InterfaceState isState)
///
/// \brief	Adds an interface vpInterface identified by isState.
///
/// \param [in,out]	vpInterface	The interface.
/// \param	isState			   	The interface's state.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaInterfaceManager::AddInterface(void* vpInterface, InterfaceState isState)
{
	this->m_vInterfaces.push_back(make_pair(vpInterface, isState));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void* LuaInterfaceManager::GetInterface(InterfaceState isState)
///
/// \brief	Returns an interface that's state matches isState.
///
/// \param	isState	State of the interface to find.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

void* LuaInterfaceManager::GetInterface(InterfaceState isState)
{
	for (uint32 i = 0; i < this->m_vInterfaces.size(); i++)
	{
		if (this->m_vInterfaces[i].second == isState)
		{
			return this->m_vInterfaces[i].first;
		}
	}

	// Resort to calling CLuaShared::GetLuaInterface
	return Src::LuaShared->GetLuaInterface((uchar)isState);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	lua_State* LuaInterfaceManager::GetState(InterfaceState isState)
///
/// \brief	Returns the first member of the class GetInterface returns.  The interface's
/// 		lua_State pointer is the first member.
///
/// \param	isState	State of the is.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

lua_State* LuaInterfaceManager::GetState(InterfaceState isState)
{
	void*		interf	= this->GetInterface(isState);
	lua_State*  state	= NULL;

	// If GetInterface returns 0 there's nothing we can do here
	if (interf == NULL)
		return NULL;

	__asm
	{
		MOV ECX, interf
		MOV EAX, [ECX + 0x4]
		MOV state, EAX
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaInterfaceManager::RemoveInterface(void* vpInterface)
///
/// \brief Searches this->m_vInterfaces
///
/// \param [in,out]	vpInterface	The interface to remove.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaInterfaceManager::RemoveInterface(void* vpInterface)
{
	for (uint32 i = 0; i < this->m_vInterfaces.size(); i++)
	{
		if (this->m_vInterfaces[i].first == vpInterface)
		{
			this->m_vInterfaces.erase(this->m_vInterfaces.begin() + i);
		}
	}
}

NAMESPACE_END2