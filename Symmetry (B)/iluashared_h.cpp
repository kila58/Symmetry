/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "iluashared_h.h"

#include "global.h"

#include "symconvar.h"

#include "lua_library.h"

#include "lua_h.h"

#include "src_lib.h"

#include "filea.h"

#include "patterns.h"

using namespace Core;
using namespace Core::Patterns;
using namespace Core::Hooks;

SymConVar<short> lua_print_run("lua_print_run", "Prints the name of each Lua file that's run to the console.  Can cause a lot of spam", 0);
SymConVar<short> lua_dump_("lua_dump", "1 = Dumps regular Lua files to /data/lua/ip/regular/, 2 = dumps files skipping those in GMod, 0 = disabled", 1);
SymConVar<short> lua_dump_dupe("lua_dump_dupe", "Saves all variations of non-dynamic files, rather than overwriting them.  Can cause a lot of mess", 0);
SymConVar<short> lua_dump_dynamic("lua_dump_dynamic", "Dumps dynamically ran Lua files (RunString, CompileString, LuaCmd, ...) to /data/lua/ip/dynamic/", 0);
SymConVar<short> lua_disable("lua_disable", "Disable the use of Lua scripts, for servers that may detect them", 0);

NAMESPACE_CORE(Hooks)

// JMP
LoadBufferX_t CLS_LoadBufferX;

// VMT
Detours::VMT* g_LuaShared;

// Typedefs
typedef PDWORD* (__thiscall* CreateInterface_t)(void* vmt, uchar cIndex, bool);
typedef void(__thiscall* CloseInterface_t)(void* vmt, PDWORD* pdwpLuaInterface);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	PDWORD_PTR* CreateInterface(uchar, bool)
///
/// \brief	Hooks CLuaShared::CreateInterface.
///
/// \param	iIndex		Unique ID for the interface. 
/// \param	bSomething	Unknown
///
/// \return	A newly created ILuaInterface.
////////////////////////////////////////////////////////////////////////////////////////////////////

