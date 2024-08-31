/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "symaimbot.h"
#include "symcmd.h"

#include "global.h"

#include "src_interface.h"	// SRC_STEAM_FRIENDS
#include "src_lib.h"		// lots
#include "src_random.h"		// DoSpreadCompensation

#include "checksum_md5.h"

#include "patterns.h"

MakeConVar4(short, aim_max_health, "(Filter) max health for a target. 0 for no max", 0);
MakeConVar4(short, aim_min_health, "(Filter) min health for a target. 0 for no min", 0);
MakeConVar4(short, aim_max_dist, "(Filter) max distance for a target. 0 for no max", 0);
MakeConVar4(short, aim_friends, "(Filter) enable or disable targeting steam friends. 0 to target", 0);
MakeConVar4(short, aim_sameteam, "(Filter) enable or disable targeting same-team players. 1 to target", 1);
MakeConVar4(float, aim_fov, "(Filter) max angular difference between your current view angles and the viewangles required to shoot a target.  0 to disable", 0);
MakeConVar4(short, aim_priority, "(Priority) prioritize players close to the cursor.  0 to base off 3d distance", 0);
MakeConVar4(short, aim_notrace, "disable traces.  0 to enable traces.  traces are used to check if player's are visible", 0);
MakeConVar4(short, aim_hitbox, "target hitbox ID, 0 is head", 0);
MakeConVar4(int, aim_triggerbot_max_distance, "(Filter) sets the maximum distance that the triggerbot will work from", 640000);

MakeConVar4(short, aim_nospread, "enable spread counteracting measures. 0 to disable", 1);
MakeConVar4(short, aim_silentaim, "silent aim.  0 = off, 1 = clientside, 2 = clientside and serverside (may decrease performance).", 0);
MakeConVar4(short, aim_spreadmode, "no-spread type. 0 = spread caching with prediction, 1 = seed forcing", 0);
MakeConVar4(int, aim_key, "key used to activate aimbot, for speed use virtual key codes: http://goo.gl/3bwrTe", VK_LMENU);
MakeConVar4(short, aim_keymode, "aim_key behaviour.  0 = when aim_key is pressed aimbot will activate, 1 = when aim_key is pressed aimbot will deactivate (aimbot is active while key is NOT held)", 0);
MakeConVar4(short, aim_prediction, "enable prediction, required for lots of stuff and it's best to not turn this off.  0 to disable", 1);
MakeConVar4(short, aim_locking, "locks onto a target so that it will only change when the target becomes invalid or your release the aimkey.  0 to disable", 0);
MakeConVar4(float, aim_smooth, "avoids snapping towards your target.  0 to disable, anything besides zero is interpolation fraction (e.g 0.5)", 0.0f);
MakeConVar4(float, aim_smooth_tolerance, "amount of total degree difference before angles snap completely.  0 to disable tolerance snapping", 5.0f);
MakeConVar4(short, aim_autofire, "enable automatic firing, whilst enabled the aimbot will fire your weapon when targets are visible. 0 to disable", 0);
MakeConVar4(short, aim_minalpha, "the lowest amount of alpha an entity can have.  this is useful for special spectating addons that make players invisible, keep on 255 for those cases.  0 to disable", 255);
MakeConVar4(short, aim_aaa, "anti-anti-aim.  1 = naive mode, expects that AA is done poorly, 2 = generic mode, tries to at least hit an enemy that's AA'ing, 0 = disabled.", 0);
MakeConVar4(short, aim_triggerbot, "enable triggerbot.  1 = fire when hovering over a target.  2 = fire & activate aimbot when hovering over a target.  3 = activate aimot only.  0 = disabled", 0);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SymAimbot::SymAimbot()
///
/// \brief	Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

