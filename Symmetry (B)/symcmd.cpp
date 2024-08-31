/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "symcmd.h"

#include "global.h"

#include "src_lib.h"

MakeConVar4(short, move_speed, "movement speed multiplier, default 1", 1);
MakeConVar4(short, move_fixmove, "fixes your movement direction when using silent aim and AA.  0 to disable", 1);
MakeConVar4(short, move_bhop, "enable bunny-hop (hold space to jump).  0 to disable", 1);
MakeConVar4(short, move_fakelag, "enables fake-lag, making you harder to hit.  0 to disable", 0);
MakeConVar4(short, move_lagsize, "the size of the lag packet-queue, bigger = more lag.  disable fake lag with move_fakelag", 15);
MakeConVar4(short, move_aa, "enable anti-aim. 1 = x only, 2 = x and y, 0 to disable", 0);
MakeConVar4(short, move_autostrafe, "strafe/sprint automatically without the need to press anything. 1 = enabled, 2 = m9k mode, 0 = disabled", 0);

//MakeConVar4(short, move_fakecrouch, "enable fake-crouch.  0 to disable", 0);

// source declaration
int SymCmd::m_iMoveSpeed = -1;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::CallAgain()
///
/// \brief	Calls the usercommand chain again, we'll end up being infinitely recursive if 
/// 		measures are not taken!
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::CallAgain()
{
	// Only CallAgain once
	if (!this->m_bCallAgain)
	{
		this->m_bCallAgain = true;
		*this->m_dwReturnAddress -= 5;	// CALL 0xDEADBEEF
										// 1b	4b
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::BlockPacket(bool bBlock)
///
/// \brief	Sets a boolean denoting whether or not the packet being built here will be
/// 		sent to the server.  If this is blocked it will be queued (for a limited time).
///
/// \param	bBlock	true to block.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::BlockPacket(bool bBlock)
{
	*this->m_bpSendPacket = !bBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymCmd::IsBlocked()
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymCmd::IsBlocked()
{
	return *this->m_bpSendPacket;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RestoreAngles()
///
/// \brief	Restore angles from backup data.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RestoreAngles()
{
	memcpy(&this->m_pCmd->viewangles, &this->m_cmdOriginal.viewangles, sizeof(QAngle));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RestoreMove()
///
/// \brief	Restore movement direction from backup data.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RestoreMove()
{
	// forwardmove, sidemove and upmove are in order
	// float x, float y, float z
	memcpy(&this->m_pCmd->forwardmove, &this->m_cmdOriginal.forwardmove, sizeof(float) * 3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RestoreButtons()
///
/// \brief	Restore buttons from backup data.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RestoreButtons()
{
	this->m_pCmd->buttons = this->m_cmdOriginal.buttons;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SymCmd::SymCmd(CUserCmd* pCmd, bool* bpSendPacket, dword* dwReturnAddress)
///
/// \brief	Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

SymCmd::SymCmd(CUserCmd* pCmd, bool* bpSendPacket, dword* dwReturnAddress)
{
	this->m_pCmd			= pCmd;
	this->m_bpSendPacket	= bpSendPacket;
	this->m_dwReturnAddress = dwReturnAddress;

	this->m_bCallAgain		= false;
	this->m_bHasAimTarget	= true;

	// Store backup
	memcpy(&this->m_cmdOriginal, this->m_pCmd, sizeof(CUserCmd));

	// This can be modified by bullet prediction in aimbot, so we must 
	// know record these before it's too late
	SrcEntity* ply = SrcLib::GetLocalPlayer();

	this->m_bFireCommand = false;

	if (ply)
	{
		C_BaseCombatWeapon* weapon = SrcLib::GetActiveWeapon(ply);

		if (weapon)
		{
			float time = SrcLib::GetPlayerTickBase(ply) * SrcLib::GetGlobalVars()->interval_per_tick;

			if (time >= SrcLib::GetNextPrimaryFire(weapon) && SrcLib::HasAmmo(weapon))
				this->m_bFireCommand = true;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SymCmd::~SymCmd()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

SymCmd::~SymCmd()
{
	// nothing here yet
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunSpeedhack()
///
/// \brief	Executes the speedhack operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunSpeedhack()
{
	if (m_iMoveSpeed == -1)
		m_iMoveSpeed = move_speed.GetValue();

	if (m_iMoveSpeed > 1)
	{
		m_iMoveSpeed--;

		this->CallAgain();
		this->BlockPacket(true);
	}
	else
	{
		m_iMoveSpeed = move_speed.GetValue();
		this->BlockPacket(false);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunAimbot()
///
/// \brief	Executes the aimbot operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunAimbot()
{
	if (Game::Aimbot)
	{
		if (Game::Aimbot->Setup(this->m_pCmd, this))
		{
			this->m_bHasAimTarget = true;
			Game::Aimbot->Perform(this->m_pCmd);
		}
		else
			this->m_bHasAimTarget = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunFixMove()
///
/// \brief	Executes the fix-move operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunFixMove()
{
	if (!move_fixmove.GetValue())
		return;

	QAngle va; Src::Engine->GetViewAngles(va);

	Vector& move = *(Vector*)&this->m_pCmd->forwardmove;
	float yaw, speed;

	QAngle ang = this->m_pCmd->viewangles;

	// Fix movement angles only when they differ from our engine viewangles
	if (ang.y != va.y || ang.x != va.x)
	{
		speed = sqrt((move.x * move.x) + (move.y * move.y));

		yaw = RAD2DEG(atan2(move.y, move.x));
		yaw = DEG2RAD(ang.y - va.y + yaw);

		this->m_pCmd->forwardmove	= cos(yaw) * speed;			
		this->m_pCmd->sidemove		= sin(yaw) * speed;

		float normal = SrcLib::Normalize(ang.x);

		// We'd be going in the opposite direction on the server
		if (normal <= -90.0f || normal >= 90.0f)
			this->m_pCmd->forwardmove = -this->m_pCmd->forwardmove;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunRecoilFix()
///
/// \brief	Executes the recoil fix operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunRecoilFix()
{
	if (this->m_bHasAimTarget)
	{
		SrcEntity* ply = SrcLib::GetLocalPlayer();

		if (ply)
		{
			C_BaseCombatWeapon* weapon = SrcLib::GetActiveWeapon(ply);

			// Only works wiht HL2, have no nice way of telling if a weapon
			// is HL2 or not
			/*
			if (weapon && weapon->IsBaseCombatWeapon())
			{
				this->m_pCmd->viewangles -= (SrcLib::GetPunchAngle(SrcLib::GetLocalPlayer()));
			}*/
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunBhop()
///
/// \brief	Executes the bunny-hop operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunBhop()
{
	if (!move_bhop.GetValue())
		return;

	if ((this->m_pCmd->buttons & IN_JUMP) == IN_JUMP)
	{
		SrcEntity* ply = SrcLib::GetLocalPlayer();

		if (ply && SrcLib::GetMoveType(ply) == MOVETYPE_WALK)
		{
			if (!SrcLib::IsOnGround(ply))
			{
				this->m_pCmd->buttons &= ~IN_JUMP;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunAA()
///
/// \brief	Executes anti-aim operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunAA()
{
	if (move_aa.GetValue() && SrcLib::GetMoveType(SrcLib::GetLocalPlayer()) == MOVETYPE_WALK)
	{
		if ((this->m_pCmd->buttons & IN_ATTACK) == IN_ATTACK)
		{
			this->m_pCmd->viewangles.x = (180 - this->m_pCmd->viewangles.x);
			this->m_pCmd->viewangles.y = -(180 - this->m_pCmd->viewangles.y);
		}
		else
		{
			this->m_pCmd->viewangles.x = -181;

			if (move_aa.GetValue() >= 2)
				this->m_pCmd->viewangles.y = Src::Random->RandomFloat(-180.0f, 180.0f);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunFakeLag()
///
/// \brief	Executes the fake-lag operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunFakeLag()
{
	if (!move_fakelag.GetValue())
		return;

	if (m_iMoveSpeed <= 1)
	{
		static int counter = 0;

		if (this->m_bFireCommand && this->m_pCmd->buttons & IN_ATTACK)
		{
			this->BlockPacket(false);
			counter = move_lagsize.GetValue();
		}
		else
		{
			if (!counter)
			{
				// Send out packets
				this->BlockPacket(false);

				// Set counter to queue another x packets
				counter = move_lagsize.GetValue();
			}
			else
			{
				// Block and decrease
				counter--;
				this->BlockPacket(true);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunAutoStrafe()
///
/// \brief	Executes the automatic-strafe operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunAutoStrafe()
{
	if (move_autostrafe.GetValue())
	{
		if (move_autostrafe.GetValue() == 2 && this->m_pCmd->buttons & IN_ATTACK)
		{
			this->m_pCmd->buttons &= ~IN_SPEED;
		}
		else 
			this->m_pCmd->buttons |= IN_SPEED;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymCmd::RunFakeCrouch()
///
/// \brief	Executes the fake-crouch operation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymCmd::RunFakeCrouch()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymCmd::ShouldSetViewAngles()
///
/// \brief	Determine if we should set view angles, if we're anti-aiming, targeting a player,
/// 		this will return false.
///
/// \return	See above.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymCmd::ShouldSetViewAngles()
{
	return !this->m_bHasAimTarget;
}