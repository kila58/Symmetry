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

#include "console_entry.h"
#include "command_args.h"
#include "console_manager.h"

#include "console.h"

#include "lua_gmod.h"

NAMESPACE_CORE(Console)

// Callback for console commands
typedef void ( __cdecl * ConCommandFunc )( CommandArgs );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SymConCommandBase
///
/// \brief	Console command base inherited by Lua and C++ console commands.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SymConCommandBase : public IConsoleEntry
{
protected:

	string	m_sUsage;
	int		m_iMinArgs;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	string SymConCommandBase::GetUsage()
	///
	/// \brief	Gets a string describing the usage of this console command.
	///
	/// \return	The usage.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	string GetUsage()
	{
		return this->m_sUsage;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	int SymConCommandBase::GetMinArgs()
	///
	/// \brief	Gets minimum arguments.
	///
	/// \return	The minimum arguments.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int GetMinArgs()
	{
		return this->m_iMinArgs;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class ConCommand
///
/// \brief Console command.  Essentially a wrapper for a console command callback (see above)
/// 	   that stores extra information like the name & help string.  This also bases
/// 	   off IConsoleEntry so that it can registered with the console manager.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SymConCommand : public SymConCommandBase
{
private:

	ConCommandFunc m_ccCallback;
	
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SymConCommand::SymConCommand(string sName, ConCommandFunc ccFunc)
	///
	/// \brief	Constructor.
	///
	/// \param	sName 	The name of the console command.
	/// \param	ccFunc	The callback for this console command.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SymConCommand(string sName, ConCommandFunc ccFunc)
	{
		this->m_sName		= sName;
		this->m_sHelpString = "";

		this->m_ccCallback = ccFunc;

		ConsoleManager::RegistryEntryProxy(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SymConCommand::SymConCommand(string sName, string sHelp, ConCommandFunc ccFunc)
	///
	/// \brief	Constructor.
	///
	/// \param	sName 	The name of the console command.
	/// \param	sHelp 	The help string for this command.
	/// \param	ccFunc	The callback for this console command.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SymConCommand(string sName, string sHelp, ConCommandFunc ccFunc)
	{
		this->m_sName		= sName;
		this->m_sHelpString = sHelp;

		this->m_ccCallback = ccFunc;

		ConsoleManager::RegistryEntryProxy(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	SymConCommand::SymConCommand(string sName, string sHelp, string sUsage, int iMinArgs,
	/// 	ConCommandFunc ccFunc)
	///
	/// \brief	Constructor.
	///
	/// \param	sName   	The name of the console command.
	/// \param	sHelp   	The help string for this command.
	/// \param	sUsage  	A string that describes the usage of this command.
	/// \param	iMinArgs	Minimum arguments required to run this  command.
	/// \param	ccFunc  	The callback for this console command.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SymConCommand(string sName, string sHelp, string sUsage, int iMinArgs, ConCommandFunc ccFunc)
	{
		this->m_sName		= sName;
		this->m_sHelpString = sHelp;

		this->m_ccCallback	= ccFunc;

		this->m_sUsage		= sUsage;
		this->m_iMinArgs	= iMinArgs;

		ConsoleManager::RegistryEntryProxy(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn void ConCommand::Execute(CommandArgs caArgs)
	///
	/// \brief Executes this console command parsing caArgs as arguments.
	///
	/// \param caArgs The arguments to pass to this console command.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Execute(CommandArgs caArgs, bool bPrint = true)
	{
		if (caArgs.GetArgCount() < this->m_iMinArgs)
		{
			if (bPrint)
			{
				PrintLine("Usage : %s", this->m_sUsage.c_str());
			}
			return;
		}

		if (this->m_ccCallback != NULL)
			this->m_ccCallback(caArgs);
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	LuaConCommand
///
/// \brief	A console command designed to be defined in Lua and executed in Lua.
////////////////////////////////////////////////////////////////////////////////////////////////////

class LuaConCommand : public SymConCommandBase
{
private:
	int m_iLuaCallbackRef;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	LuaConCommand::LuaConCommand(string sName, int iFuncRef)
	///
	/// \brief	Constructor.
	///
	/// \param	sName   	The name of the console command.
	/// \param	iFuncRef	A reference to the Lua callback for this concommand.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	LuaConCommand(string sName, int iFuncRef)
	{
		this->m_sName = sName;
		this->m_sHelpString = "";

		this->m_iLuaCallbackRef = iFuncRef;

		ConsoleManager::RegistryEntryProxy(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	LuaConCommand::LuaConCommand(string sName, string sHelp, int iFuncRef)
	///
	/// \brief	Constructor.
	///
	/// \param	sName   	The name of the console command.
	/// \param	sHelp   	The help string for this command.
	/// \param	iFuncRef	A reference to the Lua callback for this concommand.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	LuaConCommand(string sName, string sHelp, int iFuncRef)
	{
		this->m_sName = sName;
		this->m_sHelpString = sHelp;

		this->m_iLuaCallbackRef = iFuncRef;

		ConsoleManager::RegistryEntryProxy(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	LuaConCommand::LuaConCommand(string sName, string sHelp, string sUsage, int iMinArgs,
	/// 	int iFuncRef)
	///
	/// \brief	Constructor.
	///
	/// \param	sName   	The name of the console command.
	/// \param	sHelp   	The help string for this command.
	/// \param	sUsage  	A string that describes the usage of this command.
	/// \param	iMinArgs	Minimum arguments required to run this  command.
	/// \param	iFuncRef	A reference to the Lua callback for this concommand.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	LuaConCommand(string sName, string sHelp, string sUsage, int iMinArgs, int iFuncRef)
	{
		this->m_sName = sName;
		this->m_sHelpString = sHelp;

		this->m_iLuaCallbackRef = iFuncRef;

		this->m_sUsage = sUsage;
		this->m_iMinArgs = iMinArgs;

		ConsoleManager::RegistryEntryProxy(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn void ConCommand::Execute(CommandArgs caArgs)
	///
	/// \brief Executes this console command parsing caArgs as arguments.
	///
	/// \param caArgs The arguments to pass to this console command.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Execute(CommandArgs caArgs, lua_State* L, bool bPrint = true)
	{
		if (caArgs.GetArgCount() < this->m_iMinArgs)
		{
			if (bPrint)
			{
				PrintLine("Usage : %s\n", this->m_sUsage.c_str());
			}
			return;
		}

		if (this->m_iLuaCallbackRef)
		{
			lua_rawgeti(L, GMOD_REGINDEX, this->m_iLuaCallbackRef);

			// Push duplicate and re-register
			lua_pushvalue(L, -1);
			luaL_ref(L, GMOD_REGINDEX);

			// Push arguments
			lua_newtable(L);

			// Build argument table
			for (int i = 0; i < caArgs.GetArgCount(); i++)
			{
				lua_pushinteger(L, i + 1);
				lua_pushstring(L, caArgs.GetArgAt(i).c_str());
				lua_settable(L, -3);
			}

			// Push other things
			lua_pushstring(L, caArgs.GetArgString().c_str());
			lua_pushstring(L, caArgs.GetCommandString().c_str());

			// The console handler must call the function, we can't use the Global
			// namespace from inside here
		}
	}
};

#define MakeConCommand(name)\
	SymConCommand __##name("" #name "", name)

#define MakeConCommand2(name, help)\
	SymConCommand __##name("" #name "", help, name)

#define MakeConCommand4(name, help, usage, minargs)\
	SymConCommand __##name("" #name "", help, usage, minargs, name)

NAMESPACE_END2