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

//#include "src_entity.h"

#include "detours.h"
#include "console_manager.h"

#include "lua_interface_manager.h"

#include "lua_manager.h"

#include "virtualcontainer.h"

#include "communications.h"

#include "lua_hook_manager.h"

#include "src_surface.h"
#include "src_luashared.h"

#include "symaimbot.h"
#include "symvisuals.h"

#include "winutil.h"

#include "basepanel.h"

using namespace Core;
using namespace Core::Lua;

using namespace Lib;
using namespace Lib::Detours;
using namespace Core::Console;
using namespace Lib::Lua;

////////////////////////////////////////////////////////////////////////////////////////////////////
///										  == CLASSES ==
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Class
{
	// Handles console input and output, as well as convars and console commands.
	extern ConsoleManager*				Console;

	// Handles every Lua interface.
	extern LuaInterfaceManager*			StateManager;

	// Handles Lua scripts.
	extern LuaManager*					ScriptManager;

	// Handles communication with the injector
	extern Core::IO::Communications*	Network;

	// Stores Lua files and resources
	extern Lib::IO::VirtualContainer*	VirtualC;

	// Handles Lua hooks
	extern Core::Lua::HookManager*		HookManager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///										 == GAME CLASSES ==
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Game
{
	// Handles Symmetry's aimbot
	extern SymAimbot*	Aimbot;

	// Handles Symmetry's visuals
	extern SymVisuals*	Visuals;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///								     == SOURCE INTERFACES ==
////////////////////////////////////////////////////////////////////////////////////////////////////

// All source interfaces
namespace Src
{
	extern IVEngineClient*				Engine;
	extern IVModelRender*				ModelRender;
	extern IVRenderView*				RenderView;
	extern IVModelInfoClient*			ModelInfo;
	extern IEngineTrace*				Trace;
	extern IClientEntityList*			CEntList;
	extern IBaseClientDLL*				Client;
	extern IMaterialSystem*				MatSys;
	extern ISteamClient*				SteamClient;
	extern IUniformRandomStream*		Random;
	extern IEngineTool*					Tool;
	extern IGameMovement*				GameMovement;
	extern ICvar*						CVar;
	extern IPrediction*					Prediction;
	extern IPanel*						Panel;
	extern IEngineVGui*					EngineUI;
	extern SrcSurface*					Surface;
	extern CInput*						Input;
	extern ClientModeShared*			CModeShared;
	extern SrcLuaShared*				LuaShared;
	extern IMemAlloc*					MemAlloc;
	extern void*						CViewRender;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///										 == VARIABLES ==
////////////////////////////////////////////////////////////////////////////////////////////////////

// Global variables
namespace Var
{
	// Currently working directory for the injector
	extern string					CWD;

	// Username of logged in user
	extern string					Username;

	// Expiration date of the user's subscription
	extern time_t					ExpiryDate;

	// Amount of resource files that need to be sent from the injector
	// to the binary (generally lua files)
	extern uint32					ResourceCount;

	// IPC stuff
	extern HANDLE					IPCInputPipe;
	extern HANDLE					IPCOutputPipe;

	extern HANDLE					IPCInjectorHandle;

	// Information about the last bullet we fired
	extern FireBulletsInfo_t*		LastBullet;

	// Used in FireBullets hook to check if the next bullet we fire is simulated or not
	// (simulated bullets are used by the aimbot to forcefully calculate the spread of the 
	// next bullet fired, simulated bullets should not fire traces or show bullet impacts)
	extern bool						NextBulletSimulated;

	// Screen width/height (set once, no need to recalculate each time)
	// NOTE: only really here to reduce ugly calls to Src::Engine->GetScreenSize
	extern int						ScreenW;
	extern int						ScreenH;

	// Shutting/shut down
	extern bool						ShuttingDown;
	extern bool						ShutDown;

	// A string-version
	extern string					Version;

	// Did render.Capture make another frame render just now?
	extern bool						IsTakingScreenshot;

	// Global parent parent for everything
	extern BasePanel*				WorldPanel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///								   == EXPORTED WINDOWS FUNCTION ==
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Win
{ 
	// Moved to Src
	//extern IMemAlloc*	MemAlloc;
}