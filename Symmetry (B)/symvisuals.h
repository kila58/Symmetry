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
#include "src_surface.h"
#include "modelrender_h.h"
#include "symconvar.h"

typedef void(__thiscall* DrawModelExecute_t)(void*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

class SymVisuals
{
private:
	HSteamPipe		m_hSteamPipe;
	HSteamUser		m_hSteamUser;
	ISteamFriends*	m_pStreamFriends;

	SrcHFont		m_hWatermarkFont;
	SrcHFont		m_hESPFont;

	IMaterial*		m_mWallhackTone1;
	IMaterial*		m_mWallhackTone2;

public:

	SymVisuals();
	~SymVisuals();

	// Main cycles
	void HUDCycle();
	bool RenderCycle(void* pRenderer, const DrawModelState_t &state, const ModelRenderInfo_t &info, 
		matrix3x4_t* bone, DrawModelExecute_t original);

	// Sub cycles
	void DrawESP();
	void DrawSpectators();
	bool DrawChams(void* pRenderer, const DrawModelState_t &state, const ModelRenderInfo_t &info, 
		matrix3x4_t* bone, DrawModelExecute_t original);

	// Panels
	static void ShowHideUI(Core::Console::SymConVarBase* pBase, short sOld, short sNew);
	static void ShowHideSpectators(Core::Console::SymConVarBase* pBase, short sOld, short sNew);
	static void ShowHideTargets(Core::Console::SymConVarBase* pBase, short sOld, short sNew);
	static void ShowHideRadar(Core::Console::SymConVarBase* pBase, short sOld, short sNew);

	// Other
	void DrawWatermark();
};