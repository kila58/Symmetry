/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "patterns.h"

#include "convar.h"

#include "filea.h"
#include "global.h"

using namespace Core::Console;
using namespace Core;

NAMESPACE_CORE(Patterns)

////////////////////////////////////////////////////////////////////////////////////////////////////
///								     == EXTREN DEFINITIONS == 
////////////////////////////////////////////////////////////////////////////////////////////////////

// CLuaGamemode
PatternDefinition(CLuaGamemode__Call);
PatternDefinition(CLuaGamemode__CallWithArgs);
PatternDefinition(CLuaGamemode__CallFinish);
PatternDefinition(CLuaGamemode__CallReturns);

// CLuaShared
PatternDefinition(CLuaShared__CreateLuaInterface);
PatternDefinition(CLuaShared__CloseLuaInterface);

// Lua 
PatternDefinition(LuaPushNil);
PatternDefinition(LuaPushNumber);
PatternDefinition(LuaPushInteger);
PatternDefinition(LuaPushString);
PatternDefinition(LuaPushClosure);
PatternDefinition(LuaPushBoolean);
PatternDefinition(LuaPushUserData);
PatternDefinition(LuaPushValue);
PatternDefinition(LuaCreateTable);
PatternDefinition(LuaGetTable);
PatternDefinition(LuaSetTable);
PatternDefinition(LuaGetField);
PatternDefinition(LuaSetField);
PatternDefinition(LuaToNumber);
PatternDefinition(LuaToInteger);
PatternDefinition(LuaToBoolean);
PatternDefinition(LuaToLString);
PatternDefinition(LuaToCFunction);
PatternDefinition(LuaToUserData);
PatternDefinition(LuaToPointer);
PatternDefinition(LuaType);
PatternDefinition(LuaCall);
PatternDefinition(LuaPCall);
PatternDefinition(LuaLoadString);
PatternDefinition(LuaLoadBuffer);
PatternDefinition(LuaRef);
PatternDefinition(LuaUnRef);
PatternDefinition(LuaRawGet);
PatternDefinition(LuaRawSet);
PatternDefinition(LuaRawGetI);
PatternDefinition(LuaRawSetI);
PatternDefinition(LuaSetTop);
PatternDefinition(LuaGetTop);
PatternDefinition(LuaCheckType);
PatternDefinition(LuaRemove);
PatternDefinition(LuaReplace);
PatternDefinition(LuaGC);

// SrcEntity
PatternDefinition(SrcEntity__GetSteamID);
PatternDefinition(SrcEntity__GetPlayerName);
PatternDefinition(SrcEntity__FireBullets);
PatternDefinition(SrcEntity__GetClassname);
PatternDefinition(SrcEntity__SetPredictionRandomSeed);
PatternDefinition(SrcEntity__GetVectors);

// Client global
PatternDefinition(LuaCurTime);
PatternDefinition(LocalPlayer);

// CViewRender
PatternDefinition(CViewRender__Render);

// CUserCmd
PatternDefinition(CUserCmd__GetChecksum);

dword* CInput;					// global instance of CInput
dword* ClientModeShared;		// global instance of clientmodeshared
dword* CViewRender;				// global instance of CViewRender

////////////////////////////////////////////////////////////////////////////////////////////////////
///									   == PATTERN TABLE == 
////////////////////////////////////////////////////////////////////////////////////////////////////