SymAimbot::SymAimbot()
{
	this->m_pMe = NULL;
	this->m_pWeapon = NULL;

	this->m_vFrom = Vector(0, 0, 0);
	this->m_vTo = Vector(0, 0, 0);

	this->m_pCurrentTarget = NULL;
	this->m_pLockedTarget = NULL;

	this->m_bInSmoothing = false;

	this->m_hSteamPipe		= Src::SteamClient->CreateSteamPipe();
	this->m_hSteamUser		= Src::SteamClient->ConnectToGlobalUser(this->m_hSteamPipe);
	this->m_pStreamFriends	= Src::SteamClient->GetISteamFriends(this->m_hSteamUser, this->m_hSteamPipe, SRC_STEAM_FRIENDS);

	if (!this->m_pStreamFriends)
	{
		PrintBadL("Warning, " SRC_STEAM_FRIENDS " is invalid");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SymAimbot::~SymAimbot()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

SymAimbot::~SymAimbot()
{
	if (!this->m_hSteamPipe)
		return;

	if (!Src::SteamClient->BReleaseSteamPipe(this->m_hSteamPipe))
	{
		PrintBadL("Couldn't close steam pipe");
	}

	Src::SteamClient->ReleaseUser(this->m_hSteamPipe, this->m_hSteamUser);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymAimbot::GetTargetHitbox(SrcEntity* pEnt, Vector* vpOut)
///
/// \brief	Gets target hitbox position.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymAimbot::GetTargetHitbox(SrcEntity* pEnt, Vector* vpOut)
{
	if (!pEnt)
		return false;

	if (aim_aaa.GetValue() == 2)
	{
		float x = *MakePtr(float*, pEnt, 0x2F64);

		// Get the middle of the player's eyes and their position (bottommost point)
		if (x >= 90.0f || x <= -90.0f)
		{
			Vector mins, maxs;
			pEnt->GetRenderBounds(mins, maxs);

			*vpOut = pEnt->GetAbsOrigin() + ((mins + maxs) * 0.5);
			return true;
		}
	}

	matrix3x4_t Matrix[MAXSTUDIOBONES];

	if (!pEnt->SetupBones(Matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 
		SrcLib::GetPlayerTickBase(this->m_pMe) * SrcLib::GetGlobalVars()->interval_per_tick))
		return false;

	studiohdr_t *pStudioHeader = Src::ModelInfo->GetStudiomodel(pEnt->GetModel());

	if (!pStudioHeader)
		return false;

	mstudiobbox_t *pBox;

	pBox = pStudioHeader->pHitbox(min(aim_hitbox.GetValue(), pStudioHeader->iHitboxCount(0)), 0);

	Vector vMax, vMin;
	VectorTransform(pBox->bbmin, Matrix[pBox->bone], vMin);
	VectorTransform(pBox->bbmax, Matrix[pBox->bone], vMax);

	*vpOut = ((vMin + vMax) * 0.5);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymAimbot::DoSpreadCompensation(CUserCmd* pCmd, QAngle& qOut)
///
/// \brief	Gets spread compensation.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymAimbot::DoSpreadCompensation(CUserCmd* pCmd, QAngle& qOut)
{
	if (aim_nospread.GetValue())
	{
		if (aim_spreadmode.GetValue())
		{
			// 142 reduces spread to almost nothing (ALMOST!!)
			pCmd->command_number = 142;
			pCmd->random_seed = MD5_PseudoRandom(142) & 0x7FFFFFFF;
		}
		else
		{
			// Counteract spread cached in FireBullets from
			// (ItemPostFrame -> PrimaryAttack -> FireBullets)
			if (Var::LastBullet)
			{
				Vector forward, dir;
				AngleVectors(qOut, &forward);

				uint32 seed = (MD5_PseudoRandom(pCmd->command_number) & 0x7fffffff);

				// Get the spread for the bullet we're about to fire
				dir = SrcRandom::CalculateSpread(seed & 0xFF,
					-Var::LastBullet->m_vecSpread, forward);

				VectorAngles(dir, qOut);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymAimbot::FindTarget(CUserCmd* pCmd)
///
/// \brief	Searches for a suitable target.
////////////////////////////////////////////////////////////////////////////////////////////////////

SrcEntity* SymAimbot::FindTarget(CUserCmd* pCmd)
{
	SrcEntity* target = NULL;
	this->m_vFrom = this->m_pMe->EyePosition();

    if (this->m_pAimTarget && aim_triggerbot.GetValue() == 2)
    {
        Vector to = this->m_pAimTarget->GetAbsOrigin();

        if (SrcLib::GetLifeState(this->m_pAimTarget) == LIFE_ALIVE &&
            this->IsValidTarget(this->m_pAimTarget) &&
            this->GetTargetHitbox(this->m_pAimTarget, &to))
        {
            this->m_vTo = to;
            return this->m_pAimTarget;
        }
    }

	for (int i = 0; i < Src::CEntList->GetHighestEntityIndex(); i++)
	{
		if (i == Src::Engine->GetLocalPlayer())
			continue;

		SrcEntity* ply = IdxToEnt(i);

		if (!ply)
			continue;

		// Avoid expensive betterness checks
		if (!IsPlayerIndex(i))
			continue;

		// Check if the player is alive
		if (SrcLib::GetLifeState(ply) != LIFE_ALIVE)
			continue;

		// Don't bother checking anything if this isn't even a better
		// targe than the one we have
		if (!this->IsTargetPrefered(ply, target))
			continue;

		// Check validity (less expensive than ray-trace)
		if (!this->IsValidTarget(ply))
			continue;

		Vector to = ply->GetAbsOrigin();

		// We need to have the hitbox we want to shoot
		if (!this->GetTargetHitbox(ply, &to))
			continue;

        if (aim_fov.GetValue())
        {
            QAngle targetAng;
            VectorAngles(to - this->m_vFrom, targetAng);

            float absdiff = abs(AngleDiff(pCmd->viewangles.x, targetAng.x)) + abs(AngleDiff(pCmd->viewangles.y, targetAng.y));

            if (absdiff > aim_fov.GetValue())
                continue;
        }

		if (!aim_notrace.GetValue() && !SrcLib::IsPointEntityVisible(this->m_vFrom, to, ply))
			continue;

		target = ply;
		this->m_vTo = to;
	}

	return target;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymAimbot::AimAtTarget(CUserCmd* pCmd)
///
/// \brief	Aims at our current target.
///
/// \param [in,out]	pCmd	If non-null, the command.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymAimbot::AimAtTarget(CUserCmd* pCmd)
{
	// Angle to point at target
	QAngle targetAng;
	VectorAngles(this->m_vTo - this->m_vFrom, targetAng);

	if (aim_smooth.GetValue() > 0.0f && !aim_silentaim.GetValue())
	{
		QAngle viewangles;
		QAngle smoothAngle;
		Src::Engine->GetViewAngles(viewangles);

		// Convert
		Quaternion src, dest, result;
		AngleQuaternion(viewangles, src);
		AngleQuaternion(targetAng, dest);

		QuaternionSlerp(src, dest, aim_smooth.GetValue(), result);

		// Convert to euler
		QuaternionAngles(result, smoothAngle);

		if (aim_smooth_tolerance.GetValue() > 0.0f)
		{
			float absdiff = abs(AngleDiff(smoothAngle.x, targetAng.x)) + abs(AngleDiff(smoothAngle.y, targetAng.y));

			if (absdiff > aim_smooth_tolerance.GetValue())
			{
				targetAng = smoothAngle;
				this->m_bInSmoothing = true;
			}
		}
		else
		{
			this->m_bInSmoothing = true;
			targetAng = smoothAngle;
		}
	}

	// We shouldn't have roll (p,y,r => x,y,z)
	targetAng.z = 0;

	// Set viewangles (not command angles) now, before nospread
	if (!aim_silentaim.GetValue())
		Src::Engine->SetViewAngles(targetAng);

	// Perform spread compensation (follows cvar rules)
	this->DoSpreadCompensation(pCmd, targetAng);

	// Time to shoot! Here because it doesn't like to coop with psilent afterwards
	if (aim_autofire.GetValue() && !this->m_bInSmoothing)
		this->FireWeapon(pCmd);

	// Change viewangles after we've compensated spread
	if (aim_silentaim.GetValue() >= 2)
	{
		// Don't modify viewangles
		if (m_cmdModifier->m_bFireCommand && (m_cmdModifier->m_pCmd->buttons & IN_ATTACK))
			// Queue this packet for next tick (and make next tick now)
			this->m_cmdModifier->BlockPacket(true);
		else
			// If we can't shoot we wont aim
			return;
	}

	pCmd->viewangles = targetAng;
	SrcLib::Normalize(pCmd->viewangles);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymAimbot::IsValidTarget(SrcEntity* pEnt)
///
/// \brief	Query if 'pEnt' is valid target.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymAimbot::IsValidTarget(SrcEntity* pEnt)
{
	// Silly checks but we'll do them anyway.
	// NOTE: no player is valid if we're not
	if (!pEnt)
		return false;

	// Not in PVS
	if (pEnt->IsDormant())
		return false;

	int health = pEnt->GetHealth();

	// Health checks
	if (aim_max_health.GetValue() && health > aim_max_health.GetValue()) return false;
	if (aim_min_health.GetValue() && health < aim_min_health.GetValue()) return false;

	// Distance (avoid doing sqrt'd distance without the setting being enabled)
	if (aim_max_dist.GetValue())
	{
		float dist = pEnt->GetAbsOrigin().DistTo(this->m_pMe->GetAbsOrigin());
		if (dist > aim_max_dist.GetValue())
			return false;
	}

	// Is the target's opacity not suitable? check here
	if (aim_minalpha.GetValue() && pEnt->GetRenderColor().a < aim_minalpha.GetValue())
		return false;

	// Check if they're our friend on steam (very slow before caching, avoid without setting!)
	// Bots can't be friends either, don't bother checking them
	if (!aim_friends.GetValue())
	{
		CSteamID steamID;
		SrcLib::GetSteamID(pEnt, &steamID);

		if (this->m_pStreamFriends->HasFriend(steamID, k_EFriendFlagImmediate))
			return false;
	}

	// Team check
	if (!aim_sameteam.GetValue() && this->m_pMe->GetTeamNumber() == pEnt->GetTeamNumber())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymAimbot::IsTargetPrefered(SrcEntity* pFirst, SrcEntity* pSecond)
///
/// \brief	Query if 'pFirst' is target prefered.
///
/// \param [in,out]	pFirst 	First target.
/// \param [in,out]	pSecond	Second target.
///
/// \return	true if target prefered, false if not.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymAimbot::IsTargetPrefered(SrcEntity* pFirst, SrcEntity* pSecond)
{
	// Anything is better than nothing
	if (!pSecond)
		return true;

	// Anything is still better than nothing
	if (!pFirst)
		return false;

	// If we've enabled target locking, then the locked target is ALWAYS better
	// than another target (we don't want another target)
	if (aim_locking.GetValue() && this->m_pLockedTarget)
	{
		SrcEntity* locked = this->m_pLockedTarget;

		if (locked == pFirst)
			return true;

		if (locked == pSecond)
			return false;
	}

	if (aim_priority.GetValue())
	{
		// Calculate both the new screen positions for this target
		// and our current one
		Vector vnew, vold;

		if (!SrcLib::WorldToScreen(pSecond->GetAbsOrigin(), vold))
			return true;

		if (!SrcLib::WorldToScreen(pFirst->GetAbsOrigin(), vnew))
			return false;

		// Middle of the screen (crosshair)
		Vector2D screen(Var::ScreenW / 2, Var::ScreenH / 2);

		Vector2D a(vnew.x, vnew.y);
		Vector2D b(vold.x, vold.y);

		return a.DistToSqr(screen) < b.DistToSqr(screen);
	}
	else
	{
		// Worldpos
		return pFirst->GetAbsOrigin().DistToSqr(this->m_pMe->GetAbsOrigin()) <
			pSecond->GetAbsOrigin().DistToSqr(this->m_pMe->GetAbsOrigin());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymAimbot::Setup(CUserCmd* pCmd)
///
/// \brief	Sets up an aimbot cycle.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymAimbot::Setup(CUserCmd* pCmd, SymCmd* pSymCmd)
{
    // Reset these
    this->m_bInSmoothing    = false;
    this->m_pAimTarget      = NULL;
    this->m_pCurrentTarget  = NULL;
	this->m_pMe				= IdxToEnt(Src::Engine->GetLocalPlayer());
    this->m_cmdModifier     = pSymCmd;

    if (!this->m_pMe)
        return false;

    this->m_pWeapon = SrcLib::GetActiveWeapon(this->m_pMe);

    if (!this->m_pWeapon)
        return false;

    // Is the aimbot activated?
    bool aim = true;

    if (!GetAsyncKeyState(aim_key.GetValue()))
    {
        if (!aim_keymode.GetValue())
        {
            aim = false;
        }

    }
    else if (aim_keymode.GetValue())
    {
        aim = false;
    }

    if (!aim && aim_triggerbot.GetValue() > 0)
    {
        Ray_t	ray;
        trace_t	results;

        Vector forward;
        AngleVectors(pCmd->viewangles, &forward);
        forward *= aim_triggerbot_max_distance.GetValue();

        // Initialize the ray with start and end positions
        ray.Init(this->m_pMe->EyePosition(), this->m_pMe->EyePosition() + forward);

        // Perform the ray-trace
        Src::Trace->TraceRay(ray, MASK_SHOT, NULL, &results);

        if (results.m_pEnt->IsPlayer() && results.m_pEnt != (C_BaseEntity*)this->m_pMe)
        {
            this->m_pAimTarget = (SrcEntity*)results.m_pEnt;

            // We're pointing at a target, so we can fire
            if (aim_triggerbot.GetValue() == 1 || aim_triggerbot.GetValue() == 2)
                this->FireWeapon(pCmd);

            // Triggerbot is set to NOT enable the aimbot, fix that here
            if (aim_triggerbot.GetValue() == 1)
                return false;
        }
        else
        {
            // No aimbot and no triggerbot target, abort
            return false;
        }
    }
    // No aimbot and no triggerbot, abort
    else if (!aim)
        return false;

	if (aim_prediction.GetValue())
	{
		Var::NextBulletSimulated = true;
		((CPrediction*)Src::Prediction)->RunCommand((C_BasePlayer*)this->m_pMe, pCmd, NULL);
		Var::NextBulletSimulated = false;
	}

	this->m_pCurrentTarget = this->FindTarget(pCmd);

	if (!this->m_pCurrentTarget)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymAimbot::Perform(CUserCmd* pCmd)
///
/// \brief	Performs aimbot cycle with calculated target.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymAimbot::Perform(CUserCmd* pCmd)
{
	if (this->m_pCurrentTarget)
	{
		this->AimAtTarget(pCmd);

		return true;
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymAimbot::FireWeapon(CUserCmd* pCmd)
///
/// \brief	Marks this packet 
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymAimbot::FireWeapon(CUserCmd* pCmd)
{
	float curtime = 0.0f;
	if (aim_prediction.GetValue())
	{
		// CurTime was already predicted
		curtime = SrcLib::CurTime();
	}
	else
	{
		// CurTime needs predicting
		curtime = SrcLib::GetPlayerTickBase(this->m_pMe) * SrcLib::GetGlobalVars()->interval_per_tick;
	}

	float primary = SrcLib::GetNextPrimaryFire(this->m_pWeapon);

	if (curtime >= primary)
	{
		pCmd->buttons |= IN_ATTACK;
	}
}