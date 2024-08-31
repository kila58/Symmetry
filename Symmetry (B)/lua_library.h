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
NAMESPACE_CORE(Lua)

// Quick macros
#define LFUNC(_func_) int _func_(lua_State* L)
#define LUA_BIND(name, func) lua_pushcfunction(L, func); lua_setfield(L, -2, name)

// Pushes the lua library onto the stack
class Library
{
public:
	static int	m_iSymTable;
public:
	static void Initialize(lua_State*);
};

NAMESPACE_END2