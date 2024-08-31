/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "iluagamemode_h.h"

#include "global.h"

#include "patterns.h"

NAMESPACE_CORE(Hooks)

Call_t			CLG_Call;
CallWithArgs_t	CLG_CallWithArgs;
CallFinish_t	CLG_CallFinish;
CallReturns_t	CLG_CallReturns;

void*			CLGVMT = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void __stdcall Call(const char* cName)
///
/// \brief	Hooks CLuaGamemode::Call.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __stdcall Call(const char* cName)
{
	void* vmt; GetReg(ECX, vmt);
	CLGVMT = vmt;

	if (Class::HookManager->IsReady())
	{
		Class::HookManager->Call(cName);
	}
	else 
		CLG_Call(vmt, cName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void __stdcall CallWithArgs(const char* cName)
///
/// \brief	Hooks CLuaGamemode::CallWithArgs.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __stdcall CallWithArgs(const char* cName)
{
	void* vmt; GetReg(ECX, vmt);
	CLGVMT = vmt;

	CLG_CallWithArgs(vmt, cName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void __stdcall CallFinish(int iArgs)
///
/// \brief	Hooks CLuaGamemode::CallFinish.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __stdcall CallFinish(int iArgs)
{
	void* vmt; GetReg(ECX, vmt);
	CLGVMT = vmt;

	if (Class::HookManager->IsReady())
		Class::HookManager->FinishCall(iArgs, 0, true);

	return CLG_CallFinish(vmt, iArgs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void __stdcall CallReturns(int iArgs, int iReturn)
///
/// \brief	Hooks CLuaGamemode::CallReturns.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __stdcall CallReturns(int iArgs, int iReturn)
{
	void* vmt; GetReg(ECX, vmt);
	CLGVMT = vmt;

	if (Class::HookManager->IsReady())
		Class::HookManager->FinishCall(iArgs, iReturn, true);

	return CLG_CallReturns(vmt, iArgs, iReturn);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaGamemodeH::Hook()
///
/// \brief	Hooks all the function in this file.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaGamemodeH::Hook()
{
	MH_CreateHook(Patterns::CLuaGamemode__Call, &Call,
		reinterpret_cast<void**>(&CLG_Call));
	MH_EnableHook(Patterns::CLuaGamemode__Call);

	MH_CreateHook(Patterns::CLuaGamemode__CallWithArgs, &CallWithArgs,
		reinterpret_cast<void**>(&CLG_CallWithArgs));
	MH_EnableHook(Patterns::CLuaGamemode__CallWithArgs);

	MH_CreateHook(Patterns::CLuaGamemode__CallFinish, &CallFinish,
		reinterpret_cast<void**>(&CLG_CallFinish));
	MH_EnableHook(Patterns::CLuaGamemode__CallFinish);

	MH_CreateHook(Patterns::CLuaGamemode__CallReturns, &CallReturns,
		reinterpret_cast<void**>(&CLG_CallReturns));
	MH_EnableHook(Patterns::CLuaGamemode__CallReturns);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaGamemodeH::UnHook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaGamemodeH::UnHook()
{
	CLGVMT = NULL;

	MH_DisableHook(Patterns::CLuaGamemode__Call);
	MH_RemoveHook(Patterns::CLuaGamemode__Call);

	MH_DisableHook(Patterns::CLuaGamemode__CallWithArgs);
	MH_RemoveHook(Patterns::CLuaGamemode__CallWithArgs);

	MH_DisableHook(Patterns::CLuaGamemode__CallFinish);
	MH_RemoveHook(Patterns::CLuaGamemode__CallFinish);

	MH_DisableHook(Patterns::CLuaGamemode__CallReturns);
	MH_RemoveHook(Patterns::CLuaGamemode__CallReturns);
}

NAMESPACE_END2