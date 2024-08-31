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
#include "symconvar.h"

NAMESPACE_CORE(Console)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	ConsoleManager
///
/// \brief	This instance will manage everything to do with the console. It will handle
/// 		input/output, console variables, console commands &amp; parsing/execution of each of
/// 		those.  It owns the thread that console commands / convar callbacks are ran in.
////////////////////////////////////////////////////////////////////////////////////////////////////

class ConsoleManager
{
private:

	// Each convar/concommand will be stored in here
	vector< IConsoleEntry* >	m_vEntries;

	// Aliases (x = y)
	vector<pair<string, string>>m_vAliases;

	// Key bindings 
	string						m_sKeyBindings[MOUSE_LAST + 1];


private:

	string			CfgSafeStr(string);

public:

	ConsoleManager();
	~ConsoleManager();

	// Entry manipulation
	void			RemoveEntry(string);
	void			RegisterEntry(IConsoleEntry*);
	IConsoleEntry*	FindEntry(string);

	// Command related functions
	vector<CommandArgs>	ParseCommand(string);
	void				PerformCommand(CommandArgs, bool bSilent = false, bool bSave = true);

	// Alias stuff
	bool			AddAlias(string, string);
	bool			RemoveAlias(string);
	string			ProcessAliases(string);
	vector<pair<string, string>>
					GetAliases();

	// Bind stuff
	bool			AddBind(ButtonCode_t code, string);
	bool			RemoveBind(ButtonCode_t code);
	bool			RunBind(ButtonCode_t code, bool bReleased);
	vector<pair<ButtonCode_t, string>>
					GetBindings();

	// Other
	void			SaveConfig(string);
	void			LoadConfig(string);

	void			Exec(string);

	int				GetConvarCount();
	int				GetConComCount();
	int				GetBindCount();

	vector< IConsoleEntry* >
					GetConsoleEntries();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn template <typename T> virtual void CConsole::Write(T)
	///
	/// \brief Writes a type T into the console.
	///
	/// \tparam typename Write type.
	/// \param tValue The value to write.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	void Write(T tValue)
	{
		cout << tValue;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn template <typename T> virtual T CConsole::Read()
	///
	/// \brief Reads a type T from the console.
	///
	/// \tparam typename Read type.
	///
	/// \return .
	////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	T Read()
	{
		T tVal; cin >> tVal;
		return tVal;
	}

	// Static function for accessing the global instance of this class
	void static RegistryEntryProxy(IConsoleEntry* pEntry);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	AliasProxy
///
/// \brief	Defined in source.  Constructs an alias that can be made inside source files.  Alias
/// 		will be added on CRT initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////

class AliasProxy
{
public:
	AliasProxy(string, string);
};

// Macro for defining CRT initialized aliases
#define MakeAlias(a, b) AliasProxy __##a__##b_(a, b)

// Mapping for keys
extern const char* g_pszButtonNames[];

NAMESPACE_END2