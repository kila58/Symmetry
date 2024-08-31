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
#include "symcmd.h"

class SymAimbot
{
private: // Members 

	// Variables that are set in the Setup method (used later in Perform)
	SrcEntity*			m_pMe;					// shorthand way of using ourself in the cheat - set each tick so we know 
												// we're still valid

	SrcEntity*			m_pCurrentTarget;		// the target that we calculated in Setup

    SrcEntity*			 m_pAimTarget;           // the player that we're aiming at, this is set in Setup only if
                                                // aim_triggerbot isn't disabled

	C_BaseCombatWeapon*	m_pWeapon;				// our current weapon

	Vector				m_vFrom, m_vTo;			// the postion we've calculated that we should shoot from -> to
												// this is done to query if a target is valid, so we cache them 
												// so that it can be used later when we aim at the target

	// Variables that are not reset each command
	SrcEntity*			m_pLockedTarget;	// target locking enabled? this is the target we're locked onto
												// locking is implemented in the "is better target" function, where the locked target is
												// always better than another target - our locked target will be "dumped" when it becomes
												// invalid
												
	HSteamPipe		m_hSteamPipe;				// Steam
	HSteamUser		m_hSteamUser;				// Steam
	ISteamFriends*	m_pStreamFriends;			// Steam

	bool			m_bInSmoothing;				// we aimed at our target this cycle, however we're in smoothing mode and we're not yet
												// pointing at our target, reset this when we're pointing fully at our target or if 
												// we don't have a target anymore - if this is true don't activate triggerbot
											
	SymCmd*			m_cmdModifier;				// this cycle's instance of the command modifier class
					
private: // Methods

	// Prediction
	void RunMovementPrediction(CUserCmd* pCmd);			// movement prediction
	void RunAnimationPrediction(CUserCmd* pCmd);		// animation prediction
	void RunCompensationPrediction(CUserCmd* pCmd);		// lag compensation prediciton (ping -> 200ms)
	void RunBulletPrediction(CUserCmd* pCmd);			// bullet spread prediction (for dynamic weapons)
	
	// Targeting
	bool			GetTargetHitbox(SrcEntity* pEnt, Vector* vpOut);
	void			DoSpreadCompensation(CUserCmd* pCmd, QAngle& qOut);
	SrcEntity*	FindTarget(CUserCmd* pCmd);
	void			AimAtTarget(CUserCmd* pCmd);

	// Validation/Comparison
	bool IsValidTarget(SrcEntity* pEnt);
	bool IsTargetPrefered(SrcEntity* pFirst, SrcEntity* pSecond);

public: // Methods

	// Constructor/Destructor
	SymAimbot();
	~SymAimbot();

	// Core
	bool Setup(CUserCmd* pCmd, SymCmd* pSymCmd);
	bool Perform(CUserCmd* pCmd);

	// Other public methods
	void FireWeapon(CUserCmd* pCmd);
};