Pattern g_pPatterns[] = 
{
	// == ::CLuaGamemode

	/* ::Call				*/ PATTERN("55 8b ec 56 8b f1 ff", "client.dll", "CLuaGamemode::Call", &CLuaGamemode__Call),
	/* ::CallWithArgs		*/ PATTERN("55 8b ec 53 56 8b f1 8b 0d ?? ?? ?? ?? 83 b9 0c 10 00 00 00 0f 95 c3 84 db 74 ?? 6a 04 6a 00 68 ?? ?? ?? ?? 6a 00 68 ?? ?? ?? ?? ff 15 ?? ?? ?? ?? ff 15 ?? ?? ?? ?? 84 c0 75 ?? 8b 45 08 50 68 ?? ?? ?? ?? ff 15 ?? ?? ?? ?? 8b 16 8b 82 8c", "client.dll", "CLuaGamemode::CallWithArgs", &CLuaGamemode__CallWithArgs),
	/* ::CallFinish			*/ PATTERN("55 8b ec 8b 0d ?? ?? ?? ?? 8b 55", "client.dll", "CLuaGamemode::CallFinish", &CLuaGamemode__CallFinish),
	/* ::CallReturns		*/ PATTERN("55 8b ec 8b 55 0c 8b 0d", "client.dll", "CLuaGamemode::CallRetuns", &CLuaGamemode__CallReturns),

	// == CLuaShared

	/* ::CreateLuaInterface */ PATTERN("55 8b ec 8b 45 0c 56 50", "lua_shared.dll", "CLuaShared::CreateLuaInterface", &CLuaShared__CreateLuaInterface),
	/* ::CloseLuaInterface	*/ PATTERN("55 8b ec 56 8b 75 08 8b 06 8b 90 c0", "lua_shared.dll", "CLuaShared::CloseLuaInterface", &CLuaShared__CloseLuaInterface),

	// == Global lua_shared.dll functions

	/* lua_pushnil			*/ PATTERN("8b 4c 24 04 8b 41 14 c7", "lua_shared.dll", "lua_pushnil", &LuaPushNil),
	/* lua_pushnumber		*/ PATTERN("8b 4c 24 04 dd", "lua_shared.dll", "lua_pushnumber", &LuaPushNumber),
	/* lua_pushinteger		*/ PATTERN("8b 4c 24 04 db", "lua_shared.dll", "lua_pushinteger", &LuaPushInteger),
	/* lua_pushstring		*/ PATTERN("56 8b 74 24 08 57 8b 7c 24 10 85", "lua_shared.dll", "lua_pushstring", &LuaPushString),
	/* lua_pushcclosure		*/ PATTERN("57 8b 7c 24 08", "lua_shared.dll", "lua_pushcclosure", &LuaPushClosure),
	/* lua_pushboolean		*/ PATTERN("8b 4c 24 04 33", "lua_shared.dll", "lua_pushboolean", &LuaPushBoolean),
	/* lua_pushlightuserdata*/ PATTERN("8b 4c 24 04 8b 41 14 8b", "lua_shared.dll", "lua_pushlightuserdata", &LuaPushUserData),
	/* lua_pushvalue		*/ PATTERN("8b 44 24 08 56 8b", "lua_shared.dll", "lua_pushvalue", &LuaPushValue),
	/* lua_createtable		*/ PATTERN("56 8b 74 24 08 8b 46 08 8b 48 14 3b 48 18 72 ?? 8b ce e8 ?? ?? ?? ?? 8b 44 24 10", "lua_shared.dll", "lua_createtable", &LuaCreateTable),
	/* lua_gettable			*/ PATTERN("8b 44 24 08 56 8b 74 24 08 8b d6 e8 ?? ?? ?? ?? 8b 4e", "lua_shared.dll", "lua_gettable", &LuaGetTable),
	/* lua_settable			*/ PATTERN("8b 44 24 08 56 8b 74 24 08 8b d6 e8 ?? ?? ?? ?? 8b 4e 14 83 e9 10", "lua_shared.dll", "lua_settable", &LuaSetTable),
	/* lua_getfield			*/ PATTERN("55 8b ec 83 e4 f8 8b 45 0c 83", "lua_shared.dll", "lua_getfield", &LuaGetField),
	/* lua_setfield			*/ PATTERN("55 8b ec 83 e4 f8 8b 45 0c 83 ec 0c 53 56 8b 75 08 57 8b d6 e8 ?? ?? ?? ?? 8b 55 10 8b f8 8b c2 8d 58 01 8a 08 40 84 c9 75 ?? 2b c3 50 52 56 e8 ?? ?? ?? ?? 89 44 24 1c 8d 44 24 1c 50 57 56 c7 44 24 2c fb ff ff ff e8 ?? ?? ?? ?? 83 c4 18 85 c0 74", "lua_shared.dll", "lua_setfield", &LuaSetField),
	/* lua_tonumber			*/ PATTERN("55 8b ec 83 e4 f8 8b 45 0c 8b 55 08 83 ec 08 e8 ?? ?? ?? ?? 8b 48 04 83 f9 f2 77", "lua_shared.dll", "lua_tonumber", &LuaToNumber),
	/* lua_tointeger		*/ PATTERN("55 8b ec 83 e4 f8 8b 45 0c 8b 55 08 83 ec 08 e8 ?? ?? ?? ?? 8b 48 04 83 f9 f2 73", "lua_shared.dll", "lua_tointeger", &LuaToInteger),
	/* lua_toboolean		*/ PATTERN("8b 44 24 08 8b 54 24 04 e8 ?? ?? ?? ?? 83 78 04 fe", "lua_shared.dll", "lua_toboolean", &LuaToBoolean),
	/* lua_tolstring		*/ PATTERN("56 8b 74 24 0c 57", "lua_shared.dll", "lua_tolstring", &LuaToLString),
	/* lua_tocfunction		*/ PATTERN("8b 44 24 08 8b 54 24 04 e8 ?? ?? ?? ?? 83 78 04 f7 75 ?? 8b 08", "lua_shared.dll", "lua_tocfunction", &LuaToCFunction),
	/* lua_touserdata		*/ PATTERN("8b 44 24 08 8b 54 24 04 e8 ?? ?? ?? ?? 8b 48", "lua_shared.dll", "lua_touserdata", &LuaToUserData),
	/* lua_topointer		*/ PATTERN("8b 44 24 08 8b 54 24 04 e8 ?? ?? ?? ?? 8b 48 04 83 f9 f3 75 ?? 8b 00 83 c0 18 c3 83 f9 fc 75 ?? 8b 00 c3 83", "lua_shared.dll", "lua_topointer", &LuaToPointer),
	/* lua_type				*/ PATTERN("8b 44 24 08 56 8b 74 24 08 8b d6 e8 ?? ?? ?? ?? 8b 48", "lua_shared.dll", "lua_type", &LuaType),
	/* lua_call				*/ PATTERN("8b 44 24 0c 8b", "lua_shared.dll", "lua_call", &LuaCall),
	/* lua_pcall			*/ PATTERN("8b 44 24 04 8b", "lua_shared.dll", "lua_pcall", &LuaPCall),
	/* luaL_loadstring		*/ PATTERN("8b 54 24 08 8b c2", "lua_shared.dll", "luaL_loadstring", &LuaLoadString),
	/* luaL_loadbufferx		*/ PATTERN("55 8b ec 83 e4 f8 83 ec 78", "lua_shared.dll", "luaL_loadbufferx", &LuaLoadBuffer),
	/* luaL_ref				*/ PATTERN("53 8b 5c 24 0c 8d", "lua_shared.dll", "luaL_ref", &LuaRef),
	/* luaL_unref			*/ PATTERN("53 8b 5c 24 10 85 db 78", "lua_shared.dll", "luaL_unref", &LuaUnRef),
	/* law_rawget			*/ PATTERN("8b 44 24 08 56 8b 74 24 08 8b d6 e8 ?? ?? ?? ?? 8b 4e 14 8b", "lua_shared.dll", "law_rawget", &LuaRawGet),
	/* lua_rawset			*/ PATTERN("8b 44 24 08 53", "lua_shared.dll", "lua_rawset", &LuaRawSet),
	/* lua_rawgeti			*/ PATTERN("8b 44 24 08 56 8b 74 24 08 8b d6 e8 ?? ?? ?? ?? 8b 08", "lua_shared.dll", "lua_rawgeti", &LuaRawGetI),
	/* lua_rawseti			*/ PATTERN("8b 44 24 08 56 57 8b 7c 24 0c 8b d7 e8 ?? ?? ?? ?? 8b 30 8b 44 24 14", "lua_shared.dll", "lua_rawseti", &LuaRawSetI),
	/* lua_settop			*/ PATTERN("8b 54 24 08 85", "lua_shared.dll", "lua_settop", &LuaSetTop),
	/* lua_gettop			*/ PATTERN("8b 4c 24 04 8b", "lua_shared.dll", "lua_gettop", &LuaGetTop),
	/* luaL_checktype		*/ PATTERN("56 8b 74 24 08 57", "lua_shared.dll", "luaL_checktype", &LuaCheckType),
	/* lua_remove			*/ PATTERN("8b 44 24 08", "lua_shared.dll", "lua_remove", &LuaRemove),
	/* lua_replace			*/ PATTERN("53 8b 5c", "lua_shared.dll", "lua_replace", &LuaReplace),
	/* lua_gc				*/ PATTERN("8b 44 24 08 53 8b 5c 24 08 56 8b", "lua_shared.dll", "lua_gc", &LuaGC),

	// == CBasePlayer

	/* ::GetSteamID			*/ PATTERN("55 8b ec a1 ?? ?? ?? ?? 81 ec ?? ?? ?? ?? 56 8b 30 8b 41 08 83 c1 08 8d 95 7c ff ff ff 52 8b 50 24 ff d2 8b 0d ?? ?? ?? ?? 50 8b 46 20 ff d0 5e 84 c0 74", "client.dll", "SrcEntity::GetSteamID", &SrcEntity__GetSteamID),
	/* ::GetPlayerName		*/ PATTERN("a1 ?? ?? ?? ?? 85 c0 74 ?? 56 8b b0", "client.dll", "SrcEntity::GetPlayerName", &SrcEntity__GetPlayerName),

	// == CBaseEntity

	/* ::FireBullets		*/ PATTERN("53 8b dc 83 ec 08 83 e4 f0 83 c4 04 55 8b 6b 04 89 6c 24 04 8b ec 81 ec ?? ?? ?? ?? 56 8b", "client.dll", "SrcEntity::FireBullets", &SrcEntity__FireBullets),
	/* ::GetClassname		*/ PATTERN("57 8b f9 8b 87 1c", "client.dll", "SrcEntity::GetClassname", &SrcEntity__GetClassname),
	/* ::SetPredictionRandomSeed*/ PATTERN("55 8b ec 8b 45 08 85 c0 75 ?? c7", "client.dll", "SrcEntity::SetPredictionRandomSeed", &SrcEntity__SetPredictionRandomSeed),
	/* ::GetVectors			*/ PATTERN("55 8b ec 56 57 8b f1 e8 ?? ?? ?? ?? 8b 45", "client.dll", "SrcEntity::GetVectors", &SrcEntity__GetVectors),

	// Global client.dll functions

	/* CurTime				*/ PATTERN("55 8b ec 8b 45 08 8b 48 48 8b 11 50 8b 82 c4 01 00 00 ff d0 8b 0d ?? ?? ?? ?? a1 ?? ?? ?? ?? 8b 11 d9 40 0c", "client.dll", "_G.CurTime", &LuaCurTime),
	/* LocalPlayer			*/ PATTERN("55 8b ec 8b 45 08 8b 48 48 8b 11 50 8b 82 c4 01 00 00 ff d0 e8 ?? ?? ?? ?? 50", "client.dll", "_G.LocalPlayer", &LocalPlayer),

	// == CViewRender
	
	/* ::Render				*/ PATTERN("55 8b ec 81 ec ?? ?? ?? ?? 53 56 57 8b f9 8b 0d ?? ?? ?? ?? 83 b9 0c 10 00 00 00 89 7d c8", "client.dll", "CViewRender::Render", &CViewRender__Render),

	// == CUserCmd
	
	/* ::GetChecksum		*/ PATTERN("55 8b ec 51 56 8d", "client.dll", "CUserCmd::GetChecksum", &CUserCmd__GetChecksum),

	// == Other (not functions)

	/* CInput instance		*/ PATTERN("8b 0d ?? ?? ?? ?? 8b 01 8b 40 0c 52 8b 55 08 51 d9 1c 24", "client.dll", "CInput", &CInput),
	/* CModeShared instance	*/ PATTERN("8b 0d ?? ?? ?? ?? 8b 11 8b 42 ?? ff d0 50 8b cf", "client.dll", "ClientModeShared", &ClientModeShared),
	/* CViewRender instance */ PATTERN("8b 0d ?? ?? ?? ?? 85 c9 75 ?? b9", "client.dll", "CViewRender", &CViewRender)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool Validate()
///
/// \brief Validates the patterns in g_pPatterns as shown above.
///
/// \return If p_hash_action == 0 this function will always return true
/// 		unless one or more patterns didn't resolve at all.  If p_hash_action == 1 this
/// 		function will return false if on or more patterns resolve to a function
/// 		that doesn't match the hash stored for that function.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool Validate()
{
	for (int i = 0; i < GetPatternCount(); i++)
	{
		Mem::Signature sig = Mem::MakeSignature(g_pPatterns[i].sCodeStylePattern, g_pPatterns[i].sModule);
		dword*	address = (dword*)Mem::ResolveSignature(sig);

		free(sig.cMask);
		free(sig.cPattern);

		string	name = "SIG[" + to_string(i) + "]";

#if defined( BUILDMODE_TEST ) || defined( BUILDMODE_BETA )
		name = g_pPatterns[i].sName.c_str();
#endif

		if (!(*g_pPatterns[i].ppAddress = address))
		{
			PrintBadL("%s did not resolve!", name.c_str());
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn Pattern* GetPattern(uint32 iIndex)
///
/// \brief Returns a pattern by index in g_pPatterns.  If iIndex does not exist inside 
/// 	   g_pPatterns, NULL is returned.
///
/// \param iIndex Index of the target pattern inside g_pPatterns.
///
/// \return .
////////////////////////////////////////////////////////////////////////////////////////////////////

Pattern* GetPattern(uint32 iIndex)
{
	return GetPatternCount() > (int)iIndex ? &g_pPatterns[iIndex] : NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn int GetPatternCount()
///
/// \brief Returns the amount of patterns inside g_pPatterns, using 
/// 	   sizeof( g_pPatterns ) / sizeof( Pattern )
///
/// \return The amount of patterns contained in g_pPatterns.
////////////////////////////////////////////////////////////////////////////////////////////////////

int	GetPatternCount()
{
	return sizeof( g_pPatterns ) / sizeof( Pattern );
}

NAMESPACE_END2