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

#include "src_interface.h"

#include "symmetry.h"

#include "cvarutil.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void src_cvar(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void src_cvar(CommandArgs args)
{
	string name = args.GetArgAt(0);
	string value = args.GetArgAt(1);

	// Name is not case sensitive
	transform(name.begin(), name.end(), name.begin(), ::tolower);

	ConVar* cv = Src::CVar->FindVar(name.c_str());

	if (cv)
	{
		CvarUtil::SetCvar(cv, value);

		if (!args.IsSilent())
			PrintGoodL("Set source cvar %s => %s", name.c_str(), value.c_str());
	}
	else
	{
		if (!args.IsSilent())
			PrintBadL("Can't find source cvar '%s'", name.c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void src_rcc(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void src_rcc(CommandArgs args)
{
	if (!args.IsSilent())
		PrintGoodL("Running '%s' in source console..", args.GetArgString().c_str());

	Src::Engine->ClientCmd(args.GetArgString().c_str());
}

// Add the concommands
MakeConCommand4(src_cvar, "Sets a source convar", "src_cvar <name> <value>", 2);
MakeConCommand4(src_rcc, "Runs a source engine console command", "src_rcc <command...>", 1);