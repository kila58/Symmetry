/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "testhooks_h.h"

#include "global.h"
#include "prediction.h"

#include "vmt.h"

#include "symconvar.h"
#include "src_lib.h"

#include "inetmessage.h"
#include "patterns.h"

NAMESPACE_CORE(Hooks)

Detours::VMT* test;

typedef ITexture*(__thiscall* testt)(void* vmt, int x, int y, int width, int height, unsigned char *data, ImageFormat dstFormat);

ITexture* __fastcall testhook(void* pThis, void*_, int x, int y, int width, int height, unsigned char *data, ImageFormat dstFormat)
{
	PrintDebugL("Called testhook %d, %d, %d, %d", x, y, width, height);
	return ((testt)(test->GetMethod(13)))(pThis, x, y, width, height, data, dstFormat);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void TestHooksH::Hook()
///
/// \brief	Hooks all functions in this file.
////////////////////////////////////////////////////////////////////////////////////////////////////

void TestHooksH::Hook()
{
	//test = new Detours::VMT((PDWORD*)Src::MatSys->GetRenderContext());
	//test->Hook((dword)testhook, 13);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void TestHooksH::UnHook()
///
/// \brief	Un-hooks all functions in this file.
////////////////////////////////////////////////////////////////////////////////////////////////////

void TestHooksH::UnHook()
{

}


NAMESPACE_END2