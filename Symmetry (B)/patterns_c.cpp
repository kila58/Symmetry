/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "console_manager.h"
#include "concommand.h"
#include "convar.h"

#include "patterns.h"
#include "filea.h"

#include "global.h"

using namespace Lib;
using namespace Core::Console;
using namespace Core::Patterns;

#ifdef BUILDMODE_BETA

NAMESPACE_CORE_B

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn void p_dump(CommandArgs args)
///
/// \brief Writes all of addresses of patterns to the console.
///
/// \param args The arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////

void p_dump(CommandArgs args)
{
	for (int i = 0; i < Patterns::GetPatternCount(); i++)
	{
		if (Patterns::GetPattern(i) == NULL || Patterns::GetPattern(i)->ppAddress == NULL || *Patterns::GetPattern(i)->ppAddress == NULL)
			continue;

#if defined ( BUILDMODE_TEST ) || defined( BUILDMODE_BETA )
		string sName =  Patterns::GetPattern(i)->sName;
#else
		string sName = "SIG[ " + to_string(i) + "]";
#endif

		PrintGoodL("%s resolved at address : %X", sName.c_str(), *Patterns::GetPattern(i)->ppAddress);
	}
}

// Add the concommands
MakeConCommand4(p_dump, "Dump pattern adresses.", "p_dump", 0);

NAMESPACE_END1

#endif