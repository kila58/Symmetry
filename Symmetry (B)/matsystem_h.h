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
NAMESPACE_CORE(Hooks)

#define MATSYS_GETRENDERCONTEXT 98
#define MATSYS_SETRENDERCONTEXT 107
#define RENDC_READPIXELS		13

// Generic hooking method
class MatSystemH
{
public:
	static void	Hook();
	static void UnHook();
};

NAMESPACE_END2