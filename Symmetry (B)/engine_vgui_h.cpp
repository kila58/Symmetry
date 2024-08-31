/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "engine_vgui_h.h"

#include "vmt.h"

#include "global.h"

NAMESPACE_CORE(Hooks)

Detours::VMT* g_EngineVGUI;

typedef void(__thiscall* UpdateButtonState_t)(void* pECX, const InputEvent_t&);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool __fastcall KeyEvent(void* pECX, void* _, const InputEvent_t& input)
///
/// \brief	Key event.
////////////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall UpdateButtonState(void* pECX, void* pEDX, const InputEvent_t& input)
{
	bool down = input.m_nType != IE_ButtonReleased;
	ButtonCode_t code = (ButtonCode_t)input.m_nData;

	if (Class::Console && Class::Console->RunBind(code, input.m_nType == IE_ButtonReleased))
		return;

	return ((UpdateButtonState_t)g_EngineVGUI->GetMethod(VGUI_UPDATEBSTATE))(pECX, input);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void EngineVGUIH::Hook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void EngineVGUIH::Hook()
{
	g_EngineVGUI = new Detours::VMT((PDWORD*)Src::EngineUI);
	g_EngineVGUI->Hook((dword)UpdateButtonState, VGUI_UPDATEBSTATE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void EngineVGUIH::UnHook()
////////////////////////////////////////////////////////////////////////////////////////////////////

void EngineVGUIH::UnHook()
{
	DeleteAndNull(g_EngineVGUI);
}

NAMESPACE_END2