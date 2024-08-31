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
NAMESPACE_CORE(Lua)

class HookManager
{
private:

	// Reference to the function that's for each hook
	int		m_iCallFunction;
	
	// Cached Lua state (cached so we don't need to find it for each
	// hook that's called)
	lua_State*	m_sCache;

public:

	void SetCallFunction(int iFunction);

	void Call(char_c* cName);

	void StartCall(char_c* cName);
	void FinishCall(int iArgCount, int iReturnCount, bool bIsGameHook);

	void PushArg(void* vArg, int iType);

	bool IsReady();
};

NAMESPACE_END2