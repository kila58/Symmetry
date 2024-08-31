/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "lua_hook_manager.h"

#include "global.h"

#include "lua_gmod.h"

NAMESPACE_CORE(Lua)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void HookManager::SetCallFunction(int iFunction)
///
/// \brief	Sets the reference to the function that's used to process hooks in Lua.
/// 		
/// 		This function will also cache the CLIENT Lua state since as this is a nice place to
/// 		do it.
///
/// \param	iFunction	Reference to the function.
////////////////////////////////////////////////////////////////////////////////////////////////////

void HookManager::SetCallFunction(int iFunction)
{
	this->m_iCallFunction = iFunction;

	// Cache the Lua state here too, when we set the call function,
	// we'll definitely have the Lua client state
	this->m_sCache = Class::StateManager->GetState(CLIENT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void HookManager::Call(char_c* cName)
///
/// \brief	Calls a hook that doesn't require arguments nor return values.
/// 		
///
/// \param [in,out]	cName	Name of the hook.
///
/// \return	See above.
////////////////////////////////////////////////////////////////////////////////////////////////////

void HookManager::Call(char_c* cName)
{
	if (!this->IsReady())
		return;

	lua_State* L = this->m_sCache;

	// Push our function onto the stack
	lua_rawgeti(L, GMOD_REGINDEX, this->m_iCallFunction);

	// Push a second copy and re-reference it
	lua_pushvalue(L, -1);
	this->m_iCallFunction = luaL_ref(L, GMOD_REGINDEX);

	// Push the name of the hook
	lua_pushstring(L, cName);

	// Call the hook
	lua_call(L, 1, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void HookManager::StartCall(char_c* cName)
///
/// \brief	Starts a call to a hook by placing the Lua hook processor function and the name of 
/// 		the hook on the stack.  Arguments should be placed on the stack after this, followed
/// 		by a call to HookManager::FinishCall, which will call the Lua hook processor.
/// 		
/// \param [in,out]	cName	If non-null, the name.
////////////////////////////////////////////////////////////////////////////////////////////////////

void HookManager::StartCall(char_c* cName)
{
	if (!this->IsReady())
		return;

	lua_State* L = this->m_sCache;

	// Push our function onto the stack
	lua_rawgeti(L, GMOD_REGINDEX, this->m_iCallFunction);

	// Push a second copy and re-reference it
	lua_pushvalue(L, -1);
	this->m_iCallFunction = luaL_ref(L, GMOD_REGINDEX);
	
	lua_pushstring(L, cName);	// hook 
	lua_pushnil(L);				// gamemode
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void HookManager::FinishCall(int iArgCount, int iReturnCount, bool bIsGameHook)
///
/// \brief	Finishes a hook call by calling lua_call.  if bIsGameHook is true, then this will go back
///         iArgCount into the stack and replace the call funtion with symmetry's own.
///
/// \param	iArgCount   	Number of arguments.
/// \param	iReturnCount	Number of returns.
/// \param	bIsGameHook 	True if the hook is a game hook, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////

void HookManager::FinishCall(int iArgCount, int iReturnCount, bool bIsGameHook)
{
	if (!this->IsReady())
		return;

	lua_State* L = this->m_sCache;

	if (bIsGameHook)
	{
		int stackPos = -1;

		// Apparently, the first function on the stack is always
		// the hook.Call function except when it's not, so to avoid 
		// stack-searching for each hook, we'll check if the first 
		// item on the stack is what it's meant to be, if it's not 
		// we can search the stack for the function
		if (lua_type(L, 1) != LUA_TFUNCTION)
		{
			stackPos = 1;
		}
		else
		{
			// There's never two functions on the stack
			for (int i = 1; i <= lua_gettop(L); i++)
			{
				if (lua_type(L, i) == LUA_TFUNCTION)
				{
					stackPos = i;
					break;
				}
			}
		}

		// Dunno why this would happen
		if (stackPos == -1)
			return;

		// Push our function onto the stack. 
		lua_rawgeti(L, GMOD_REGINDEX, this->m_iCallFunction);

		// Push a second copy and re-reference it
		lua_pushvalue(L, -1);
		this->m_iCallFunction = luaL_ref(L, GMOD_REGINDEX);

		// Replace the hook.Call function
		lua_replace(L, stackPos);
	}
	else
		lua_call(this->m_sCache, iArgCount + 2, iReturnCount);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void HookManager::PushArg(void* vArg, int iType)
///
/// \brief	Pushes an argument onto the Lua stack.  The point of this function is so that
/// 		you don't need to reobtain the CLIENT Lua state when pushing hook arguments.
///
/// \param [in,out]	vArg	If non-null, the argument.
/// \param	iType			Zero-based index of the type.
////////////////////////////////////////////////////////////////////////////////////////////////////

void HookManager::PushArg(void* vArg, int iType)
{
	switch (iType)
	{
	case LUA_TNIL:
		lua_pushnil(this->m_sCache);
		break;
	case LUA_TBOOLEAN:
		lua_pushboolean(this->m_sCache, (int)vArg == 1 ? true : false);
		break;
	case LUA_TLIGHTUSERDATA:
		lua_pushlightuserdata(this->m_sCache, vArg);
		break;
	case LUA_TNUMBER:
		lua_pushnumber(this->m_sCache, *((double*)vArg));
		break;
	case LUA_TSTRING:
		lua_pushstring(this->m_sCache, (const char*)vArg);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool HookManager::IsReady()
///
/// \brief	Returns true if there's a Lua processing function registered.
///
/// \return	true if ready, false if not.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool HookManager::IsReady()
{
	return this->m_iCallFunction != 0;
}

NAMESPACE_END2
