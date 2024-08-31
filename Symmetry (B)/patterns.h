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
using namespace Lib;

#include "lua_gmod.h"

NAMESPACE_CORE(Patterns)

#undef LoadString

////////////////////////////////////////////////////////////////////////////////////////////////////
///								== ENUMS/STRUCTURES/TYPDEFS/MACROS == 
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Pattern
{
	string		sCodeStylePattern;
	string		sModule;
#if defined( BUILDMODE_BETA ) || defined( BUILDMODE_TEST )
	string		sName;
#endif
	void**	ppAddress;
};

#if defined( BUILDMODE_BETA ) || defined( BUILDMODE_TEST )

#define PATTERN(pattern, module, name, func)\
{\
	pattern, \
	module, \
	name, \
	(void**)func\
}

#else

// Still except name argument so tha the macro "call" is no different
// between buildmodes
#define PATTERN(pattern, module, name, func)\
{\
	pattern, \
	module, \
	(void**)func\
}

#endif

#define PatternPrototype(ret, convention, name, ...)\
	typedef ret(convention * T_##name)(__VA_ARGS__); \
	extern T_##name name;

#define PatternDefinition(name)\
	T_##name name;

// CLuaGamemode
PatternPrototype(bool, __thiscall, CLuaGamemode__Call, void*, const char* pszHookName);
PatternPrototype(bool, __thiscall, CLuaGamemode__CallWithArgs, void*, const char* pszHookName);
PatternPrototype(bool, __thiscall, CLuaGamemode__CallFinish, void*, int iArgCount);
PatternPrototype(bool, __thiscall, CLuaGamemode__CallReturns, void*, int iArgCount, int iReturnCount);

// CLuaShared
PatternPrototype(PDWORD*, __thiscall, CLuaShared__CreateLuaInterface, void*, uchar cIndex, bool);
PatternPrototype(void, __thiscall, CLuaShared__CloseLuaInterface, PDWORD* pdwpLuaInterface);

// Lua 
PatternPrototype(void, __cdecl, LuaPushNil, lua_State*);
PatternPrototype(void, __cdecl, LuaPushNumber, lua_State*, lua_Number);
PatternPrototype(void, __cdecl, LuaPushInteger, lua_State*, lua_Integer);
PatternPrototype(const char*, __cdecl, LuaPushString, lua_State*, const char*);
PatternPrototype(void, __cdecl, LuaPushClosure, lua_State*, lua_CFunction, int);
PatternPrototype(void, __cdecl, LuaPushBoolean, lua_State*, int);
PatternPrototype(void, __cdecl, LuaPushUserData, lua_State*, UserData*);
PatternPrototype(void, __cdecl, LuaPushValue, lua_State*, int);
PatternPrototype(void, __cdecl, LuaCreateTable, lua_State*, int, int);
PatternPrototype(void, __cdecl, LuaGetTable, lua_State*, int);
PatternPrototype(void, __cdecl, LuaSetTable, lua_State*, int);
PatternPrototype(void, __cdecl, LuaGetField, lua_State*, int, const char*);
PatternPrototype(void, __cdecl, LuaSetField, lua_State*, int, const char*);
PatternPrototype(lua_Number, __cdecl, LuaToNumber, lua_State*, int, int*);
PatternPrototype(lua_Integer, __cdecl, LuaToInteger, lua_State*, int, int*);
PatternPrototype(int, __cdecl, LuaToBoolean, lua_State*, int);
PatternPrototype(const char*, __cdecl, LuaToLString, lua_State*, int, size_t*);
PatternPrototype(lua_CFunction, __cdecl, LuaToCFunction, lua_State*, int);
PatternPrototype(UserData*, __cdecl, LuaToUserData, lua_State*, int);
PatternPrototype(const void*, __cdecl, LuaToPointer, lua_State*, int);
PatternPrototype(int, __cdecl, LuaType, lua_State*, int);
PatternPrototype(int, __cdecl, LuaCall, lua_State*, int, int, int, lua_CFunction);
PatternPrototype(int, __cdecl, LuaPCall, lua_State*, int, int, int, int, lua_CFunction);
PatternPrototype(int, __cdecl, LuaLoadString, lua_State*, const char*);
PatternPrototype(int, __cdecl, LuaLoadBuffer, lua_State*, const char*, size_t, const char*, const char*);
PatternPrototype(int, __cdecl, LuaRef, lua_State*, const char*);
PatternPrototype(void, __cdecl, LuaUnRef, lua_State*, int, int);
PatternPrototype(void, __cdecl, LuaRawGet, lua_State*, int);
PatternPrototype(void, __cdecl, LuaRawSet, lua_State*, int);
PatternPrototype(void, __cdecl, LuaRawGetI, lua_State*, int, int);
PatternPrototype(void, __cdecl, LuaRawSetI, lua_State*, int, int);
PatternPrototype(void, __cdecl, LuaSetTop, lua_State*, int);
PatternPrototype(int, __cdecl, LuaGetTop, lua_State*);
PatternPrototype(void, __cdecl, LuaCheckType, lua_State*, int, int);
PatternPrototype(void, __cdecl, LuaRemove, lua_State*, int);
PatternPrototype(void, __cdecl, LuaReplace, lua_State*, int);
PatternPrototype(int, __cdecl, LuaGC, lua_State*, int, int);

// CBasePlayer
PatternPrototype(void, __thiscall, SrcEntity__GetSteamID, SrcEntity*, CSteamID*);
PatternPrototype(char*, __thiscall, SrcEntity__GetPlayerName, SrcEntity*);

// CBaseEntity
PatternPrototype(void, __thiscall, SrcEntity__FireBullets, SrcEntity*, const FireBulletsInfo_t &fbInfo);
PatternPrototype(char*, __thiscall, SrcEntity__GetClassname, SrcEntity*);
PatternPrototype(void, __stdcall, SrcEntity__SetPredictionRandomSeed, CUserCmd*);
PatternPrototype(void, __thiscall, SrcEntity__GetVectors, SrcEntity*, Vector*, Vector*, Vector*);

// Client global
PatternPrototype(int, __cdecl, LuaCurTime, lua_State*);
PatternPrototype(SrcEntity*, __stdcall, LocalPlayer);

// CViewRender
PatternPrototype(void, __thiscall, CViewRender__Render, void*, vrect_t*);

// CUserCmd
PatternPrototype(CRC32_t, __thiscall, CUserCmd__GetChecksum, CUserCmd*);

// Non-function pointers
extern dword* CInput;					// global instance of CInput
extern dword* ClientModeShared;			// global instance of clientmodeshared
extern dword* CViewRender;				// global instance of CViewRender

////////////////////////////////////////////////////////////////////////////////////////////////////
///								           == OTHER SHIT == 
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///								        == NAMESPACE METHODS == 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool		Validate();
Pattern*	GetPattern(uint32);	
int			GetPatternCount();

NAMESPACE_END2