PDWORD*	__fastcall CreateInterface(void* pECX, void* _, uchar iIndex, bool bSomething)
{
	PrintGoodL("Interface %d created (b = %s)", (int)iIndex, bSomething ? "true" : "false");

	// Pointer to the new interace created by the original function
	dword* interface = (dword*)
		((CreateInterface_t)(g_LuaShared->GetMethod(LUASHARED_CREATEINTERFACE)))(pECX, iIndex, bSomething);

	// Add the interface to the state manager
	Class::StateManager->AddInterface((void*)interface, (InterfaceState)iIndex);

	return (PDWORD*)interface;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CloseInterface(PDWORD_PTR*)
///
/// \brief	Hooks CLuaShared::CloseLuaInterface
///
/// \param pdwpLuaInterface		The interface that's about to close.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall CloseInterface(void* pECX, void* _, PDWORD* pdwpLuaInterface)
{
	// Store VMT
	void* vmt; GetReg(ECX, vmt)

	// We need to tell things (like the Lua library) that references
	// no longer are valid.
	Library::m_iSymTable = 0;

	// Technically we could keep this reference, since as it's set on load,
	// but it's safer this way I suppose
	Class::HookManager->SetCallFunction(0);

	// Unhook stuff
	LuaH::UnHook();
	
	// Make sure next time we a interface is created we'll run scripts
	Class::ScriptManager->SetHasRun(false);

	// We're done with the state
	Class::StateManager->RemoveInterface(pdwpLuaInterface);
	
	return ((CloseInterface_t)(g_LuaShared->GetMethod(LUASHARED_CLOSEINTERFACE)))(pECX, pdwpLuaInterface);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LoadBufferX()
///
/// \brief	Loads buffer x coordinate.
////////////////////////////////////////////////////////////////////////////////////////////////////

int LoadBufferX(lua_State* L, char_c* cBuf, size_t iSize, char_c* cName, char_c* cMode)
{
	// Don't dump anything but clientside files 
	if (L != Class::StateManager->GetState(CLIENT))
		return CLS_LoadBufferX(L, cBuf, iSize, cName, cMode);

	static int fileCount = 0;

	string name = cName;
	name = name.substr(1);

	// NULL with server files, called in listen servers
	INetChannelInfo* info = SrcLib::GetNetChannelInfo();

	// Dump files (can be expensive to check if a file is dynamic or not)
	if (info && (lua_dump_.GetValue() || lua_dump_dynamic.GetValue()))
	{
		string data = string(cBuf, iSize);
		string dumpPath = "data/lua/" + string(info->GetAddress()) + "/";

		bool dynamic = true;

		// Shit way to check?
		for (char c : name)
		{
			if (c == '\\' || c == '/' || c == ':')
				dynamic = false;

			// Check for characters that can only occur in dynamic files
			if (IsBadDirChar(c))
				dynamic = true;
		}

		// Dump as dynamic file
		if (dynamic && lua_dump_dynamic.GetValue())
		{
			CreateDirectoryG(dumpPath + "dynamic/");
			string path = dumpPath + "dynamic/" + name;

			if (data.length() > 255 || data.find("\n") != string::npos)
			{
				// We're going to put this file in it's own directory
				CreateDirectoryG(path);

				// Find a name that's not used
				int idx = 0;
				while (FileExistsG(path + "/entry_" + to_string(idx) + ".lua"))
					idx++;

				WriteFileG(path + "/entry_" + to_string(idx) + ".lua", data);
			}
			else
			{
				time_t t = time(NULL);
				char timestr[0xFF];

				strftime(timestr, sizeof(timestr), "%H:%M:%S", localtime(&t));

				// Format new line
				string line = "[" + string(timestr) + "] " + data + "\n";

				// Append file, nothing else we need to do
				AppendFileG(path + ".lua", line);
			}
		}

		// Dump as regular file
		if (!dynamic && lua_dump_.GetValue())
		{
			if (lua_dump_.GetValue() == 1 || (!Lib::IO::FileExists("garrysmod/" + name) ||
				Mem::GetMD5(Lib::IO::ReadFile("garrysmod/" + name)) != Mem::GetMD5(data)))
			{
				// Get path and dir
				string path = dumpPath + "regular/" + name;
				string dir = path;

				for (uint32 i = path.length(); true; i--)
				{
					if (path[i] == '\\' || path[i] == '/')
						break;

					dir = path.substr(0, i);
				}

				// Create path to file
				CreateDirectoryG(dir);

				if (FileExistsG(path))
				{
					if (lua_dump_dupe.GetValue())
					{
						if (!FileExistsG(dir + Mem::GetMD5(data) + ".lua") &&
							Mem::GetMD5(data) != Mem::GetMD5(ReadFileG(path)))
						{
							WriteFileG(dir + Mem::GetMD5(data) + ".lua", data);
						}
					}
					else
					{
						WriteFileG(path, data);
					}
				}
				else
					WriteFileG(path, data);
			}
		}
	}

	// Run Lua scripts.  If the CLIENT state does not exist yet, don't bother doing anything.
	if (!Class::ScriptManager->GetHasRun() && Class::StateManager->GetInterface(CLIENT) != NULL && !lua_disable.GetValue())
	{
		Class::ScriptManager->RunScripts();
		Class::ScriptManager->SetHasRun(true);

		// We need to hook Lua functions after running our scripts
		LuaH::Hook(Class::StateManager->GetState(CLIENT));

		// Reset filecount to 1, this would be the first file run on this server
		fileCount = 1;
	}
	else
		fileCount++;

	// Print filename
	if (lua_print_run.GetValue())
	{
		string sname = name;
		Lib::IO::MakeDirSafe(sname);

		// Print the safe name
		PrintGoodL("%d) Running %s", fileCount, sname.c_str());
	}

	return CLS_LoadBufferX(L, cBuf, iSize, cName, cMode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn static void LuaSharedH::Hook();
///
/// \brief Hooks all functions in this file.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaSharedH::Hook()
{
	g_LuaShared = new Detours::VMT((PDWORD*)Src::LuaShared);
	g_LuaShared->Hook((dword)CreateInterface, LUASHARED_CREATEINTERFACE);
	g_LuaShared->Hook((dword)CloseInterface, LUASHARED_CLOSEINTERFACE);

	MH_CreateHook(Patterns::LuaLoadBuffer, &LoadBufferX,
		reinterpret_cast<void**>(&CLS_LoadBufferX));
	MH_EnableHook(Patterns::LuaLoadBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaSharedH::UnHook()
///
/// \brief	Unhooks all functions in this file.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaSharedH::UnHook()
{
	DeleteAndNull(g_LuaShared);

	MH_DisableHook(Patterns::LuaLoadBuffer);
	MH_RemoveHook(Patterns::LuaLoadBuffer);
}

NAMESPACE_END2