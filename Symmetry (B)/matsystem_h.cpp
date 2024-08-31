/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "matsystem_h.h"

#include "vmt.h"

#include "global.h"
#include "ienginevgui.h"

#include "patterns.h"

using namespace Lib;

NAMESPACE_CORE(Hooks)

Detours::VMT* g_MatSystem		= NULL;
Detours::VMT* g_RenderContext	= NULL;

IMatRenderContext* g_CurrentRenderContext = NULL;

typedef IMatRenderContext* (__thiscall * GetRenderContext_t)(void*);
typedef void(__thiscall * SetRenderContext_t)(void*, IMatRenderContext*);
typedef ITexture*(__thiscall* ReadPixels_t)(void* vmt, int x, int y, int width, int height, unsigned char *data, ImageFormat dstFormat);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	ITexture* __fastcall ReadPixels(void* pThis, void*_, int x, int y, int width, int height,
/// 	unsigned char *data, ImageFormat dstFormat)
////////////////////////////////////////////////////////////////////////////////////////////////////

ITexture* __fastcall ReadPixels(void* pThis, void*_, int x, int y, int width, int height, unsigned char *data, ImageFormat dstFormat)
{
	int screenW, screenH;
	Src::Engine->GetScreenSize(screenW, screenH);

	if (width >= (screenW * 0.70) || height >= (screenH * 0.70))
	{
		// Already taking a screenshot?  
		if (Var::IsTakingScreenshot)
			return ((ReadPixels_t)g_RenderContext->GetMethod(RENDC_READPIXELS))(pThis, x, y, width, height, data, dstFormat);

		PrintWarningL("Screenshot detected: (X=%d, Y=%d, W=%d, H=%d)", x, y, width, height);

		Var::IsTakingScreenshot = true;

		// Clear current buffer
		g_CurrentRenderContext->ClearColor4ub(255, 255, 255, 225);
		g_CurrentRenderContext->ClearBuffers(true, true, true);

		vrect_t rect = { x, y, width, height };

		Patterns::CViewRender__Render(Src::CViewRender, &rect);

		// Render panels
		Src::RenderView->VGui_Paint(PaintMode_t::PAINT_UIPANELS);
		Src::RenderView->VGui_Paint(PaintMode_t::PAINT_INGAMEPANELS);

		Var::IsTakingScreenshot = false;

		return ((ReadPixels_t)g_RenderContext->GetMethod(RENDC_READPIXELS))(pThis, x, y, width, height, data, dstFormat);
	}
	else
	{
		PrintDebugL("RenderPixels: (X=%d, Y=%d, W=%d, H=%d)", x, y, width, height);
	}

	return ((ReadPixels_t)g_RenderContext->GetMethod(RENDC_READPIXELS))(pThis, x, y, width, height, data, dstFormat);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SetRenderContext(IMatRenderContext* pRender)
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetRenderContext(IMatRenderContext* pRender)
{
	g_CurrentRenderContext = pRender;

	return ((SetRenderContext_t)g_MatSystem->GetMethod(MATSYS_SETRENDERCONTEXT))(Src::MatSys, pRender);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void HookRenderContext()
///
/// \brief	Hook render context.
////////////////////////////////////////////////////////////////////////////////////////////////////

void HookRenderContext(IMatRenderContext* pRender)
{
	g_RenderContext = new Detours::VMT((PDWORD*)pRender);
	g_RenderContext->Hook((dword)ReadPixels, RENDC_READPIXELS);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void MatSystemH::Hook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void MatSystemH::Hook()
{
	g_MatSystem = new Detours::VMT((PDWORD*)Src::MatSys);
	g_MatSystem->Hook((dword)SetRenderContext, MATSYS_SETRENDERCONTEXT);

	if (g_CurrentRenderContext =
		((GetRenderContext_t)g_MatSystem->GetMethod(MATSYS_GETRENDERCONTEXT))(Src::MatSys))
	{
		HookRenderContext(g_CurrentRenderContext);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void MatSystemH::UnHook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void MatSystemH::UnHook()
{
	DeleteAndNull(g_MatSystem);
	DeleteAndNull(g_RenderContext);
}

NAMESPACE_END2