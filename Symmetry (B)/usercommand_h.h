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

#define CINPUT_GETUSERCMD	8
#define CINPUT_CREATEMOVE	3
#define CINPUT_WRITECMD		5

#define CLIENT_CREATEMOVE	21
#define CLIENT_KEYEVENT		20

// Generic hooking method (only supports one hook)
class UserCommandH
{
public:
	static void	Hook();
	static void UnHook();
};

NAMESPACE_END2