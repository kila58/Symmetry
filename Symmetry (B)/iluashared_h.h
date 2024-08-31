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

NAMESPACE_CORE(Hooks)

#define LUASHARED_CREATEINTERFACE	4
#define LUASHARED_CLOSEINTERFACE	5

// Typedefs
typedef int		(*LoadBufferX_t)(lua_State*, char_c*, size_t, char_c*, char_c*);

// Used in lua.cpp
extern LoadBufferX_t CLS_LoadBufferX;

// Generic hooking method
class LuaSharedH
{
public:
	static void	Hook();
	static void UnHook();
};

NAMESPACE_END2