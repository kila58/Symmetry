/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "lua_manager.h"
#include "lua_library.h"

#include "patterns.h"

#include "global.h"

#include "lua_gmod.h"

#include "symconvar.h"

SymConVar<short> lua_print_errors("lua_print_errors", "Print Lua errors.  Most Lua errors are redirected to the Symmetry console, if this is not enabled they won't be printed anywhere.", 1);

NAMESPACE_CORE_B

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LuaManager::LuaManager(InterfaceState isState)
///
/// \brief	Default constructor.
///
/// \param	isState .
////////////////////////////////////////////////////////////////////////////////////////////////////

LuaManager::LuaManager(int iState)
{
	this->m_bHasRun		= false;
	this->m_iState		= iState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LuaManager::~LuaManager()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

LuaManager::~LuaManager()
{
	// Added with below functions, these need to be deleted
	for (LuaEncryptedFile* f : this->m_vAutorunFiles)
	{
		delete f;
	}

	for (LuaEncryptedFile* f : this->m_vCodeQueue)
	{
		delete f;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaManager::RunScripts()
///
/// \brief	Executes each script in m_AutorunFiles
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaManager::RunScripts()
{
	for (uint32 i = 0; i < this->m_vAutorunFiles.size(); i++)
	{
		LuaEncryptedFile* file = this->m_vAutorunFiles[i];

		if (!this->RunCode(file->sData, file->sName, !file->bIsPlain))
			continue;

#ifdef	BUILDMODE_BETA
		PrintGoodL("Ran Lua file %s", file->sName.c_str());
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaManager::ProcessQueue()
///
/// \brief	Processes queued Lua files.  THIS SHOULD BE CALLED IN THE MAIN GAME THREAD.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaManager::ProcessQueue()
{
	for (uint32 i = 0; i < this->m_vCodeQueue.size(); i++)
	{
		LuaEncryptedFile* file = this->m_vCodeQueue[i];

		if (!this->RunCode(file->sData, file->sName, !file->bIsPlain))
			continue;
	}

	this->m_vCodeQueue.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaManager::SetHasRun(bool bHasRun)
///
/// \brief	Sets m_bHasRun.   
///
/// \param	bHasRun	.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaManager::SetHasRun(bool bHasRun)
{
	this->m_bHasRun = bHasRun;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool LuaManager::GetHasRun()
///
/// \brief	Returns m_bHasRun.
///
/// \return .
////////////////////////////////////////////////////////////////////////////////////////////////////

bool LuaManager::GetHasRun()
{
	return this->m_bHasRun;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaManager::AddAutorunFile(string sContents, string sName, bool bIsEncrypted)
///
/// \brief	Adds a file to the autorun queue.
///
/// \param	sContents   	The contents of the file.
/// \param	sName			The name of the file.
/// \param	bIsEncrypted	True if the file is encryted with AES.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaManager::AddAutorunFile(string sContents, string sName, bool bIsEncrypted)
{
	LuaEncryptedFile* file = new LuaEncryptedFile;

	file->sData = sContents;
	file->sName = sName;
	file->bIsPlain = !bIsEncrypted;

	this->m_vAutorunFiles.push_back(file);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaManager::AddFile(string sContents, string sName, bool bIsEncrypted)
///
/// \brief	Adds file to the code queue, so that it's ran on the main thread.
///
/// \param	sContents   	The contents of the file.
/// \param	sName			The name of the file.
/// \param	bIsEncrypted	True if the file is encryted with AES.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaManager::AddFile(string sContents, string sName, bool bIsEncrypted)
{
	LuaEncryptedFile* file = new LuaEncryptedFile;

	file->sData = sContents;
	file->sName = sName;
	file->bIsPlain = !bIsEncrypted;

	this->m_vCodeQueue.push_back(file);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool LuaManager::RunCode(string sLua, string sName, bool bIsEncrypted)
///
/// \brief	Executes Lua code.  The environment name is the second argument.
///
/// \param	sLua			The Lua code to run.
/// \param	sName			The environment name.
/// \param	bIsEncrypted	True if the code is encrypted with AES.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool LuaManager::RunCode(string sLua, string sName, bool bIsEncrypted)
{
	lua_State* L = Class::StateManager->GetState((InterfaceState)this->m_iState);

	// Make sure we actually have a valid state
	if (L == NULL)
	{
		PrintBadL("Tried running %s without state %d!", sName.c_str(), this->m_iState);
		return false;
	}

	// Decrypt the code if we need to
	string plain = "";

	if (bIsEncrypted)
	{
		plain = Mem::AESDecrypt(sLua, Class::VirtualC->GetKeySet().Key, 
			Class::VirtualC->GetKeySet().IV);
	}
	else
	{
		plain = sLua;
	}

	// Push the Lua as a function onto the stack
	int compile = luaL_loadbuffer(L, plain.c_str(), plain.size(), ("@" + sName).c_str());

	// Check if the script compiled successfully
	if (compile != 0)
	{
		this->HandleError(lua_tostring(L, -1), sName);
		lua_pop(L, 1);

		return false;
	}

	// Then push the Lua library onto the stack
	Library::Initialize(L);

	// Clean-up so the code isn't left in memory
	plain.erase();

	// Call the script in safemode
	bool b = this->SafeCall(L, sName, 1, 0);

	return b;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool LuaManager::SafeCall(lua_State* L, string sName, int iArgs, int iReturn)
///
/// \brief	Calls a function in safemode.  sName is the path or errorpath that will be
/// 		printed if the call fails.
///
/// \param [in,out]	L	Lue state.
/// \param	sName	 	The scriptpath.
/// \param	iArgs	 	Number of arguments.
/// \param	iReturn  	Number of returns.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool LuaManager::SafeCall(lua_State* L, string sName, int iArgs, int iReturn)
{
	// Call the function passing the library in as the first argument
	int res = lua_pcall(L, iArgs, iReturn, NULL);

	if (res == 0)
	{
		return true;
	}
	else
	{
		// Non-zero returns are errors, the error string is pushed onto the Lua stack
		this->HandleError(lua_tostring(L, -1), sName);
		lua_pop(L, 1);

		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void LuaManager::HandleError(string sError, string sName)
///
/// \brief	Handles a Lua error.
///
/// \param	sError	The error.
/// \param	sName 	The name of the script the caused the error.
////////////////////////////////////////////////////////////////////////////////////////////////////

void LuaManager::HandleError(string sError, string sName)
{
	if (lua_print_errors.GetValue())
	{
		PrintBadL("LUA_ERR: %s", sError.c_str());
	}
}

NAMESPACE_END1