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

// Hook typedefs
typedef void(__thiscall* Call_t)(void* vmt, const char* cName);
typedef void(__thiscall* CallWithArgs_t)(void* vmt, const char* cName);
typedef void(__thiscall* CallFinish_t)(void* vmt, int iArgs);
typedef void(__thiscall* CallReturns_t)(void* vmt, int iArgs, int iReturn);

// Externs, so we can call this from places
extern Call_t			CLG_Call;
extern CallWithArgs_t	CLG_CallWithArgs;
extern CallFinish_t		CLG_CallFinish;
extern CallReturns_t	CLG_CallReturns;

// Generic hooking method (only supports one hook)
class LuaGamemodeH
{
public:
	static void	Hook();
	static void UnHook();
};

NAMESPACE_END2