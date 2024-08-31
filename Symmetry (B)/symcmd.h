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

// Created each time a new command is passed to ClientModeShared::CreateMove
// the object is then destroyed when ClientModeShared::CreateMove returns to the
// caller.
class SymCmd
{
	// SymAimbot needs to access private members of this class
	friend class SymAimbot;

private:

	CUserCmd*	m_pCmd;				// command pointer
	CUserCmd	m_cmdOriginal;		// copy of original command

	bool*		m_bpSendPacket;		// bSendPacket in CL_Move
	dword*		m_dwReturnAddress;	// return address from CL_Move

	bool		m_bCallAgain;		// call command chain again

	bool		m_bHasAimTarget;	// do we have a valid aimbot target?  (this is only true when there's a target and the aimbot IS targeting them)

	bool		m_bFireCommand;		// can our weapon fire this command?

	static int	m_iMoveSpeed;		// must not be modified between packets

private:
	// Build another and send that one too
	void		CallAgain();

	// CL_SendMove manipulation
	void		BlockPacket(bool bBlock);
	bool		IsBlocked();

	// Restore states from original command
	void		RestoreAngles();
	void		RestoreMove();
	void		RestoreButtons();

public:

	SymCmd(CUserCmd* pCmd, bool* bpSendPacket, dword* dwReturnAddress);
	~SymCmd();

	// Public usercommand modification methods
	void RunSpeedhack();				// faster moving
	void RunAimbot();					// SymAimbot
	void RunFixMove();					// fix movement for clientsie view angles
	void RunRecoilFix();				// fix angles to compensate for punchangle
	void RunBhop();						// jump if we're on the floor
	void RunAA();						// nasty angles for other aimbots
	void RunFakeLag();					// simulated lag
	void RunAutoStrafe();				// automatic running
	void RunFakeCrouch();				// fake crouching

	// Other public methods
	bool ShouldSetViewAngles(); 
};