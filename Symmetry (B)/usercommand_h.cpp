/**                  ====================                      *
*      _____                                __               *
*     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
*     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
*    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
*   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
*        /____/                                  /____/      *
*   - by Defc0n                                              *
*                  ====================                      **/

#include "usercommand_h.h"
#include "global.h"

#include "symconvar.h"

#include "src_lib.h"

#include "global.h"
#include "src_random.h"

#include "symcmd.h"

#include "patterns.h"

NAMESPACE_CORE(Hooks)

// Typedefs
typedef CUserCmd* (__thiscall* GetUserCmd_t)(CInput*, int);
typedef bool(__thiscall* CreateMove_t)(void*, int, float, bool);

Detours::VMT* g_CInputHook = NULL;
Detours::VMT* g_ClientHook = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CUserCmd* __stdcall GetUserCmd(int iSeqNum)
///
/// \brief	Hooks CInput::GetUserCmd.
////////////////////////////////////////////////////////////////////////////////////////////////////

CUserCmd* __stdcall GetUserCmd(int iSeqNum)
{
	static CUserCmd* commands = *(CUserCmd **)((DWORD)Src::Input + 0xC4);
	return &commands[iSeqNum % 90];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void __fastcall CreateMove(void* pClient, dword _, int iSeq, float f1, bool b1)
///
/// \brief	Hooks CHLClient::CreateMove.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall CreateMove(void* pClient, dword _, int iSeq, float f1, bool b1)
{
	// Needs to be done in game thread
	Class::ScriptManager->ProcessQueue();

	void* _ebp; __asm MOV _ebp, EBP;

	// bSendPacket
	bool* send = (bool*)(*(char**)_ebp - 1);

	// Return addres
	dword* ret = (dword*)(*(char**)_ebp + 4);

	((CreateMove_t)g_ClientHook->GetMethod(CLIENT_CREATEMOVE))(pClient, iSeq, f1, b1);

	CInput::CVerifiedUserCmd* verifiedCommands = *(CInput::CVerifiedUserCmd **)((DWORD)Src::Input + 0xC8);
	CInput::CVerifiedUserCmd* verifiedCommand = &verifiedCommands[iSeq % 90];
	CUserCmd* cmd = GetUserCmd(iSeq);

	if (cmd)
	{
		SymCmd symcmd(cmd, send, ret);

        QAngle old = cmd->viewangles;

		symcmd.RunSpeedhack();		    // can be called in any order
		symcmd.RunBhop();			    // can be called in any order
		symcmd.RunFakeLag();		    // can be ran at any time

		symcmd.RunAimbot();			    // must be first to touch viewangles
		symcmd.RunRecoilFix();		    // must be called after aimbot
		symcmd.RunAA();				    // must be called after aimbot
		symcmd.RunFixMove();		    // must be called after viewangle modifications
		symcmd.RunAutoStrafe();		    // must be called after aimbot
		symcmd.RunFakeCrouch();		    // must be called after aimbot
	}
	verifiedCommand->m_cmd = *cmd;
	verifiedCommand->m_crc = Patterns::CUserCmd__GetChecksum(cmd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void UserCommandH::Hook()
///
/// \brief	Hooks all of the functions in this file.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCommandH::Hook()
{
	if (g_CInputHook)
		UserCommandH::UnHook();

	if (!Src::Input || !Src::Client)
		return;

	g_CInputHook = new Detours::VMT((PDWORD*)Src::Input);
	g_CInputHook->Hook((dword)GetUserCmd, CINPUT_GETUSERCMD);

	g_ClientHook = new Detours::VMT((PDWORD*)Src::Client);
	g_ClientHook->Hook((dword)CreateMove, CLIENT_CREATEMOVE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void UserCommandH::UnHook(Detours::VMT* pVMT)
///
/// \brief	Un-hooks all hooks in this file
////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCommandH::UnHook()
{
	// Destructor unhooks
	DeleteAndNull(g_CInputHook);
	DeleteAndNull(g_ClientHook);
}

NAMESPACE_END2