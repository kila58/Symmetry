/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "modelrender_h.h"

#include "vmt.h"

#include "global.h"

#include "src_lib.h"

NAMESPACE_CORE(Hooks)

Detours::VMT* g_ModelRender = NULL;

typedef void(__thiscall* DrawModelExecute_t)(void*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state,
/// 	const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
///
/// \brief	Hooks IVModelRender::DrawModelExecute.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall DrawModelExecute(void* pECX, void* _, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t* bone)
{
	DrawModelExecute_t fn = ((DrawModelExecute_t)g_ModelRender->GetMethod(MRNDR_DRAWMEXECUTE));

	if (Game::Visuals)
	{
		if (!Game::Visuals->RenderCycle(pECX, state, info, bone, fn))
			return;
	}
		
	return fn(pECX, state, info, bone);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ModelRenderH::Hook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void ModelRenderH::Hook()
{
	g_ModelRender = new Detours::VMT((PDWORD*)Src::ModelRender);
	g_ModelRender->Hook((dword)DrawModelExecute, MRNDR_DRAWMEXECUTE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void ModelRenderH::UnHook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void ModelRenderH::UnHook()
{
	DeleteAndNull(g_ModelRender);
}

NAMESPACE_END2