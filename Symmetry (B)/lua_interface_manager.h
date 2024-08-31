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
NAMESPACE_LIB(Lua)

////////////////////////////////////////////////////////////////////////////////////////////////////
///											== OTHER ==
////////////////////////////////////////////////////////////////////////////////////////////////////

// These are the same as UID's passed into CLuaShared::CreateLuaInterface
enum InterfaceState
{
	MENU	= 2,
	SERVER	= 1,
	CLIENT	= 0
};

////////////////////////////////////////////////////////////////////////////////////////////////////
///											== CLASS ==
////////////////////////////////////////////////////////////////////////////////////////////////////

class LuaInterfaceManager
{
private:

	// A vector of every state stored
	vector<pair<void*, InterfaceState>>	m_vInterfaces;

public:

	void		AddInterface(void*, InterfaceState);

	void*		GetInterface(InterfaceState);
	lua_State*	GetState(InterfaceState);

	void		RemoveInterface(void*);

};

NAMESPACE_END2