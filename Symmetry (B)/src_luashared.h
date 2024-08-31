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

typedef PDWORD* ILuaInterface_t;

class SrcLuaShared
{
public:
	virtual void _unknown() = 0;
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void DumpStats() = 0;
	virtual ILuaInterface_t CreateLuaInterface(uchar cIndex, bool bSomething) = 0;
	virtual void CloseLuaInterface(ILuaInterface_t) = 0;
	virtual ILuaInterface_t GetLuaInterface(uchar cIndex) = 0;
	// more stuff we don't care about
};