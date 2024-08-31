/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#ifdef BUILDMODE_BETA

#include "command_args.h"
#include "concommand.h"

#include "global.h"

#include "src_interface.h"

#include "symmetry.h"

// TEMP
#include "src_lib.h"
#include "patterns.h"
#include "titlebar.h"

#include "netvarutil.h"

using namespace Lib;
using namespace Core::Console;

NAMESPACE_CORE_B

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void sigshort(CommandArgs args)
///
/// \brief	Sigshorts the given arguments.
///
/// \param	args	The arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////

void sigshort(CommandArgs args)
{
	string strsig = args.GetArgAt(0);

	if (strsig == "")
	{
		PrintLine("Signature file is empty.");
	}
	else 
	{
		if (GetModuleHandle(args.GetArgAt(1).c_str()) || args.GetArgAt(1) == "this")
		{
			Mem::Signature sig = Mem::MakeSignature(strsig, args.GetArgAt(1) == "this" ? "" : args.GetArgAt(1));

			PrintLine("Shortening signature ..");

			dword wEntry = Mem::ResolveSignature(sig);
			int iSize	= strlen((char*)sig.cMask);

			cout << sig.iSize << endl;
					
			if (wEntry == 0)
			{
				PrintBadL("Cannot shorten sig that doesn't resolve!");
				return;
			}

			PrintLine("Adrress : %X", wEntry);
			PrintLine("Length  : %i", iSize);

			int i;
			for (i = 0; i < iSize; i++)
			{
				uchar cByte = sig.cPattern[iSize - i - 1];

				// Null the last character
				sig.cMask[iSize - i - 1] = '\0';
				sig.cPattern[iSize - i - 1] = '\0';

				// Does it still resolve properly?
				if (Mem::ResolveSignature(sig) == wEntry)
				{
					PrintLine("DROPPED : 0x%X", (int)cByte);

					// Remove 3 characters from the textstyle pattern
					strsig = strsig.substr(0, strsig.length() - 3);
				}
				else 
				{
					break;
				}
			}

			Lib::IO::WriteFile("out.txt", strsig);
			PrintGoodL("New signature written to out.txt (%i bytes long, %i bytes shorter than original)",
				iSize - i, i);

		}
		else 
		{
			PrintBadL("Bad module '%s'", args.GetArgAt(1).c_str());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void resolve(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void resolve(CommandArgs args)
{
	Mem::Signature sig = Mem::MakeSignature(args.GetArgAt(0), args.GetArgAt(1));
	dword address = Mem::ResolveSignature(sig);

	if (address == 0)
	{
		PrintBadL("Address didn't resolve");
	}
	else 
	{
		PrintGoodL("Resolved at address %X", address);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void d_shutdown(CommandArgs arg)
////////////////////////////////////////////////////////////////////////////////////////////////////

void shutdown(CommandArgs arg)
{
	Core::Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void d_getnetvar(CommandArgs arg)
////////////////////////////////////////////////////////////////////////////////////////////////////

void get_prop(CommandArgs arg)
{
	RecvTable* table = NetVarUtil::FindTable(arg.GetArgAt(0));

	if (table)
	{
		RecvProp* prop = NetVarUtil::FindProp(table, arg.GetArgAt(1));

		if (prop)
		{
			PrintGoodL("Found at 0x%X", prop->GetOffset());
		}
		else
		{
			PrintBadL("Prop not found");
		}
	}
	else
	{
		PrintBadL("Class not found");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void find_prop(CommandArgs arg)
////////////////////////////////////////////////////////////////////////////////////////////////////

void find_prop(CommandArgs arg)
{
	RecvTable* table = NetVarUtil::FindTable(arg.GetArgAt(0));

	if (table)
	{
		vector<RecvProp*> results = NetVarUtil::FindProps(table, arg.GetArgAt(1));

		if (results.size() == 0)
		{
			PrintBadL("No results found!");
		}
		else
		{
			for (RecvProp* prop : results)
			{
				PrintGoodL("%s @ 0x%X", prop->GetName(), prop->GetOffset());
			}
		}
	}
	else
	{
		PrintBadL("Class not found");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void d_datatables(CommandArgs arg)
////////////////////////////////////////////////////////////////////////////////////////////////////

void list_dt(CommandArgs arg)
{
	ClientClass* current = Src::Client->GetAllClasses();

	// Iterate over linked list until we have a matching name
	for (; current; current = current->m_pNext)
	{
		PrintLine("%s -> %X", current->GetName(), current);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void d_datatables(CommandArgs arg)
////////////////////////////////////////////////////////////////////////////////////////////////////

void find_dt(CommandArgs arg)
{
	vector<RecvTable*> results = NetVarUtil::FindTables(arg.GetArgAt(0));

	if (results.size() == 0)
	{
		PrintBadL("No results found!");
	}
	else
	{
		for (RecvTable* tbl : results)
		{
			PrintGoodL("%s -> %X", tbl->GetName(), tbl);
		}
	}
}

void test1(CRecvProxyData *pData, void *pStruct, void* pOut)
{
	float value = pData->m_Value.m_Float;

	// Max 89.0f client, 90.0f clamped
	if (value == 90.0f)
	{
		value -= 90.0f;
	}

	// Min -89.0f client, -90.0f clamped
	if (value == -90.0f)
	{
		value += 90.0f;
	}

	*(float*)pOut = value;
}

void test2(CRecvProxyData *pData, void *pStruct, void* pOut)
{
	*(float*)pOut = pData->m_Value.m_Float;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void test(CommandArgs args)
////////////////////////////////////////////////////////////////////////////////////////////////////

void test(CommandArgs args)
{
	SrcEntity* ply = IdxToEnt(Src::Engine->GetLocalPlayer());

	if (ply)
	{

		PrintDebugL("PLAYER_H = %d", ply->GetHealth());
		PrintDebugL("WEAPON = %X", SrcLib::GetActiveWeapon(ply));
		PrintDebugL("CLASS = %s", SrcLib::GetClassname((SrcEntity*)SrcLib::GetActiveWeapon(ply)));

	}
	else
	{

		PrintBadL("LocalPlayer not found");
		return;

	}
}

MakeConCommand4(sigshort, "Shortens a text-style signature", "sigshort <signature> <module name> [minimum]", 2);
MakeConCommand4(resolve, "Resolves a text-style pattern <pattern> in <module>", "resolve <pattern> <module>", 2);
MakeConCommand4(shutdown, "Shuts symmetry down.", "shutdown", 0);
MakeConCommand4(get_prop, "Retrieves a datatable prop.", "get_prop <class> <var>", 2);
MakeConCommand4(find_prop, "Finds a datatable prop.", "find_prop <class> <var>", 2);
MakeConCommand4(list_dt, "Lists datatable classes.", "list_dt", 0);
MakeConCommand4(find_dt, "Finds datatable classes.", "find_dt <regex substr>", 1);
MakeConCommand4(test, "Developer test command", "test", 0);

NAMESPACE_END1

#endif