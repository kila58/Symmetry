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

#include "lua_interface_manager.h"

#include "patterns.h"

#include "filea.h"

using namespace Lib;
using namespace Core;
using namespace Core::Console;

NAMESPACE_CORE_B

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void lua_run(CommandArgs args)
///
/// \brief	Usage: lua_run <code>.
////////////////////////////////////////////////////////////////////////////////////////////////////

void lua_run(CommandArgs args)
{
	string str = args.GetArgString();

	if (str.size() == 0)
	{
		PrintBadL("String is empty");
	}
	else if (Class::StateManager->GetInterface(CLIENT) == NULL)
	{
		PrintBadL("Lua cannot be ran at this time");
	}
	else
	{
		Class::ScriptManager->AddFile(str, "/sym/lua_run/", false);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void lua_file(CommandArgs args)
///
/// \brief	Usage: lua_file <file>.
///
/// \param	args	The arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////

void lua_file(CommandArgs args)
{
	string str = args.GetArgString();

	if (str.size() == 0 || !FileExistsG(str))
	{
		PrintBadL("File doesn't exist.");
	}
	else if (Class::StateManager->GetInterface(CLIENT) == NULL)
	{
		PrintBadL("Lua cannot be ran at this time");
	}
	else
	{
		Class::ScriptManager->AddFile(ReadFileG(str), "/sym/lua_file/", false);
	}
}

// Add the concommands
MakeConCommand4(lua_run, "Runs a lua string on the client state", "lua_run <code>", 1);
MakeConCommand4(lua_file, "Runs a lua file", "lua_file <file>", 1);

NAMESPACE_END1