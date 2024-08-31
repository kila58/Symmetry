/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "command_args.h"
#include "concommand.h"

#include "global.h"

#include "filea.h"

#include "console_manager.h"

#include "netmsg_ipc_clear.h"

using namespace Core;
using namespace Core::Console;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void help(CommandArgs arg)
////////////////////////////////////////////////////////////////////////////////////////////////////

void help(CommandArgs arg)
{
	if (arg.GetArgCount() > 0)
	{
		string name = arg.GetArgAt(0);

		IConsoleEntry* e = Class::Console->FindEntry(name);

		if (e)
		{
			if (dynamic_cast<SymConCommandBase*>(e))
			{
				SymConCommandBase* cmd = (SymConCommandBase*)e;

				PrintLine(CONSOLE_RED "%s", cmd->GetUsage().c_str());
				PrintLine(CONSOLE_DARK_GREY "\t- %s", cmd->GetHelp().c_str());

				return;
			}
			else
			{
				PrintBadL("'%s' isn't a console command", name.c_str());
				return;
			}
		}
		else
		{
			PrintBadL("Couldn't find help for '%s'", name.c_str());
			return;
		}
	}

	vector<IConsoleEntry*> entries = Class::Console->GetConsoleEntries();
	PrintLine("Showing %d console commands and %d convars", Class::Console->GetConComCount(),
		Class::Console->GetConvarCount());

	int count;

	PrintLine("");
	PrintLine("----");
	PrintLine("- Console Commands");
	PrintLine("----");
	PrintLine("");

	count = 0;
	for (IConsoleEntry* e : Class::Console->GetConsoleEntries())
	{
		if (dynamic_cast<SymConCommandBase*>(e) != NULL)
		{
			count++;
			SymConCommandBase* cmd = (SymConCommandBase*)e;

			PrintLine(CONSOLE_YELLOW  "%02d) " CONSOLE_RED "%s", count, cmd->GetUsage().c_str());
			PrintLine(CONSOLE_DARK_GREY "\t\t- %s", cmd->GetHelp().c_str());
		}
	}
	
	PrintLine("");
	PrintLine("----");
	PrintLine("- Console Variables");
	PrintLine("----");
	PrintLine("");

	count = 0;
	for (IConsoleEntry* e : Class::Console->GetConsoleEntries())
	{
		if (dynamic_cast<SymConVarBase*>(e) != NULL)
		{
			count++;
			SymConVarBase* cv = (SymConVarBase*)e;

			PrintLine(CONSOLE_YELLOW "%02d) " CONSOLE_RED "\"%s\"\t=\t\"%s\"\t" CONSOLE_GREY "(def. \"%s\")", count,
				cv->GetName().c_str(), cv->GetValueEx().c_str(), cv->GetDefaultValueEx().c_str());
			PrintLine(CONSOLE_DARK_GREY "\t\t- %s", cv->GetHelp().c_str());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void find(CommandArgs arg)
////////////////////////////////////////////////////////////////////////////////////////////////////

void find(CommandArgs arg)
{
	string str = arg.GetArgAt(0);
	transform(str.begin(), str.end(), str.begin(), ::tolower);

	vector<pair<size_t, SymConVarBase*>>		convars;
	vector<pair<size_t, SymConCommandBase*>>	concommands;

	// Find matching entries
	for (IConsoleEntry* e : Class::Console->GetConsoleEntries())
	{
		string name = e->GetName();
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		size_t pos = name.find(str);
		if (pos != string::npos)
		{
			if (dynamic_cast<SymConVarBase*>(e))
			{
				convars.push_back(make_pair(pos, (SymConVarBase*)e));
			}
			else if (dynamic_cast<SymConCommandBase*>(e))
			{
				concommands.push_back(make_pair(pos, (SymConCommandBase*)e));
			}
		}
	}

	int total = (int)(convars.size() + concommands.size());

	if (total == 0)
	{
		PrintLine("No matching console entries for '%s'", str.c_str());
		return;
	}

	// Print matching convars
	if (convars.size())
	{
		PrintLine("");
		PrintLine("----");
		PrintLine("- Console Variables (%d matches)", (int)convars.size());
		PrintLine("----");
		PrintLine("");

		for (pair<size_t, SymConVarBase*> e : convars)
		{
			size_t pos = e.first;
			string name = e.second->GetName();

			string first = pos == 0 ? "" : name.substr(0, pos);
			string middle = name.substr(pos, str.length());
			string last = name.substr(first.length() + middle.length(), string::npos);

			PrintLine(CONSOLE_GREY "%s" CONSOLE_RED "%s" CONSOLE_GREY "%s", first.c_str(),
				middle.c_str(), last.c_str());
		}
	}

	// Print matching concommands
	if (concommands.size())
	{
		PrintLine("");
		PrintLine("----");
		PrintLine("- Console Commands (%d matches)", (int)concommands.size());
		PrintLine("----");
		PrintLine("");

		for (pair<size_t, SymConCommandBase*> e : concommands)
		{
			size_t pos = e.first;
			string name = e.second->GetName();

			string first = pos == 0 ? "" : name.substr(0, pos);
			string middle = name.substr(pos, str.length());
			string last = name.substr(first.length() + middle.length(), string::npos);

			PrintLine(CONSOLE_GREY "%s" CONSOLE_RED "%s" CONSOLE_GREY "%s", first.c_str(),
				middle.c_str(), last.c_str());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void alias(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void alias(CommandArgs args)
{
	if (args.GetArgCount() < 1 && !args.IsSilent())
	{
		vector<pair<string, string>> aliases = Class::Console->GetAliases();

		if (!aliases.size())
		{
			PrintBadL("No aliases to list");
			return;
		}

		// List aliases
		PrintLine("Listing %d aliases", (int)aliases.size());

		for (uint32 i = 0; i < aliases.size(); i++)
		{
			PrintLine(CONSOLE_YELLOW "%02u) " CONSOLE_RED "\"%s\" = \"%s\"", i + 1,
				aliases[i].first.c_str(), aliases[i].second.c_str());
		}
	}
	else if (args.GetArgCount() > 1)
	{
		string s1 = args.GetArgAt(0);
		string s2 = args.GetArgAt(1);

		if (Class::Console->AddAlias(s1, s2))
		{
			if (!args.IsSilent())
				PrintGoodL("Added alias! (%s => %s)", s1.c_str(), s2.c_str());
		}
		else
		{
			if (!args.IsSilent())
				PrintBadL("Couldn't add alias for '%s'!", s1.c_str());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void unalias(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void unalias(CommandArgs args)
{
	if (Class::Console->RemoveAlias(args.GetArgAt(0)))
	{
		PrintGoodL("Removed alias %s", args.GetArgAt(0).c_str());
	}
	else
	{
		PrintBadL("Couldn't find alias '%s'", args.GetArgAt(0).c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void exec(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void exec(CommandArgs args)
{
	string filename = args.GetArgAt(0);

	if (FileExistsG(filename))
	{
		if (!args.IsSilent())
			PrintGoodL("Executing %s ..", filename.c_str());

		Class::Console->Exec(ReadFileG(filename));
	}
	else
	{
		if (!args.IsSilent())
			PrintBadL("Can't find file '%s'!", filename.c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void bind(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void bind(CommandArgs args)
{
	string key = args.GetArgAt(0);
	string bind = args.GetArgAt(1);

	transform(key.begin(), key.end(), key.begin(), ::toupper);

	for (uint32 i = BUTTON_CODE_NONE; i <= MOUSE_LAST; i++)
	{
		if (key == g_pszButtonNames[i])
		{
			if (Class::Console->AddBind((ButtonCode_t)i, bind))
			{
				if (!args.IsSilent())
				{
					PrintGoodL("Bound '%s' to '%s'!", key.c_str(), bind.c_str());
				}
				
				return;
			}
			else
			{
				if (!args.IsSilent())
				{
					PrintBadL("'%s' is already bound! Use unbind to unbind the key", key.c_str());
				}
				
				return;
			}
		}
	}

	PrintBadL("'%s' is not a key (case-insensitive)", key.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void unbind(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void unbind(CommandArgs args)
{
	string key = args.GetArgAt(0);

	transform(key.begin(), key.end(), key.begin(), ::toupper);

	for (uint32 i = BUTTON_CODE_NONE; i <= MOUSE_LAST; i++)
	{
		if (key == g_pszButtonNames[i])
		{
			if (Class::Console->RemoveBind((ButtonCode_t)i))
			{
				if (!args.IsSilent())
				{
					PrintGoodL("'%s' unbound!", key.c_str());
				}
				return;
			}
			else
			{
				if (!args.IsSilent())
				{
					PrintBadL("'%s' is not bound!", key.c_str());
				}
				return;
			}
		}
	}

	PrintBadL("'%s' is not a key (case-insensitive)", key.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void unbindall(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void unbindall(CommandArgs args)
{
	int count = 0;

	for (pair<ButtonCode_t, string> bind : Class::Console->GetBindings())
	{
		if (Class::Console->RemoveBind(bind.first))
		{
			count++;
		}
	}

	if (!args.IsSilent())
	{
		if (count)
		{
			PrintGoodL("Unbound %d command(s)", count);
		}
		else
		{
			PrintBadL("No keys unbound!  (Nothing bound?)");
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void bind_map(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void bind_map(CommandArgs args)
{
	if (args.IsSilent())
		return;

	int count = Class::Console->GetBindCount();

	if (count)
	{
		PrintGoodL("Listing %d binds", count);

		for (pair<ButtonCode_t, string> bind : Class::Console->GetBindings())
		{
			const char* name = g_pszButtonNames[bind.first];

			if (name)
			{
				PrintLine("(%d) '%s' = '%s'", bind.first, name, bind.second.c_str());
			}
		}
	}
	else
		PrintBadL("No binds to list!");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void bind_map(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void bind_help(CommandArgs args)
{
	PrintLine("Binds are used to run strings on the console when keys are pressed.");
	PrintLine("Things to note when using binds: ");
	PrintLine("- If you bind a key to text that starts with '+', releasing the bound key will run the same text replacing the '+' with a '-' (minus)");
	PrintLine("- You can run multiple queries splitting them with a semicolon (;)");
	PrintLine("- Use the blackslash character (\\) to escape any special character (e.g the semicolon, the quote, etc)");
	PrintLine("- Where speed is key, binds are not used.  For example the aimbot should never be toggled with binds.  Use aim_key cvar for that");
	PrintLine("- You can use the alias command to create binds that need to use the release event.  For example, 'alias +speed \"move_speed 5\"' could be paired with 'alias -speed \"move_speed 1\".");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void clear(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void clear(CommandArgs args)
{
	Class::Network->Send(Lib::IO::IpcClear());
}

MakeConCommand4(help, "Shows help for all concommands & convars, or help for a specific concommand", "help [concommand name]", 0);
MakeConCommand4(find, "Searches for console entries", "find <string>", 1);
MakeConCommand4(alias, "Adds an alias, or lists aliases.  This will list aliases if no arguments are provided", "alias [string [replacement]]", 0);
MakeConCommand4(unalias, "Removes an alias", "unalias <string>", 1);
MakeConCommand4(exec, "Executes a file of console input", "exec <filename>", 1);
MakeConCommand4(bind, "Binds a key to a command or list of commands", "bind <key> <string>", 2);
MakeConCommand4(unbind, "Removes a key binding", "unbind <key>", 1);
MakeConCommand4(unbindall, "Removes all key bindings", "unbindall", 0);
MakeConCommand4(bind_map, "Prints a list of current bindings", "bind_map", 0);
MakeConCommand4(bind_help, "Prints some basic information about bindings", "bind_help", 0);
MakeConCommand4(clear, "Clears the Symmetry console", "clear", 0);

// Alias for help
MakeAlias("?", "help");