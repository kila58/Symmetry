/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "symmetry.h"

#include "global.h"

#include "iluashared_h.h"
#include "iluagamemode_h.h"
#include "cbasentity_h.h"
#include "usercommand_h.h"
#include "testhooks_h.h"
#include "ipanel_h.h"
#include "engine_vgui_h.h"
#include "modelrender_h.h"
#include "matsystem_h.h"
#include "src_proxies.h"

#include "filea.h"

#include "src_interface.h"

#include "patterns.h"

using namespace Lib;
using namespace Lib::Detours;
using namespace Core::Hooks;
using namespace Core::IO;

// Global classes
LuaInterfaceManager*		Class::StateManager		= NULL;
LuaManager*					Class::ScriptManager	= NULL;
Communications*				Class::Network			= NULL;
Lib::IO::VirtualContainer*	Class::VirtualC			= NULL;
HookManager*				Class::HookManager		= NULL;
ConsoleManager*				Class::Console			= NULL;

// Global variables
string						Var::CWD = "";
string						Var::Username = "";
time_t						Var::ExpiryDate = 0;
uint32						Var::ResourceCount = 0xFFFFFFFF;

HANDLE						Var::IPCInputPipe		= NULL;
HANDLE						Var::IPCOutputPipe		= NULL;
HANDLE						Var::IPCInjectorHandle	= NULL;

FireBulletsInfo_t*			Var::LastBullet				= new FireBulletsInfo_t;
bool						Var::NextBulletSimulated	= false;

int							Var::ScreenW;
int							Var::ScreenH;

bool						Var::ShuttingDown	= false;
bool						Var::ShutDown		= false;

string						Var::Version = "";

bool						Var::IsTakingScreenshot = false;
BasePanel*					Var::WorldPanel;

// Source interfaces
IVEngineClient*				Src::Engine;
IVModelRender*				Src::ModelRender;
IVRenderView*				Src::RenderView;
IVModelInfoClient*			Src::ModelInfo;
IEngineTrace*				Src::Trace;
IClientEntityList*			Src::CEntList;
IBaseClientDLL*				Src::Client;
IMaterialSystem*			Src::MatSys;
ISteamClient*				Src::SteamClient;
IUniformRandomStream*		Src::Random;
IEngineTool*				Src::Tool;
CInput*						Src::Input;
ClientModeShared*			Src::CModeShared;
IGameMovement*				Src::GameMovement;
ICvar*						Src::CVar;
IPrediction*				Src::Prediction;
IPanel*						Src::Panel;
SrcSurface*					Src::Surface;
IEngineVGui*				Src::EngineUI;
SrcLuaShared*				Src::LuaShared;
IMemAlloc*					Src::MemAlloc;
void*						Src::CViewRender;

// Game classes
SymAimbot*					Game::Aimbot;
SymVisuals*					Game::Visuals;

// WinExports
//CrtDelete_t					Win::Delete;
//CrtMalloc_t					Win::Malloc;

NAMESPACE_CORE_B

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PreCRTInit();
////////////////////////////////////////////////////////////////////////////////////////////////////

