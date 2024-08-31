/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "ipanel_h.h"

#include "src_surface.h"

#include "global.h"

NAMESPACE_CORE(Hooks)

// Typedefs
typedef void(__thiscall* PaintTraverse_t)(IPanel*, SrcVPANEL, bool, bool);

Detours::VMT* g_IPanelHook = NULL;
SrcVPANEL g_spTop = NULL, g_spHUD = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void __stdcall PaintTraverse(SrcVPANEL vPanel)
///
/// \brief	Traverses a paintable panel.
///
/// \param	vPanel	???
////////////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall PaintTraverse(IPanel* pPanel, void*_, SrcVPANEL vPanel, bool bForceRepaint, bool bAllowForce)
{
	((PaintTraverse_t)g_IPanelHook->GetMethod(PANEL_PAINTRAVERSE))(pPanel, vPanel, bForceRepaint, bAllowForce);

	if (g_spTop == NULL)
	{
		if (!strcmp(Src::Panel->GetName(vPanel), "Awesomium"))
		{
			g_spTop = vPanel;
		}
	}

	if (g_spHUD == NULL)
	{
		if (!strcmp(Src::Panel->GetClassName(vPanel), "CHudGMod"))
		{
			g_spHUD = vPanel;
		}
	}

	// Draw watermarks 
	if (g_spTop && vPanel == g_spTop && !Src::Engine->IsInGame())
	{
		if (Game::Visuals)
			Game::Visuals->DrawWatermark();

		// Draw world panel
		if (Var::WorldPanel)
		{
			Var::WorldPanel->Think();
			Var::WorldPanel->Paint(Var::ScreenW, Var::ScreenH);
		}
	}

	// Draw HUD stuff (ESP)
	if (g_spHUD && vPanel == g_spHUD && Src::Engine->IsInGame())
	{
		// Draw world panel
		if (Var::WorldPanel)
		{
			Var::WorldPanel->Think();
			Var::WorldPanel->Paint(Var::ScreenW, Var::ScreenH);
		}

		if (Game::Visuals)
			Game::Visuals->HUDCycle();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void IPanelH::Hook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void IPanelH::Hook()
{
	if (!g_IPanelHook)
	{
		g_IPanelHook = new Detours::VMT((PDWORD*)Src::Panel);
		g_IPanelHook->Hook((dword)PaintTraverse, PANEL_PAINTRAVERSE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void IPanelH::UnHook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void IPanelH::UnHook()
{
	DeleteAndNull(g_IPanelHook);
}

NAMESPACE_END2