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
#include "generic.h"
#include "src_generic.h"

#include "globalvars_base.h"

#include "igamemovement.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
///									  == MACRO DEFINTIONS == 
////////////////////////////////////////////////////////////////////////////////////////////////////

// Virtual indices
#define SRCENG_GETNETCHAN_FUNC	72	// IVEngineClient::GetNetChannelInfo
#define SRCENG_TAKINSCRNS_FUNC	85	// IVEngineClient::IsTakingScreenshot
#define SRCENT_GETSTEAMID_FUNC	271 // SrcEntity::GetSteamID
#define SRCWEP_ITEMFRAME_FUNC	293 // C_BaseCombatWeapon::ItemPostFrame
#define SRCPLY_GETACTVWEP		269 // SrcEntity::GetActiveWeapon
#define	SRCMOV_PROCESSMOVEMENT  1	// CGameMovement::ProcessMovement

// Offsets
#define	SRCPLY_GETLOCALP		0x15  // _G.LocalPlayer + 0x15 = SrcEntity::GetLocalPlayer
#define SRCVAR_CURTIME			0xC	  // gpGlobals + 0xC = curtime
#define SRCVAR_OFFSET			0x1B  // _G.CurTime + 0x1B = gpGlobals
#define SRCWEP_NEXTPRIMARYATT	0x1B38// this + 0x1B38 = m_fNextPrimaryAttack
#define SRCPLY_CURCOMMAND		0x2820// this + 0x2820 = m_pCurrentCommand
#define SRCPLY_TICKBASE			0x28F0// this + 0x28F0 = m_nTickBase
#define SRCPLY_LIFESTATE		0x8F  // this + 0x8F = m_lifeState
#define SRCENT_GROUNDENTITY		0x254 // this + 0x254 = m_hGroundEntity
#define SRCENT_OWNERENTITY		0x4D8 // this + 0x4D8 = m_hOwnerEntity

// Other
enum InButtons
{
	IN_ATTACK = 1 << 0,
	IN_JUMP	= 1 << 1,
	IN_DUCK = 1 << 2,
	IN_FORWARD = 1 << 3,
	IN_BACK	= 1 << 4,
	IN_USE =1 << 5,
	IN_CANCEL = 1 << 6,
	IN_LEFT	= 1 << 7,
	IN_RIGHT = 1 << 8,
	IN_MOVELEFT	= 1 << 9,
	IN_MOVERIGHT = 1 << 10,
	IN_ATTACK2 = 1 << 11,
	IN_RUN = 1 << 12,
	IN_RELOAD = 1 << 13,
	IN_ALT1	= 1 << 14,
	IN_ALT2 = 1 << 15,
	IN_SCORE = 1 << 16,
	IN_SPEED = 1 << 17,
	IN_WALK	= 1 << 18,
	IN_ZOOM	= 1 << 19,
	IN_WEAPON1 = 1 << 20,
	IN_WEAPON2 = 1 << 21,
	IN_BULLRUSH	= 1 << 22,
};

#define IdxToEnt(idx)\
	idx ? (SrcEntity*)Src::CEntList->GetClientEntity(idx) : NULL\

#define IsPlayerIndex(idx)\
	((idx >= 1 && idx <= SrcLib::GetGlobalVars()->maxClients) ? true : false)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class	SrcLib
///
/// \brief	Static functions for interacting with the source engine.
////////////////////////////////////////////////////////////////////////////////////////////////////

class SrcLib
{
public:
	// Player
	static void						GetSteamID(SrcEntity*, CSteamID*);
	static C_BaseCombatWeapon*		GetActiveWeapon(SrcEntity*);
	static SrcEntity*				GetLocalPlayer();

	// Entity
	static void						SetPredictionRandomSeed(CUserCmd*);

	// Weapon
	static void						ItemPostFrame(C_BaseCombatWeapon*);
	static float					GetNextPrimaryFire(C_BaseCombatWeapon*);
	static bool						HasAmmo(C_BaseCombatWeapon* bWep);

	// Functions for unalligned interface/class calls
	static INetChannelInfo*			GetNetChannelInfo();
	static void						ProcessMovement(SrcEntity*, CMoveData*);
	static void						SetPlayerCurCommand(SrcEntity*, CUserCmd*);
	static float					GetPlayerTickBase(SrcEntity*);
	static char*					GetPlayerName(SrcEntity* bPly);
	static char*					GetClassname(SrcEntity* bEnt);
	static char						GetLifeState(SrcEntity* bPly);
	static bool						IsOnGround(SrcEntity* bEnt);
	static void						GetVectors(SrcEntity* bEnt, Vector* vForward, Vector* vRight, Vector* vUp);
	static bool						HasOwnerEntity(SrcEntity* bEnt);
	static MoveType_t				GetMoveType(SrcEntity* bPly);
	static int						GetWaterLevel(SrcEntity* bPly);
	static SrcEntity*				GetObserverTarget(SrcEntity* bPly);
	static QAngle					GetPunchAngle(SrcEntity* bPly);
	static bool						IsTakingScreenshot();

	// Wrapper functions for source engine calls
	static bool						IsPointEntityVisible(Vector vStart, Vector vEnd, SrcEntity* eTarget);

	// Other
	static void						Normalize(QAngle& qAng, float fLow = -180.0f, float fHigh = 180.0f);
	static float					Normalize(float fVal, float fLow = -180.0f, float fHigh = 180.0f);
	static bool						WorldToScreen(const Vector& vOrigin, Vector& vScreen);

	static CGlobalVarsBase*			GetGlobalVars();

	static uint32					GetSpectators(SrcEntity* bPly, int* ipSpectators, uint32 iBufSize);

	// Time-related
	static float					CurTime();
	
}; 