void PreCRTInit()
{
	// Needs to be initialized before CRT initializes globals
	// (CVars reference this in their constructors)
	Class::Console = new ConsoleManager();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void PostCRTInit();
////////////////////////////////////////////////////////////////////////////////////////////////////

void PostCRTInit()
{
	// Needed before IPC initialization downloads files
	Class::VirtualC = new Lib::IO::VirtualContainer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void IPCInit();
////////////////////////////////////////////////////////////////////////////////////////////////////

void IPCInit()
{
	if (Var::IPCInputPipe && Var::IPCOutputPipe)
	{
		Class::Network = new Communications(Var::IPCInputPipe, Var::IPCOutputPipe);

		try
		{
			// Receive variables
			while (Var::ResourceCount == 0xFFFFFFFF)
				Class::Network->Recv();

			// Receive files
			while (Class::VirtualC->GetList().size() != Var::ResourceCount)
				Class::Network->Recv();

			// Initialize everything else that doesn't need to be ran at a specific time
			Init();
		}
#ifdef BUILDMODE_BETA
		catch (Lib::IO::Exception e)
#else
		catch (Lib::IO::Exception)
#endif
		{
			// Delete network so Shutdown knows it can't print anything
			DeleteAndNull(Class::Network);
			Shutdown();

			SpewError("IPCInit failed (E%d)", e.GetErrorCode());
		}
	}
	else
	{
		// Delete network so Shutdown knows it can't print anything
		DeleteAndNull(Class::Network);		
		Shutdown();

		SpewError("IPCInit failed (In = %X, Out = %X)", Var::IPCInputPipe, Var::IPCOutputPipe);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void Init();
////////////////////////////////////////////////////////////////////////////////////////////////////

void Init()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	///									 == LOAD LUA LIBRARIES ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Class::StateManager		= new LuaInterfaceManager();
	Class::ScriptManager	= new LuaManager(CLIENT);
	Class::HookManager		= new HookManager();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///							      == RESOLVE MEMORY SIGNATURES ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	if (!Patterns::Validate())
	{
		SpewError("Patterns are not valid.  Report this error!");
		Shutdown();

		return;
	}

	PrintGoodL("Loaded %d pattern hashes", Patterns::GetPatternCount());

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///							      == RESOLVE SOURCE INTERFACES ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	SrcInterfaceGroup interfaces[] =
	{
		{ (void**)&Src::Engine,			SRC_ENGINE,				"engine.dll" },
		{ (void**)&Src::ModelRender,	SRC_MODEL_RENDER,		"engine.dll" },
		{ (void**)&Src::RenderView,		SRC_RENDER_VIEW,		"engine.dll" },
		{ (void**)&Src::ModelInfo,		SRC_MODEL_INFO,			"engine.dll" },
		{ (void**)&Src::Trace,			SRC_TRACE,				"engine.dll" },
		{ (void**)&Src::CEntList,		SRC_ENT_LIST,			"client.dll" },
		{ (void**)&Src::Client,			SRC_CLIENT,				"client.dll" },
		{ (void**)&Src::MatSys,			SRC_MAT_SYS,			"materialsystem.dll" },
		{ (void**)&Src::SteamClient,	SRC_STEAM_CLIENT,		"steamclient.dll" },
		{ (void**)&Src::Random,			SRC_RANDOM,				"engine.dll" },
		{ (void**)&Src::Tool,			SRC_TOOL,				"engine.dll" },
		{ (void**)&Src::GameMovement,	SRC_GAMEMOVEMENT,		"client.dll" },
		{ (void**)&Src::CVar,			SRC_CVAR,				"vstdlib.dll" },
		{ (void**)&Src::Prediction,		SRC_PREDICTION,			"client.dll" },
		{ (void**)&Src::Surface,		SRC_SURFACE,			"vguimatsurface.dll" },
		{ (void**)&Src::Panel,			SRC_PANEL,				"vgui2.dll" },
		{ (void**)&Src::EngineUI,		SRC_ENGINEUI,			"engine.dll" },
		{ (void**)&Src::LuaShared,		SRC_LUASHARED,			"lua_shared.dll" }
	};

	for (uint32 i = 0; i < sizeof(interfaces) / sizeof(SrcInterfaceGroup); i++)
	{
		SrcInterfaceGroup group = interfaces[i];
		if ((*group.Interface = SrcInterface::Resolve(group.ModuleName, group.InterfaceName)) == NULL)
		{
			PrintDebugL("%s is not a valid interface", interfaces[i].InterfaceName);
			SpewError("Bad interface.  Report this error!");

			Shutdown();
			return;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///							      == WINDOWS EXPORTED FUNCTIONS ==
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	WinExportedFnGroup winexports[] =
	{
		{ (void**)&Src::MemAlloc,		"tier0.dll",		TIER0_MEMALLOC, true },
	};

	for (uint32 i = 0; i < sizeof(winexports) / sizeof(WinExportedFnGroup); i++)
	{
		WinExportedFnGroup group = winexports[i];
		if (HMODULE module = GetModuleHandle(group.ModuleName))
		{
			FARPROC ret = GetProcAddress(module, group.FunctionName);

			if (!ret)
			{
				PrintDebugL("%s => %s is not a valid winexport (%X, %X)", group.ModuleName,
					group.FunctionName, module, *group.FunctionPtr);

				SpewError("Bad winexport.  Report this error!");

				Shutdown();
				return;
			}
			else
			{
				if (group.Dereference)
				{
					*group.FunctionPtr = *(void**)ret;
				}
				else
					*group.FunctionPtr = (void*)ret;
			}
		}
	}

	// Relies on patterns, so this doesn't need extra safe-checks
	Src::Input			= **(CInput***)MakePtr(dword*, Patterns::CInput, 0x02);
	Src::CModeShared	= **(ClientModeShared***)MakePtr(dword*, Patterns::ClientModeShared, 0x02);
	Src::CViewRender	= **(void***)MakePtr(void*, Patterns::CViewRender, 0x02);

	PrintGoodL("Loaded %d interfaces", sizeof(interfaces) / sizeof(SrcInterfaceGroup));

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///									    == GAME CLASSES ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Game::Aimbot = new SymAimbot();
	Game::Visuals = new SymVisuals();

	// World panel class, not a game class but oh well
	Var::WorldPanel = new BasePanel();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///						   == LOAD SAVED CONVARS / CREATE CONVAR FILE ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	if (FileExistsG("default.cfg"))
	{
		Class::Console->Exec(ReadFileG("default.cfg"));
		PrintGoodL("Loaded %d convars, %d binds & %d concommands", Class::Console->GetConvarCount(),
			Class::Console->GetBindCount(), Class::Console->GetConComCount());
	}
	else
	{
		Class::Console->SaveConfig("default.cfg");
		PrintGoodL("Created default settings (default.cfg)");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///							 == REGISTER CONSOLE CMOMANDS ON INJECTOR ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	for (IConsoleEntry* e : Class::Console->GetConsoleEntries())
	{
		if (Class::Network)
			Class::Network->Send(Lib::IO::IpcAddCommand(e->GetName()));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///									  == PERFORM ALL HOOKS ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	MH_Initialize();

	//LuaSharedH::Hook(); //don't uncomment
	//LuaGamemodeH::Hook(); //fix after esp_draw crash
	//CBaseEntityH::Hook(); //fix after esp_draw crash
	UserCommandH::Hook();
	IPanelH::Hook();
	EngineVGUIH::Hook();
	ModelRenderH::Hook();
	MatSystemH::Hook();
	SrcProxies::Hook();

#ifdef BUILDMODE_BETA
	TestHooksH::Hook();
#endif

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///								     == OTHER INITIALIZATION ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Src::Engine->GetScreenSize(Var::ScreenW, Var::ScreenH);

	// Transfer Lua files from the global virtual container to 
	// the script manager's vector of autorun scripts
	for (uint32 i = 0; i < Class::VirtualC->GetList().size(); i++)
	{
		Lib::IO::VirtualFile* v = Class::VirtualC->GetList()[i];

		if (v->LuaAutorun)
			Class::ScriptManager->AddAutorunFile(v->Data, v->Name, v->Encrypted);
	}

	// Do welcome message thing
	PrintGoodL("Welcome, %s!", Var::Username.c_str());
	PrintGoodL("Your subscription is valid until %s", ctime(&Var::ExpiryDate));
	
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Idle, 0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void Idle()
////////////////////////////////////////////////////////////////////////////////////////////////////

void Idle()
{
	try
	{
		while (!Var::ShuttingDown && !Var::ShutDown)
		{
			Class::Network->Recv();
		}
	}
#ifdef BUILDMODE_BETA
	catch (Lib::IO::Exception e)
#else
	catch (Lib::IO::Exception)
#endif
	{
		if (Var::ShuttingDown || Var::ShutDown)
			return;

		// Delete network so Shutdown knows it can't print anything
		DeleteAndNull(Class::Network);
		Shutdown();

		SpewError("Idle error (E%d)", e.GetErrorCode());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void WaitInjector()
////////////////////////////////////////////////////////////////////////////////////////////////////

void WaitInjector()
{
	if (Var::IPCInjectorHandle == NULL || Var::IPCInjectorHandle == INVALID_HANDLE_VALUE)
	{
		SpewError("Bad injector handle in WaitInjector");
		Shutdown();
	}
	else
	{
		WaitForSingleObject(Var::IPCInjectorHandle, INFINITE);
		//CloseHandle(Var::IPCInjectorHandle);

		// Already shut down
		if (Var::ShuttingDown || Var::ShutDown)
			return;

		// Delete network so Shutdown knows it can't print anything
		DeleteAndNull(Class::Network);
		Shutdown();

		SpewError("Injector process exited");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void Shutdown();
////////////////////////////////////////////////////////////////////////////////////////////////////

void Shutdown()
{
	Var::ShuttingDown = true;

	if (Var::ShutDown)
		return;

	try
	{
		// If we've a working network class, send IPC_EXIT to the injector
		if (Class::Network)
			Class::Network->Send(Lib::IO::IpcExit());
	}
	catch (...)
	{
		// nothing we can really do here
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///									  == REMOVE ALL HOOKS ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// JMP detours
	MH_Uninitialize();

	// VMT detours
	UserCommandH::UnHook();
	IPanelH::UnHook();
	EngineVGUIH::UnHook();
	ModelRenderH::UnHook();
	LuaSharedH::UnHook();
	MatSystemH::UnHook();
	SrcProxies::UnHook();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///									  == DESTROY ALL DATA ==
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// Destroy global classes/variables
	DeleteAndNull(Class::StateManager);
	DeleteAndNull(Class::ScriptManager);
	DeleteAndNull(Class::VirtualC);
	DeleteAndNull(Class::HookManager);
	//DeleteAndNull(Class::Console);
	DeleteAndNull(Class::Network);
	DeleteAndNull(Game::Aimbot);
	DeleteAndNull(Game::Visuals);
	//DeleteAndNull(Var::LastBullet);
	DeleteAndNull(Var::WorldPanel);

	Var::ShutDown = true;
}

NAMESPACE_END1