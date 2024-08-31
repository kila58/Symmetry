/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                   /____/     *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once

#include "lua_library.h"

#include "patterns.h"
#include "global.h"

#include "iluagamemode_h.h"

#include "lua_gmod.h"

#include "filea.h"

#include "console_manager.h"
#include "concommand.h"
#include "symconvar.h"

#include "src_lib.h"

#include "symaimbot.h"

using namespace Core::Patterns;

int Library::m_iSymTable = NULL;

#pragma region IO

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(FileExists)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(FileExists)
{
	lua_pushboolean(L, FileExistsG(luaL_checkstring(L, 1)));
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(DirectoryExists)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(DirectoryExists)
{
	lua_pushboolean(L, DirectoryExistsG(luaL_checkstring(L, 1)));
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(CreateFile)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(CreateFile)
{
	lua_pushboolean(L, CreateFileG(luaL_checkstring(L, 1)));
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(CreateDirectory)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(CreateDirectory)
{
	lua_pushboolean(L, CreateDirectoryG(luaL_checkstring(L, 1)));
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(WriteFile)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(WriteFile)
{
	lua_pushboolean(L, WriteFileG(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(ReadFile)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(ReadFile)
{
	lua_pushstring(L, ReadFileG(luaL_checkstring(L, 1)).c_str());
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(DeleteFile)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(DeleteFile)
{
	lua_pushboolean(L, DeleteFileG(luaL_checkstring(L, 1)));
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(DeleteDirectory)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(DeleteDirectory)
{
	lua_pushboolean(L, DeleteDirectoryG(luaL_checkstring(L, 1)));
	return 1;
}

#pragma endregion

#pragma region Hooks

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(SetHookHandler)
///
/// \brief	Sets the function that will be called for each of Garry's game hooks.  Returning
/// 		in this function will cause that hook to not run.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(SetHookHandler)
{
	luaL_checktype(L, 1, LUA_TFUNCTION);
	Class::HookManager->SetCallFunction(luaL_ref(L, GMOD_REGINDEX));

	return 0;
}

#pragma endregion

#pragma region Lua

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(ProxyCall)
///
/// \brief	Calls a Lua function in from C.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(ProxyCall)
{
	luaL_checktype(L, 1, LUA_TFUNCTION);

	int start = lua_gettop(L);
	int args = start - 1;

	lua_call(L, args, LUA_MULTRET);

	int results = (lua_gettop(L) - start) + args + 1;

	return results;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(Lookup)
///
/// \brief	Returns a global using C rawget.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(Lookup)
{
	luaL_checktype(L, 1, LUA_TSTRING);
	const char* name = lua_tostring(L, 1);

	lua_rawgeti(L, GMOD_REGINDEX, LUA_RIDX_GLOBALS);

	if (lua_type(L, -1) == LUA_TTABLE)
	{
		lua_getfield(L, -1, name);
		return 1;
	}
	else
	{
		lua_pushboolean(L, false);
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(RunVirtual)
///
/// \brief	Runs a virtual Lua file.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(RunVirtual)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	string path = string(lua_tostring(L, 1));
	string name = Mem::GetHash(path);
	
	Lib::IO::VirtualFile* v = Class::VirtualC->FindByName(name);

	if (v)
	{
		// Run the code using the ScriptManager
		bool success = Class::ScriptManager->RunCode(v->Data, "/sym/" + v->Name, v->Encrypted);
		
		// Return the results
		lua_pushboolean(L, success);
		return 1;
	}

	lua_pushboolean(L, false);
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(RunCode)
///
/// \brief	Runs Lua code.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(RunCode)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	const char* code = lua_tostring(L, 1);

	lua_pushboolean(L, Class::ScriptManager->RunCode(code, "/sym/run_code/", false));
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(ProcessQueue)
///
/// \brief	Process the Lua file queue.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(ProcessQueue)
{
	if (Class::ScriptManager)
		Class::ScriptManager->ProcessQueue();

	return 0;
}

#pragma endregion

#pragma region Console

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(Print)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(Print)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	ConsoleIO::Write(LEVEL_NORMAL, false, (char*)lua_tostring(L, 1));

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(PrintLineF)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(PrintLineF)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	PrintLine((char*)lua_tostring(L, 1));

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(PrintBadF)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(PrintBadF)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	PrintBadL((char*)lua_tostring(L, 1));

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(PrintGoodF)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(PrintGoodF)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	PrintGoodL((char*)lua_tostring(L, 1));

	return 0;
}

#ifdef BUILDMODE_BETA
////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(PrintDebugF)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(PrintDebugF)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	PrintDebugL((char*)lua_tostring(L, 1));

	return 0;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(PrintWarningF)
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(PrintWarningF)
{
	luaL_checktype(L, 1, LUA_TSTRING);

	PrintWarningL((char*)lua_tostring(L, 1));

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(RegisterConsoleCommand)
///
/// \brief Registers a Lua console command.  After registration, the command can be executed
/// 	   from the Symmetry console.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(RegisterConsoleCommand)
{
	luaL_checktype(L, 1, LUA_TSTRING);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TSTRING);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_checktype(L, 5, LUA_TFUNCTION);

	int ref				= luaL_ref(L, GMOD_REGINDEX);

	const char*	name	= lua_tostring(L, 1);
	const char*	help	= lua_tostring(L, 2);
	const char* usage	= lua_tostring(L, 3);
	int minargs = lua_tointeger(L, 4);

	Class::Console->RegisterEntry(new LuaConCommand(name, help, usage, minargs, ref));
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(RemoveConsoleEntry)
///
/// \brief	Removes a console entry.  A console entry can be a ConVar or a Console Command.  Either
/// 		way this function will remove it by name.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(RemoveConsoleEntry)
{
	const char* name = luaL_checkstring(L, 1);

	// Check if the entry exists, if it does
	// remove it and push true, otherwise push false
	if (Class::Console->FindEntry(name))
	{
		Class::Console->RemoveEntry(name);
		lua_pushboolean(L, true);
	}
	else 
		lua_pushboolean(L, false);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(RunConsoleCommand)
///
/// \brief	Runs a string in the Symmetry console.  This can be to activate a console command or
/// 		set a ConVar.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(RunConsoleCommand)
{
	for (CommandArgs args : Class::Console->ParseCommand(luaL_checkstring(L, 1)))
	{
		Class::Console->PerformCommand(args);
	}
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(GetConVarString)
///
/// \brief	Pushes a ConVar onto the stack
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(GetConVar)
{
	const char* name = luaL_checkstring(L, 1);

	// Search for an entry by name
	IConsoleEntry* entry = Class::Console->FindEntry(name);

	// Check if the entry is a ConVar and that it exists, if it isn't/doesn't
	// we can push false instead of the convar
	if (entry && dynamic_cast<SymConVarBase*>(entry) != NULL)
	{
		lua_pushlightuserdata(L, entry);
	}
	else
		lua_pushboolean(L, false);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(GetConVarNumber)
///
/// \brief	Returns the value of a ConVar.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(GetConVarValue)
{
	IConsoleEntry* entry = (IConsoleEntry*)lua_touserdata(L, 1);
	
	if (entry)
	{
		SymConVarBase* base = dynamic_cast<SymConVarBase*>(entry);
		if (base)
		{
			lua_pushstring(L, base->GetValueEx().c_str());
			return 1;
		}
	}

	lua_pushboolean(L, false);
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(SetConVar)
///
/// \brief  Sets the value of a ConVar.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(SetConVar)
{
	const char* name	= luaL_checkstring(L, 1);
	const char* value	= luaL_checkstring(L, 2);

	// Search for an entry by name
	IConsoleEntry* entry = Class::Console->FindEntry(name);

	// Check if the entry is a ConVar and that it exists, if it isn't/doesn't
	// we can push false instead of the value.
	if (entry && dynamic_cast<SymConVarBase*>(entry) != NULL)
	{
		SymConVarBase* cvbase = (SymConVarBase*)entry;

		// Set the value of the ConVar
		cvbase->SetValueEx(value);

		// Return true
		lua_pushboolean(L, true);
	}
	else
		lua_pushboolean(L, false);

	return 1;
}
#pragma endregion

#pragma region Source

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LFUNC(Test)
///
/// \brief	Test function.
////////////////////////////////////////////////////////////////////////////////////////////////////

LFUNC(Test)
{
	if (lua_type(L, 1) == LUA_TFUNCTION)
	{
		PrintDebugL("Address = %X", lua_tocfunction(L, 1));
	}
	else
	{
		PrintDebugL("Address = %X", lua_topointer(L, 1));
	}

	return 0;
}

#pragma endregion


NAMESPACE_CORE(Lua)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void Initialize(lua_State*)
///
/// \brief	Initializes the Lua library by leaving the lua library on the stack.  It must
/// 		be popped off the stack later!
///
/// \param [in,out]	lsState	Pointer to the lua_State to push the library onto.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Library::Initialize(lua_State* L)
{
	if (Library::m_iSymTable)
	{
		lua_rawgeti(L, GMOD_REGINDEX, Library::m_iSymTable);
		lua_pushvalue(L, -1);

		Library::m_iSymTable = luaL_ref(L, GMOD_REGINDEX);
		return;
	}
	else
	{
		lua_newtable(L);
	}
	
	// == IO == //
#pragma region IO
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setfield(L, -3, "IO");

	LUA_BIND("FileExists", FileExists);
	LUA_BIND("DirectoryExists", DirectoryExists);
	LUA_BIND("CreateFile", CreateFile);
	LUA_BIND("CreateDirectory", CreateDirectory);
	LUA_BIND("WriteFile", WriteFile);
	LUA_BIND("ReadFile", ReadFile);
	LUA_BIND("DeleteFile", DeleteFile);
	LUA_BIND("DeleteDirectory", DeleteDirectory);

#ifdef BUILDMODE_BETA
	LUA_BIND("PrintDebug", PrintDebugF);
#endif

	lua_pop(L, 1);

#pragma endregion
	// == Hook stuff == //
#pragma region Hook
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setfield(L, -3, "Hook");

	LUA_BIND("SetHookHandler", SetHookHandler);

	lua_pop(L, 1);
#pragma endregion
	// == Lua stuff == //
#pragma region Lua
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setfield(L, -3, "Lua");

	LUA_BIND("Call", ProxyCall);
	LUA_BIND("Lookup", Lookup);
	LUA_BIND("RunVirtual", RunVirtual);
	LUA_BIND("RunCode", RunCode);
	LUA_BIND("ProcessQueue", ProcessQueue);

	lua_pop(L, 1);
#pragma endregion
	// == Console stuff == //
#pragma region Console
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setfield(L, -3, "Console");

	LUA_BIND("Print", Print);
	LUA_BIND("PrintLine", PrintLineF);
	LUA_BIND("PrintBad", PrintBadF);
	LUA_BIND("PrintGood", PrintGoodF);
	LUA_BIND("PrintWarning", PrintWarningF);

	LUA_BIND("RegisterConsoleCommand", RegisterConsoleCommand);
	LUA_BIND("RemoveConsoleEntry", RemoveConsoleEntry);
	LUA_BIND("RunConsoleCommand", RunConsoleCommand);
	LUA_BIND("GetConVar", GetConVar);
	LUA_BIND("GetConVarValue", GetConVarValue);
	LUA_BIND("SetConVar", SetConVar);

	lua_pop(L, 1);
#pragma endregion
	// == Source stuff == //
#pragma region Source
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setfield(L, -3, "Src");

	LUA_BIND("Test", Test);

	lua_pop(L, 1);
#pragma endregion
	// == == //

	// Keep a reference to our table
	lua_pushvalue(L, -1);
	Library::m_iSymTable = luaL_ref(L, GMOD_REGINDEX);
}

NAMESPACE_END2