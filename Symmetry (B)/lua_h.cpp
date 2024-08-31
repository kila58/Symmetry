/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *  
  *                  ====================                      **/

#include "lua_h.h"
#include "lua_gmod.h"

#include "global.h"

NAMESPACE_CORE(Hooks)

lua_CFunction Orig_CollectGarbage;

lua_CFunction LUA_CollectGarbage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int collectgarbage()
////////////////////////////////////////////////////////////////////////////////////////////////////

int collectgarbage(lua_State* L)
{
	int args = lua_gettop(L);

	Class::HookManager->StartCall("OnLuaHookedFunction");

	// Push function name
	lua_pushstring(L, "collectgarbage");

	// Push original function
	lua_pushcfunction(L, LUA_CollectGarbage);

	// Push original arguments again
	for (int i = 1; i <= args; i++)
	{
		lua_pushvalue(L, 1);
		lua_remove(L, 1);
	}

	Class::HookManager->FinishCall(args + 2, LUA_MULTRET, false);

	// Are we overwriting returns?
	if (lua_gettop(L) > 0)
		return lua_gettop(L);

	// Otherwise call original, let them do the returning
	return LUA_CollectGarbage(L);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaH::Hook()
///
/// \brief	Hooks all the functions in this file.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaH::Hook(lua_State* L)
{
	lua_rawgeti(L, GMOD_REGINDEX, LUA_RIDX_GLOBALS);
	lua_getfield(L, -1, "collectgarbage");

	Orig_CollectGarbage = lua_tocfunction(L, -1);

	MH_CreateHook(Orig_CollectGarbage, collectgarbage, (void**)&LUA_CollectGarbage);
	MH_EnableHook(Orig_CollectGarbage);

	// Clean stack
	lua_pop(L, 2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaH::UnHook()
///
/// \brief	Unhooks all the functions in this file.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaH::UnHook()
{
	MH_DisableHook(Orig_CollectGarbage);
	MH_RemoveHook(Orig_CollectGarbage);
}

NAMESPACE_END2