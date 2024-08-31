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

#include "console_manager.h"
#include "concommand.h"
#include "symconvar.h"

#include "global.h"

#include "filea.h"

NAMESPACE_CORE(Console)

MakeConVar4(short, sym_save, "enable saving of binds, console variables and aliases.  0 to disable", 1);
MakeConVar4(short, sym_bind_menu, "enable running binds whilst NOT in-game.  0 to disable", 0);
MakeConVar4(short, sym_bind_silent, "enable silent execution on binds, silent execution will stop most output of console commands in the console.  0 to disable", 1);
MakeConVar4(short, sym_bind_divert, "enable bind diversion, when binds are diverted key-strokes will be overridden and whatever those keys would do normally would not happen.  0 to disable", 0);

// from source.  Aligned with ButtonCode_t
const char* g_pszButtonNames[] =
{
	"INVALID",		
	"0",			// KEY_0,
	"1",			// KEY_1,
	"2",			// KEY_2,
	"3",			// KEY_3,
	"4",			// KEY_4,
	"5",			// KEY_5,
	"6",			// KEY_6,
	"7",			// KEY_7,
	"8",			// KEY_8,
	"9",			// KEY_9,
	"A",			// KEY_A,
	"B",			// KEY_B,
	"C",			// KEY_C,
	"D",			// KEY_D,
	"E",			// KEY_E,
	"F",			// KEY_F,
	"G",			// KEY_G,
	"H",			// KEY_H,
	"I",			// KEY_I,
	"J",			// KEY_J,
	"K",			// KEY_K,
	"L",			// KEY_L,
	"M",			// KEY_M,
	"N",			// KEY_N,
	"O",			// KEY_O,
	"P",			// KEY_P,
	"Q",			// KEY_Q,
	"R",			// KEY_R,
	"S",			// KEY_S,
	"T",			// KEY_T,
	"U",			// KEY_U,
	"V",			// KEY_V,
	"W",			// KEY_W,
	"X",			// KEY_X,
	"Y",			// KEY_Y,
	"Z",			// KEY_Z,
	"KP_INS",		// KEY_PAD_0,
	"KP_END",		// KEY_PAD_1,
	"KP_DOWNARROW",	// KEY_PAD_2,
	"KP_PGDN",		// KEY_PAD_3,
	"KP_LEFTARROW",	// KEY_PAD_4,
	"KP_5",			// KEY_PAD_5,
	"KP_RIGHTARROW",// KEY_PAD_6,
	"KP_HOME",		// KEY_PAD_7,
	"KP_UPARROW",	// KEY_PAD_8,
	"KP_PGUP",		// KEY_PAD_9,
	"KP_SLASH",		// KEY_PAD_DIVIDE,
	"KP_MULTIPLY",	// KEY_PAD_MULTIPLY,
	"KP_MINUS",		// KEY_PAD_MINUS,
	"KP_PLUS",		// KEY_PAD_PLUS,
	"KP_ENTER",		// KEY_PAD_ENTER,
	"KP_DEL",		// KEY_PAD_DECIMAL,
	"[",			// KEY_LBRACKET,
	"]",			// KEY_RBRACKET,
	"SEMICOLON",	// KEY_SEMICOLON,
	"'",			// KEY_APOSTROPHE,
	"`",			// KEY_BACKQUOTE,
	",",			// KEY_COMMA,
	".",			// KEY_PERIOD,
	"/",			// KEY_SLASH,
	"\\",			// KEY_BACKSLASH,
	"-",			// KEY_MINUS,
	"=",			// KEY_EQUAL,
	"ENTER",		// KEY_ENTER,
	"SPACE",		// KEY_SPACE,
	"BACKSPACE",	// KEY_BACKSPACE,
	"TAB",			// KEY_TAB,
	"CAPSLOCK",		// KEY_CAPSLOCK,
	"NUMLOCK",		// KEY_NUMLOCK,
	"ESCAPE",		// KEY_ESCAPE,
	"SCROLLLOCK",	// KEY_SCROLLLOCK,
	"INS",			// KEY_INSERT,
	"DEL",			// KEY_DELETE,
	"HOME",			// KEY_HOME,
	"END",			// KEY_END,
	"PGUP",			// KEY_PAGEUP,
	"PGDN",			// KEY_PAGEDOWN,
	"PAUSE",		// KEY_BREAK,
	"SHIFT",		// KEY_LSHIFT,
	"RSHIFT",		// KEY_RSHIFT,
	"ALT",			// KEY_LALT,
	"RALT",			// KEY_RALT,
	"CTRL",			// KEY_LCONTROL,
	"RCTRL",		// KEY_RCONTROL,
	"LWIN",			// KEY_LWIN,
	"RWIN",			// KEY_RWIN,
	"APP",			// KEY_APP,
	"UPARROW",		// KEY_UP,
	"LEFTARROW",	// KEY_LEFT,
	"DOWNARROW",	// KEY_DOWN,
	"RIGHTARROW",	// KEY_RIGHT,
	"F1",			// KEY_F1,
	"F2",			// KEY_F2,
	"F3",			// KEY_F3,
	"F4",			// KEY_F4,
	"F5",			// KEY_F5,
	"F6",			// KEY_F6,
	"F7",			// KEY_F7,
	"F8",			// KEY_F8,
	"F9",			// KEY_F9,
	"F10",			// KEY_F10,
	"F11",			// KEY_F11,
	"F12",			// KEY_F12,

	"CAPSLOCKTOGGLE",	// KEY_CAPSLOCKTOGGLE,
	"NUMLOCKTOGGLE",	// KEY_NUMLOCKTOGGLE,
	"SCROLLLOCKTOGGLE", // KEY_SCROLLLOCKTOGGLE,

	"MOUSE1",		// MOUSE_LEFT,
	"MOUSE2",		// MOUSE_RIGHT,
	"MOUSE3",		// MOUSE_MIDDLE,
	"MOUSE4",		// MOUSE_4,
	"MOUSE5",		// MOUSE_5,

	"MWHEELUP",		// MOUSE_WHEEL_UP
	"MWHEELDOWN",	// MOUSE_WHEEL_DOWN
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void sym_config_callback(string sName, string sOld, string sNew)
///
/// \brief	Called when the sym_config convar is changed.
///
/// \param	sName	The name of the ConVar.
/// \param	sOld 	The old value.
/// \param	sNew 	The new value.
////////////////////////////////////////////////////////////////////////////////////////////////////

void sym_config_callback(SymConVarBase* pConvar, string sOld, string sNew)
{
	if (Class::Console && sOld != sNew)
		Class::Console->LoadConfig(sNew);
}
SymConVar<string>	sym_config("sym_config",	"Name of current configuration file (loaded from and saved to).  This convar will change "
	"settings when changed (if the value changed to is a valid config)", "default.cfg", sym_config_callback);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ConsoleManager::ConsoleManager()
///
/// \brief	Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

ConsoleManager::ConsoleManager()
{
	for (uint32 i = 0; i < sizeof(this->m_sKeyBindings) / sizeof(string); i++)
	{
		this->m_sKeyBindings[i] = "";
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ConsoleManager::~ConsoleManager()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

ConsoleManager::~ConsoleManager()
{
	// Instances are managed by the console
	for (IConsoleEntry* e : this->m_vEntries)
	{
		if (dynamic_cast<SymConVarBase*>(e) != NULL)
		{
			SymConVarBase* cv = (SymConVarBase*)e;
			DeleteAndNull(cv);
		}
		else if (dynamic_cast<SymConCommand*>(e) != NULL)
		{
			SymConCommand* cv = (SymConCommand*)e;
			DeleteAndNull(cv);
		}
	}

	this->m_vEntries.clear();
	this->m_vAliases.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string ConsoleManager::CfgSafeStr(string sText)
///
/// \brief	Makes a string safe for configuration (.cfg) files.
///
/// \param	sText	The text.
////////////////////////////////////////////////////////////////////////////////////////////////////

string ConsoleManager::CfgSafeStr(string sText)
{
	string buf;

	for (char c : sText)
	{
		switch (c)
		{
		case '\n':
			buf += "\\n"; break;
		case '\t':
			buf += "\\t"; break;
		case '\0':
			buf += "\\0"; break;
		case '"':
			buf += "\\\""; break;
		case ';':
			buf += "\\;"; break;
		default:
			buf += c;
		}
	}

	return "\"" + buf + "\"";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void CConsole::Remove(string sName)
///
/// \brief Removes an entry by name.
///
/// \param sName The name of the entry to remove.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleManager::RemoveEntry(string sName)
{
	string b = sName;
	transform(b.begin(), b.end(), b.begin(), ::tolower);

	for (unsigned int i = 0; i < this->m_vEntries.size(); i++)
	{
		string a = this->m_vEntries[i]->GetName();
		
		// Transform to lowercase
		transform(a.begin(), a.end(), a.begin(), ::tolower);
		

		if (a == b)
			this->m_vEntries.erase(this->m_vEntries.begin() + i);
	}

	if (Class::Network)
		Class::Network->Send(Lib::IO::IpcRemCommand(b));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void CConsole::Register(IConsoleEntry* icEntry)
///
/// \brief Register an entry.
///
/// \param icEntry The entry to register.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleManager::RegisterEntry(IConsoleEntry* icEntry)
{
	if (this->FindEntry(icEntry->GetName()))
		return;

	this->m_vEntries.push_back(icEntry);

	if (Class::Network)
		Class::Network->Send(Lib::IO::IpcAddCommand(icEntry->GetName()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn IConsoleEntry* CConsole::FindEntry(string sName)
///
/// \brief Searches for any entry with a name sName.  Returns NULL if nothing
/// 	   was found.
///
/// \param sName The name of the entry.
///
/// \return An entry by the name of sName, or NULL.
////////////////////////////////////////////////////////////////////////////////////////////////////

IConsoleEntry* ConsoleManager::FindEntry(string sName)
{
	for (unsigned int i = 0; i < this->m_vEntries.size(); i++)
	{
		string a = this->m_vEntries[i]->GetName();
		string b = sName;

		// Transform to lowercase
		transform(a.begin(), a.end(), a.begin(), ::tolower);
		

		if (a == b)
			return this->m_vEntries[i];
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn bool CConsole::PerformCommand(string sCommand)
///
/// \brief Performs a command.  The command could be setting a convar or executing a console command.
///
/// \param sCommand The command string to parse.
///
/// \return true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

vector<CommandArgs> ConsoleManager::ParseCommand(string sCommand)
{
	vector<CommandArgs> args;

	if (sCommand.length() == 0)
		return args;

	// Whether or not we've open quotes
	bool isQuoteOpen	= false;

	// Current string buffer
	string buffer		= "";

	CommandArgs cur = CommandArgs(); 

	// Parse arguments
	for (uint32 i = 0; i < sCommand.length(); i++)
	{
		if (sCommand[i] == '"')
		{
			if (i > 0 && sCommand[i - 1] == '\\')
			{
				// Replace backslash
				buffer[buffer.length() - 1] = sCommand[i];
			}
			else
				isQuoteOpen = !isQuoteOpen;
		}
		else if (sCommand[i] == ' ' && !isQuoteOpen)
		{
			cur.Push(buffer);
			buffer.clear();
		}
		else if (sCommand[i] == ';')
		{
			if (i > 0 && sCommand[i - 1] == '\\')
			{
				// Remove blackslash
				buffer[buffer.length() - 1] = sCommand[i];
			}
			else
			{
				// Push the last of the string into the current command (done before name checking
				// in case the buffer is the name)
				cur.Push(buffer);
				buffer.clear();

				if (cur.GetCommandName() != "")
				{
					// Push the command onto the list of commands and recreate with a fresh command
					args.push_back(cur);
					cur = CommandArgs();
				}
			}
		}
		else 
		{
			buffer += sCommand[i];
		}
	}

	// Add the remaining buffer as an argument in the list
	cur.Push(buffer); buffer.clear();

	// Add current command to list
	if (cur.GetCommandName() != "")
		args.push_back(cur);

	return args;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ConsoleManager::PerformCommand(CommandArgs args, bool bSilent)
///
/// \brief	Gets a convar/concommand from a CommandArgs class and executes it, passing arguments
/// 		described in the same CommandArgs class.
///
/// \param	args   	The arguments.
/// \param	bSilent	If this is true, nothing will be printed except for error messages if they 
/// 				occur.  This is used for the exec command so that lines that are for example
/// 				changing convars don't print anything, lines that aren't anything (neither
/// 				a convar or concommand) will still print error messages.
///
/// ### return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleManager::PerformCommand(CommandArgs args, bool bSilent, bool bSave)
{
	string name = this->ProcessAliases(args.GetCommandName());

	// If the command was aliased, rebuild and execute
	// new command(s) NOTE: aliased command may contain
	// semicolons, resulting in a single command calling multiple
	if (name != args.GetCommandName())
	{
		string command = name + " " + args.GetArgString();
		for (CommandArgs a : this->ParseCommand(command))
		{
			a.SetAlias(args.GetCommandName());
			this->PerformCommand(a, bSilent, bSave);
		}
	
		// Abort execution
		return;
	}

	args.SetSilent(bSilent);

	if (name != "")
	{
		IConsoleEntry* entry;
		if ((entry = this->FindEntry(name)) != NULL)
		{
			if (dynamic_cast<SymConVarBase*>(entry) != NULL)
			{
				SymConVarBase* cvBase = (SymConVarBase*)entry;

				if (args.GetArgCount() == 0)
				{
					if (!bSilent)
					{
						PrintLine(CONSOLE_RED "\"%s\" = \"%s\" " CONSOLE_GREY "(def. \"%s\")", cvBase->GetName().c_str(), cvBase->GetValueEx().c_str(),
							cvBase->GetDefaultValueEx().c_str());

						if (cvBase->GetHelp() != "")
							PrintLine(CONSOLE_GREY " - %s", cvBase->GetHelp().c_str());
					}
				}
				else
				{
					if (!bSilent)
					{
						PrintGoodL("%s = " CONSOLE_GREEN "%s" CONSOLE_GREY " (" CONSOLE_RED "%s " CONSOLE_GREY "=>" CONSOLE_GREEN " %s" CONSOLE_GREY ")",
							cvBase->GetName().c_str(), args.GetArgString().c_str(), cvBase->GetValueEx().c_str(),
							args.GetArgString().c_str());
					}

					// Set value
					cvBase->SetValueEx(args.GetArgString());

					// Save config (only if bSave is true, bSave
					// is false when the command is being run by binds)
					if (bSave)	
						this->SaveConfig(sym_config.GetValue());
				}
			}
			else if (dynamic_cast<SymConCommand*>(entry) != NULL)
			{
				SymConCommand* ccBase = (SymConCommand*)entry;
				ccBase->Execute(args);
			}
			else if (dynamic_cast<LuaConCommand*>(entry) != NULL)
			{
				lua_State* L = Class::StateManager->GetState(CLIENT);
				LuaConCommand* ccBase = (LuaConCommand*)entry;

				// Execute the command (leaves function & args on
				// Lua stack)
				if (L)
					ccBase->Execute(args, L);

				// Call
				Class::ScriptManager->SafeCall(L, "/sym/command/" + name, 3, 0);
			}
		}
		else 
		{
			if (args.GetAlias() != "")
			{
				PrintBadL("No such command \"%s\" (alias: %s => %s)", name.c_str(), 
					args.GetAlias().c_str(), name.c_str());
			}
			else
			{
				PrintBadL("No such command \"%s\"", args.GetCommandName().c_str());
			}
		}
		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ConsoleManager::AddAlias(string sTarget, string sReplace)
///
/// \brief	Adds an alias to that will replace sTarget with sReplace when commands are ran.
///
/// \param	sTarget 	Target string.
/// \param	sReplace	Replacement string.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ConsoleManager::AddAlias(string sTarget, string sReplace)
{
	transform(sTarget.begin(), sTarget.end(), sTarget.begin(), ::tolower);
	transform(sReplace.begin(), sReplace.end(), sReplace.begin(), ::tolower);

	for (uint32 i = 0; i < this->m_vAliases.size(); i++)
	{
		// Replace below
		if (this->m_vAliases[i].first == sTarget)
			this->m_vAliases.erase(this->m_vAliases.begin() + i);

		// No infinite aliasing loops
		if (this->m_vAliases[i].first == sReplace && 
			this->m_vAliases[i].second == sTarget)
		{
			return false;
		}
	}

	this->m_vAliases.push_back(make_pair(sTarget, sReplace));

	// Save aliases
	this->SaveConfig(sym_config.GetValue());

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool ConsoleManager::RemoveAlias(string sAlias)
///
/// \brief	Removes an alias described by sAlias.
///
/// \param	sAlias	The alias.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ConsoleManager::RemoveAlias(string sAlias)
{
	transform(sAlias.begin(), sAlias.end(), sAlias.begin(), ::tolower);

	for (uint32 i = 0; i < this->m_vAliases.size(); i++)
	{
		if (this->m_vAliases[i].first == sAlias)
		{
			this->m_vAliases.erase(this->m_vAliases.begin() + i);

			// Save aliases
			this->SaveConfig(sym_config.GetValue());

			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string ConsoleManager::ProcessAliases(string sAlias)
///
/// \brief	Processes a string, checking for aliases.  For example if aliases were added for 'b'
/// 		so that 'b' was replaced with 'a' and ProcessAlias was called on 'b', the returned
/// 		string would be 'a'.  This function will chain aliases, such that if an alias also
/// 		replaced 'a' with 'c', then the returned string would be 'c' if the input is 'a' or
/// 		'b'.
///
/// \param	sAlias	The alias.
///
/// \return	See above.
////////////////////////////////////////////////////////////////////////////////////////////////////

string ConsoleManager::ProcessAliases(string sAlias)
{
	transform(sAlias.begin(), sAlias.end(), sAlias.begin(), ::tolower);

	for (uint32 i = 0; i < this->m_vAliases.size(); i++)
	{
		if (this->m_vAliases[i].first == sAlias)
			return this->ProcessAliases(this->m_vAliases[i].second);
	}

	return sAlias;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	vector<pair<string, string>> ConsoleManager::GetAliases()
///
/// \brief	Returns registered aliases.
///
/// \return	The aliases.
////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<string, string>> ConsoleManager::GetAliases()
{
	return this->m_vAliases;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool ConsoleManager::AddBind(ButtonCode_t code, string sBind)
///
/// \brief	Binds the button code to the string, so that whenever the button's pressed, 
/// 		the string will be executed.
///
/// \param	code 	The code.
/// \param	sBind	The bind.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ConsoleManager::AddBind(ButtonCode_t code, string sBind)
{
	if (code <= (sizeof(this->m_sKeyBindings) / sizeof(string)))
	{
		if (this->m_sKeyBindings[code] == "")
		{
			this->m_sKeyBindings[code] = sBind;
			this->SaveConfig(sym_config.GetValue());

			return true;
		}
		else
			return false;
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool ConsoleManager::RemoveBind(ButtonCode_t code)
///
/// \brief	Removes the bind described by code.
///
/// \param	code	The code.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ConsoleManager::RemoveBind(ButtonCode_t code)
{
	if (code <= (sizeof(this->m_sKeyBindings) / sizeof(string)))
	{
		if (this->m_sKeyBindings[code] != "")
		{
			this->m_sKeyBindings[code] = "";
			this->SaveConfig(sym_config.GetValue());

			return true;
		}
		else
			return false;
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool ConsoleManager::RunBind(ButtonCode_t code, bool bReleased)
///
/// \brief	Executes a bind, returns true if a bind was ran.
///
/// \param	code	 	The code.
/// \param	bReleased	True if the key even was the key being released.
///
/// \return	True to divert the key.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ConsoleManager::RunBind(ButtonCode_t code, bool bReleased)
{
	if (!Src::Engine->IsInGame() && !sym_bind_menu.GetValue())
		return false;

	if (code <= (sizeof(this->m_sKeyBindings) / sizeof(string)))
	{
		if (this->m_sKeyBindings[code] != "")
		{
			for (CommandArgs args : this->ParseCommand(this->m_sKeyBindings[code]))
			{
				if (bReleased && args.GetCommandName()[0] == '+')
				{
					string name = args.GetCommandName();
					name[0] = '-';

					args.SetCommandName(name);
				}
				else if (bReleased)
					return false;

				this->PerformCommand(args, sym_bind_silent.GetValue() == 1 ? true : false, false);
			}

			return sym_bind_divert.GetValue() == 1 ? true : false;
		}
		else
			return false;
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	vector<pair<ButtonCode_t, string>> ConsoleManager::GetBindings()
///
/// \brief	Returns a vector of active key bindings.
///
/// \return	The bindings.
////////////////////////////////////////////////////////////////////////////////////////////////////

vector<pair<ButtonCode_t, string>> ConsoleManager::GetBindings()
{
	vector<pair<ButtonCode_t, string>> v;

	for (uint32 i = 0; i < sizeof(this->m_sKeyBindings) / sizeof(string); i++)
	{
		if (this->m_sKeyBindings[i] != "")
		{
			v.push_back(make_pair((ButtonCode_t)i, this->m_sKeyBindings[i]));
		}
	}

	return v;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ConsoleManager::SaveConfig(string sFileName)
///
/// \brief	Saves the current state (aliases, binds and convars) to a file that can be executed.
///
/// \param	sFileName	Name of the config.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleManager::SaveConfig(string sFileName)
{
	if (sym_save.GetValue() < 1)
		return;

	string buf;

	// Consolve variables
	for (uint32 i = 0; i < this->m_vEntries.size(); i++)
	{
		IConsoleEntry* e = this->m_vEntries[i];

		if (dynamic_cast<SymConVarBase*>(e))
		{
			SymConVarBase* base = (SymConVarBase*)e;

			string first = this->CfgSafeStr(base->GetName());

			// Key + padding
			buf += first;
			for (uint32 i = 0; i < 30 - first.length(); i++)
				buf += ' ';

			// Value
			buf += this->CfgSafeStr(base->GetValueEx()) + '\n';
		}
	}

	// Aliases
	for (uint32 i = 0; i < this->m_vAliases.size(); i++)
	{
		pair<string, string> p = this->m_vAliases[i];

		string first = "\"alias\"";

		// Key + padding
		buf += first;
		for (uint32 i = 0; i < 30 - first.length(); i++)
			buf += ' ';

		// Value
		buf += this->CfgSafeStr(p.first) + " " + this->CfgSafeStr(p.second) + '\n';
	}

	// Binds
	for (uint32 i = 0; i < (sizeof(this->m_sKeyBindings) / sizeof(string)); i++)
	{
		string binding = this->m_sKeyBindings[i];

		if (binding != "")
		{
			string first = "\"bind\"";
			string button = this->CfgSafeStr(g_pszButtonNames[i]);

			// Key + padding
			buf += first;
			for (uint32 i = 0; i < 30 - first.length(); i++)
				buf += ' ';

			// Value
			buf += (button + " " + this->CfgSafeStr(binding) + '\n');
		}
	}

	// Write the buffer to file
	WriteFileG(sFileName, buf);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ConsoleManager::LoadConfig(string sFileName)
///
/// \brief	Loads every ConVar from a config, nothing will happen if the ConFig doesn't exist.
/// 		
/// 		EDIT: 04/11/2014: DEPERACTED
///
/// \param	sFileName Name of the config.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleManager::LoadConfig(string sFileName)
{
	if (FileExistsG(sFileName))
	{
		string contents = ReadFileG(sFileName);

		// Put the contents of the file into a stream
		Lib::IO::Stream* str = new Lib::IO::Stream();
		str->Write(contents, false);
		str->Seek(0);

		while (str->Tell() != str->GetStreamSize())
		{
			string name = Mem::SanitizeUserInput(str->ReadUntil(' ', false));
			string value = Mem::SanitizeUserInput(str->ReadUntil('\n', false));

			IConsoleEntry* e = this->FindEntry(name);

			if (e && dynamic_cast<SymConVarBase*>(e))
			{
				SymConVarBase* base = (SymConVarBase*)e;

				// Set the value of the newly registered ConVar
				// to what we have stored for it
				base->SetValueEx(value);
			}
		}

		delete str;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ConsoleManager::Exec(string sText)
///
/// \brief	Executes a block of text as console command input.  The text can contain newlines,
/// 		carriage returns and tabs.
///
/// \param	sText	The text.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleManager::Exec(string sText)
{
	string line = "";

	for (char c : sText)
	{
		// Tabs are completly ignored
		if (c == '\t')
			continue;

		// Process line
		if (c == '\n' || c == '\r')
		{
			if (line == "")
				continue;

			for (CommandArgs args : this->ParseCommand(line))
			{
				this->PerformCommand(args, true);
			}

			line = "";

			continue;
		}

		// Build line
		line += c;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int ConsoleManager::GetConvarCount()
///
/// \brief	Returns the number of console variables registered.
////////////////////////////////////////////////////////////////////////////////////////////////////

int ConsoleManager::GetConvarCount()
{
	int count = 0;
	for (uint32 i = 0; i < this->m_vEntries.size(); i++)
	{
		IConsoleEntry* e = this->m_vEntries[i];

		if (dynamic_cast<SymConVarBase*>(e))
			count++;
	}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int ConsoleManager::GetConComCount()
///
/// \brief	Returns the number of console commands registered.
////////////////////////////////////////////////////////////////////////////////////////////////////

int ConsoleManager::GetConComCount()
{
	int count = 0;
	for (uint32 i = 0; i < this->m_vEntries.size(); i++)
	{
		IConsoleEntry* e = this->m_vEntries[i];

		if (dynamic_cast<SymConCommandBase*>(e))
			count++;
	}

	return count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int ConsoleManager::GetBindCount()
///
/// \brief	Returns the number of binds registered.
////////////////////////////////////////////////////////////////////////////////////////////////////

int ConsoleManager::GetBindCount()
{
	return this->GetBindings().size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	vector< IConsoleEntry* > ConsoleManager::GetConsoleEntries()
///
/// \brief	Returns each console entry in the m_vEntries vector.
////////////////////////////////////////////////////////////////////////////////////////////////////

vector< IConsoleEntry* > ConsoleManager::GetConsoleEntries()
{
	return this->m_vEntries;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void RegistryEntryProxy(IConsoleEntry* entry);
///
/// \brief	Description in header.
////////////////////////////////////////////////////////////////////////////////////////////////////

void ConsoleManager::RegistryEntryProxy(IConsoleEntry* entry)
{
	// We don't really need to check if Global::Console exists since as it
	// loads before the CRT.  
	if (Class::Console)
		Class::Console->RegisterEntry(entry);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	AliasProxy::AliasProxy(string sA, string sB)
///
/// \brief	Description in header.
////////////////////////////////////////////////////////////////////////////////////////////////////

AliasProxy::AliasProxy(string sA, string sB)
{
	if (Class::Console)
		Class::Console->AddAlias(sA, sB);
}

NAMESPACE_END2