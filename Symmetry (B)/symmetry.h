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
NAMESPACE_CORE_B

// Structure for grouping a pointer to a interface,
// interface name and module name
struct SrcInterfaceGroup
{
	void**		Interface;
	char_c*		InterfaceName;
	char_c*		ModuleName;
};

struct WinExportedFnGroup
{
	void**	FunctionPtr;
	char_c*	ModuleName;
	char_c* FunctionName;
	bool	Dereference;
};

// Called before CRT has initialized
void PreCRTInit();

// Called after CRT is initialized
void PostCRTInit();

// Called to initialize IPC (download files and variables)
void IPCInit();

// Called after IPC has initialized
void Init();

// Called after initialization
void Idle();

// Called by Init on another thread to wait for the injector to exit
// and call Shutdown 
void WaitInjector();

// Called whenever Symmetry should shutdown completely
void Shutdown();

NAMESPACE_END1