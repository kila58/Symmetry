/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   * 
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "global.h"
#include "cbasentity_h.h"

#include "patterns.h"

NAMESPACE_CORE(Hooks)

// Typedefs
typedef void(__thiscall * FireBullets_t)(SrcEntity* vmt, const FireBulletsInfo_t &info);
typedef void(__thiscall* PostThink_t)(SrcEntity*);

FireBullets_t	CBE_FireBullets;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void FireBullets(const FireBulletsInfo_t fbInfo&)
///
/// \brief	Hooked CBaseEntity::FireBullets.  Used to cache information about weapons when they
/// 		fire.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __stdcall FireBullets(const FireBulletsInfo_t &fbInfo)
{
	SrcEntity* thisptr; GetReg(ECX, thisptr);
	
	// We need these global interfaces
	if (!Src::CEntList || !Src::Engine)
		return CBE_FireBullets(thisptr, fbInfo);

	// Long call to get the LocalPlayer
	SrcEntity* me = (SrcEntity*)Src::CEntList->GetClientEntity(Src::Engine->GetLocalPlayer())->GetBaseEntity();

	// Only cache information from FireBullets when we're 
	// the one firing the bullets
	if (thisptr == me)
	{
		memcpy(Var::LastBullet, &fbInfo, sizeof(FireBulletsInfo_t));

        if (Var::NextBulletSimulated)
            return;
	}

	return CBE_FireBullets(thisptr, fbInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBaseEntityH::Hook()
///
/// \brief	Hooks CBaseEntity::FireBullets.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CBaseEntityH::Hook()
{
	MH_CreateHook(Patterns::SrcEntity__FireBullets, &FireBullets,
		reinterpret_cast<void**>(&CBE_FireBullets));
	MH_EnableHook(Patterns::SrcEntity__FireBullets);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CBaseEntityH::UnHook()
///
/// \brief	Un-hooks CBaseEntity::FireBullets.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CBaseEntityH::UnHook()
{
	MH_DisableHook(Patterns::SrcEntity__FireBullets);
	MH_RemoveHook(Patterns::SrcEntity__FireBullets);
}

NAMESPACE_END2