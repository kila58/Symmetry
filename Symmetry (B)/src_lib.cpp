/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "src_lib.h"
#include "global.h"

#include "patterns.h"

#include "lua_gmod.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcLib::GetSteamID(SrcEntity* bPly, CSteamID* cSteamID)
///
/// \brief	Gets the steam ID for a player.
///
/// \param [in,out]	bPly		The player.
/// \param [in,out]	cSteamID	Steam ID place-holder.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcLib::GetSteamID(SrcEntity* bPly, CSteamID* cSteamID)
{
	return Patterns::SrcEntity__GetSteamID(bPly, cSteamID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	C_BaseCombatWeapon* SrcLib::GetActiveWeapon(SrcEntity* bPly)
///
/// \brief	Gets a player's active weapon,.
///
/// \param [in,out]	bPly	The player.
///
/// \return	The player's active weapon, NULL if the player doesn't have one.
////////////////////////////////////////////////////////////////////////////////////////////////////

C_BaseCombatWeapon* SrcLib::GetActiveWeapon(SrcEntity* bPly)
{
    return
        (C_BaseCombatWeapon*)Src::CEntList->GetClientEntityFromHandle(*MakePtr(CHandle<C_BaseCombatWeapon>*, bPly, 0x21E0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SrcEntity* SrcLib::GetLocalPlayer()
///
/// \brief	Uses an offset from _G.LocalPlayer to call SrcEntity::GetLocalPlayer (static function!)
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

SrcEntity* SrcLib::GetLocalPlayer()
{
	return IdxToEnt(Src::Engine->GetLocalPlayer());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcLib::SetPredictionRandomSeed(CUserCmd* pCmd)
///
/// \brief	Sets prediction random seed.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcLib::SetPredictionRandomSeed(CUserCmd* pCmd)
{
	Patterns::SrcEntity__SetPredictionRandomSeed(pCmd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	static void SrcLib::ItemPostFrame(C_BaseCombatWeapon* bWep)
///
/// \brief	Calls a weapon's ItemPostFrame function.
///
/// \param [in,out]	bWep	The weapon.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcLib::ItemPostFrame(C_BaseCombatWeapon* bWep)
{
	// Typdef for C_BaseCombatWeapon::ItemPostFrame
	typedef void(__thiscall * fn)(C_BaseCombatWeapon* vmt);

	// VIndex for C_BaseCombatWeapon::ItemPostFrame is 293 (SRCWEP_ITEMFRAME_FUNC)
	return ((fn)(*(PDWORD_PTR*)bWep)[SRCWEP_ITEMFRAME_FUNC])(bWep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float SrcLib::GetNextPrimaryFire(C_BaseCombatWeapon* bWep)
///
/// \brief	Gets the time (relative to CurTime) that a weapon will next be able to fire.
///
/// \param [in,out]	bWep	The weapon.
///
/// \return	The next primary fire time.
////////////////////////////////////////////////////////////////////////////////////////////////////

float SrcLib::GetNextPrimaryFire(C_BaseCombatWeapon* bWep)
{
	// +4 bytes on 2/16/2016
    return *MakePtr(float*, bWep, 0x17A4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SrcLib::HasAmmo(C_BaseCombatWeapon* bWep)
///
/// \brief	Gets the current ammo count and checks if > 0.
///
/// \param [in,out]	bWep	The weapon.
///
/// \return	True if you have ammo, else false.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SrcLib::HasAmmo(C_BaseCombatWeapon* bWep)
{
	int ammo = *MakePtr(int*, bWep, 0x17E8);
	return (ammo > 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	INetChannelInfo* SrcLib::GetNetChannelInfo()
///
/// \brief	Gets the INetChannelInfo
///
/// \return	null if it fails, else the net channel information.
////////////////////////////////////////////////////////////////////////////////////////////////////

INetChannelInfo* SrcLib::GetNetChannelInfo()
{
	// We need this
	if (!Src::Engine)
		return NULL;

	// Typdef for Global::SrcEngine::GetNetChannelInfo();
	typedef INetChannelInfo* (__thiscall * fn)(void* vmt);

	// VIndex for Global::SrcEngine::GetNetChannelInfo is 72 (SRCENG_GETNETCHAN_FUNC)
	return ((fn)(*(PDWORD_PTR*)Src::Engine)[SRCENG_GETNETCHAN_FUNC])(Src::Engine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcLib::ProcessMovement(SrcEntity* bPly, CMoveData* pMoveData)
///
/// \brief	Processes movement.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcLib::ProcessMovement(SrcEntity* bPly, CMoveData* pMoveData)
{
	typedef void (__thiscall * fn)(void* vmt, SrcEntity*, CMoveData*);
	return ((fn)(*(PDWORD_PTR*)Src::GameMovement)[SRCMOV_PROCESSMOVEMENT])(Src::GameMovement, bPly, pMoveData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcLib::SetPlayerCurCommand(SrcEntity*, CUserCmd*)
///
/// \brief	Sets a player's current command.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcLib::SetPlayerCurCommand(SrcEntity* bPly, CUserCmd* cmd)
{
	*MakePtr(dword*, bPly, SRCPLY_CURCOMMAND) = (dword)cmd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int SrcLib::GetPlayerTickBase(SrcEntity* bPly)
///
/// \brief	Gets a player's tick base.
///
/// \param [in,out]	bPly	The target player.
///
/// \return	The player's tick base.
////////////////////////////////////////////////////////////////////////////////////////////////////

float SrcLib::GetPlayerTickBase(SrcEntity* bPly)
{
    //return *MakePtr(float*, bPly, 0x2558);
	return (float)*(int*)((DWORD)bPly + 0x2558);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	wchar_t* SrcLib::GetPlayerName(SrcEntity* bPly)
///
/// \brief	Gets a player's name.
///
/// \param [in,out]	bPly	The target player.
///
/// \return	The player's name.
////////////////////////////////////////////////////////////////////////////////////////////////////

char* SrcLib::GetPlayerName(SrcEntity* bPly)
{
	return Patterns::SrcEntity__GetPlayerName(bPly);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	static char* GetClassname(SrcEntity* bEnt)
///
/// \brief	Gets the classname of an entity.
///
/// \param [in,out]	bEnt	The target entity.
///
/// \return	The entity's classname
////////////////////////////////////////////////////////////////////////////////////////////////////

char* SrcLib::GetClassname(SrcEntity* bEnt)
{
	return Patterns::SrcEntity__GetClassname(bEnt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int SrcLib::GetLifeState(SrcEntity* bPly)
///
/// \brief	Gets life state.
///
/// \param [in,out]	bPly	The target player.
///
/// \return	The life state.
////////////////////////////////////////////////////////////////////////////////////////////////////

char SrcLib::GetLifeState(SrcEntity* bPly)
{
	return *MakePtr(char*, bPly, SRCPLY_LIFESTATE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SrcLib::IsOnGround(SrcEntity* bEnt)
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SrcLib::IsOnGround(SrcEntity* bEnt)
{
	return *MakePtr(dword**, bEnt, SRCENT_GROUNDENTITY) != (dword*)0xFFFFFFFF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcLib::GetVectors(SrcEntity* bEnt, Vector* vForward, Vector* vRight, Vector* vUp)
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcLib::GetVectors(SrcEntity* bEnt, Vector* vForward, Vector* vRight, Vector* vUp)
{
	return Patterns::SrcEntity__GetVectors(bEnt, vForward, vRight, vUp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SrcEntity* SrcLib::GetOwnerEntity(SrcEntity* bEnt)
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SrcLib::HasOwnerEntity(SrcEntity* bEnt)
{
	return *MakePtr(dword**, bEnt, SRCENT_OWNERENTITY) != (dword*)0xFFFFFFFF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	MoveType_t SrcLib::GetMoveType(SrcEntity* bPly)
////////////////////////////////////////////////////////////////////////////////////////////////////

MoveType_t SrcLib::GetMoveType(SrcEntity* bPly)
{
	return (MoveType_t)*MakePtr(uchar*, bPly, 0x178);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int SrcLib::GetWaterLevel(SrcEntity* bPly)
////////////////////////////////////////////////////////////////////////////////////////////////////

int SrcLib::GetWaterLevel(SrcEntity* bPly)
{
	return *MakePtr(uchar*, bPly, 0x17C);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CHandle<SrcEntity> GetObserverTarget(SrcEntity* bPly)
////////////////////////////////////////////////////////////////////////////////////////////////////

SrcEntity* SrcLib::GetObserverTarget(SrcEntity* bPly)
{
	return 
        (SrcEntity*)Src::CEntList->GetClientEntityFromHandle(*MakePtr(CHandle<SrcEntity>*, bPly, 0x24AC));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	QAngle SrcLib::GetPunchAngle(SrcEntity* bPly)
////////////////////////////////////////////////////////////////////////////////////////////////////

QAngle SrcLib::GetPunchAngle(SrcEntity* bPly)
{
	return *MakePtr(QAngle*, bPly, 0x2638);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SrcLib::IsTakingScreenshot()
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SrcLib::IsTakingScreenshot()
{
	if (!Src::Engine)
		return NULL;

	typedef bool (__thiscall * fn)(void* vmt);

	return ((fn)(*(PDWORD_PTR*)Src::Engine)[SRCENG_TAKINSCRNS_FUNC])(Src::Engine);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SrcLib::IsPointEntityVisible(Vector& vStart, Vector& vEnd, SrcEntity* eTarget)
///
/// \brief	Check if vEnd on eTarget is visible from vStart.  This is primarily used by the 
/// 		aimbot to check if some part of a player is visible from where we're firing from.
/// 		
/// 		eTarget can be null, if it is this function will simply return true if there's
/// 		nothing between vStart and vEnd.
///
/// \param [in,out]	vStart 	Where to start the trace.
/// \param [in,out]	vEnd   	Where to end the trace.
/// \param [in,out]	eTarget	The target that the end of the trace must hit.  Can be null.
///
/// \return	.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SrcLib::IsPointEntityVisible(Vector vStart, Vector vEnd, SrcEntity* eTarget)
{
	// We can't trace without this being set first
	if (!Src::Trace)
		return false;

	Ray_t	ray;
	trace_t	results;

	// Initialize the ray with start and end positions
	ray.Init(vStart, vEnd);

	// Perform the ray-trace
	Src::Trace->TraceRay(ray, MASK_SHOT, NULL, &results);

	if (results.allsolid || results.startsolid)
		return false;

	if (results.m_pEnt == (C_BaseEntity*)eTarget && eTarget)
		return true;

	return results.fraction == 1.0f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcLib::Normalize(QAngle& qAng)
///
/// \brief	Normalizes an angle (keeping the angle within -180 and 180).
///
/// \param [in,out]	qAng	The angle.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SrcLib::Normalize(QAngle& qAng, float fLow, float fHigh)
{
	for (int i = 0; i < 3; i++)
	{
		if (qAng[i] > fHigh)
		{
			qAng[i] -= (fHigh * 2);
		}
		else if (qAng[i] < fLow)
		{
			qAng[i] += (fHigh * 2);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SrcLib::Normalize(QAngle& qAng)
///
/// \brief	Normalizes an angle (keeping the angle within -180 and 180).
///
/// \param [in,out]	qAng	The angle.
////////////////////////////////////////////////////////////////////////////////////////////////////

float SrcLib::Normalize(float fVal, float fLow, float fHigh)
{
	if (fVal > fHigh)
	{
		fVal -= (fHigh * 2);
	}
	else if (fVal < fLow)
	{
		fVal += (fHigh * 2);
	}

	return fVal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SrcLib::WorldToScreen(Vector& vPoint, Vector2D* vOut)
///
/// \brief	Translates a 3d vector to a 2d screen-vector.
///
/// \param [in,out]	vPoint	The point to translate.
/// \param [in,out]	vOut  	Pointer to 2d screen position.
///
/// \return	True if the 3d point is on the screen.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SrcLib::WorldToScreen(const Vector& vPoint, Vector& vScreen)
{
	const VMatrix& worldToScreen = Src::Engine->WorldToScreenMatrix();

	float w = worldToScreen[3][0] * vPoint[0] + worldToScreen[3][1] * vPoint[1] + worldToScreen[3][2] * vPoint[2] + worldToScreen[3][3];
	vScreen.z = 0;

	int width, height;
	Src::Engine->GetScreenSize(width, height);

	if (w > 0.001)
	{
		float fl1DBw = 1 / w;
		vScreen.x = (width / 2) + (0.5 * ((worldToScreen[0][0] * vPoint[0] + worldToScreen[0][1] * vPoint[1] + worldToScreen[0][2] 
			* vPoint[2] + worldToScreen[0][3]) * fl1DBw) * width + 0.5);
		vScreen.y = (height / 2) - (0.5 * ((worldToScreen[1][0] * vPoint[0] + worldToScreen[1][1] * vPoint[1] + worldToScreen[1][2] 
			* vPoint[2] + worldToScreen[1][3]) * fl1DBw) * height + 0.5);
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CGlobalVarsBase* SrcLib::GetGlobalVars()
///
/// \brief	Gets global variables using an offset from _G.CurTime.
///
/// \return	The CGlobalVarsBase singleton.
////////////////////////////////////////////////////////////////////////////////////////////////////

CGlobalVarsBase* SrcLib::GetGlobalVars()
{
	return **(CGlobalVarsBase***)MakePtr(dword*, Patterns::LuaCurTime, SRCVAR_OFFSET);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	uint32 SrcLib::GetSpectators(SrcEntity* bPly, int* ipSpectators, uint32 iBufSize)
///
/// \brief	Gets a list of spectators for a player.
///
/// \param [in,out]	bPly			The player to retrieve the list for.
/// \param [in,out]	ipSpectators	A pointer to an array that this function will populate with
/// 								spectator ID's.
/// \param	iBufSize				Size of the array.
///
/// \return	Number of spectators.
////////////////////////////////////////////////////////////////////////////////////////////////////

uint32 SrcLib::GetSpectators(SrcEntity* bPly, int* ipSpectators, uint32 iBufSize)
{
	uint32 idx = 0;
	for (int i = 0; i <= Src::CEntList->GetHighestEntityIndex(); i++)
	{
		SrcEntity* ply = IdxToEnt(i);

		if (!ply || !IsPlayerIndex(i))
			continue;

		if (bPly != ply && SrcLib::GetObserverTarget(ply) == bPly && idx < iBufSize)
		{
			ipSpectators[idx] = i;
			idx++;
		}
	}

	return idx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	float SrcLib::GetCurTime()
///
/// \brief	Current time from the gpGlobals pointer.  I'm not too sure whether or not this is
/// 		actually ->curtime, however gpGloblals + 0xC is used by CurTime.  The pointer to
/// 		gpGlobals is also found using an offset from the CurTime Lua function binding.
/// 		
/// 		EDIT: 30/10/2014: changed to use above function and ->curtime.
///
/// \return	The current time.
////////////////////////////////////////////////////////////////////////////////////////////////////

float SrcLib::CurTime()
{
	return SrcLib::GetGlobalVars()->curtime;
}