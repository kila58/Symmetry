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
#include "lua_interface_manager.h"

NAMESPACE_CORE(Lua)

////////////////////////////////////////////////////////////////////////////////////////////////////
///										   == STRUCTS ==
////////////////////////////////////////////////////////////////////////////////////////////////////

struct LuaEncryptedFile
{
	string	sData;
	string	sName;

	bool	bIsPlain;
};

struct AESKeySet
{
	uchar*	cKey;
	uchar*	cIV;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
///										    == CLASS ==
////////////////////////////////////////////////////////////////////////////////////////////////////

class LuaManager
{
private:

	// Lua file containers
	vector<LuaEncryptedFile*>	m_vAutorunFiles;

	// We can't run Lua from a seperate thread.  So we'll queue files
	// that are ran in non-main threads.
	vector<LuaEncryptedFile*>	m_vCodeQueue;

	// Used in (Set/Get)HasRun - denotes whether or not the lua files
	// here have been run, set to false in CLuaShared::CloseLuaInterface
	// set to true in ILuaInterface::RunString
	bool						m_bHasRun;

	// Intergral representation of the state used.  CLIENT or SERVER for example.
	// (both enums defined in lua_interface_manager.h)
	int							m_iState;

public:

	LuaManager(int);
	~LuaManager();

	void	RunScripts();

	void	SetHasRun(bool);
	bool	GetHasRun();

	void	AddAutorunFile(string, string, bool encrypted = true);
	void	AddFile(string, string, bool encrypted = false);

	void	ProcessQueue();

	bool	RunCode(string, string, bool);
	bool	SafeCall(lua_State*, string, int, int);

	void	HandleError(string, string);
};

NAMESPACE_END2