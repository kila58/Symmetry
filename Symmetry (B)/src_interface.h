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
#include "generic.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
///									 == INTERFACE VERSIONS ==
////////////////////////////////////////////////////////////////////////////////////////////////////

#define SRC_ENGINE			"VEngineClient013"
#define SRC_ENGINE_SERV		"VEngineServer021"
#define SRC_MODEL_RENDER	"VEngineModel016"
#define	SRC_RENDER_VIEW		"VEngineRenderView014"
#define SRC_MODEL_INFO		"VModelInfoClient006"
#define SRC_TRACE			"EngineTraceClient003"
#define SRC_ENT_LIST		"VClientEntityList003"
#define SRC_MAT_SYS			"VMaterialSystem080"
#define SRC_STEAM_CLIENT	"SteamClient012"
#define SRC_STEAM_FRIENDS	"SteamFriends003"
#define SRC_RANDOM			"VEngineRandom001"
#define SRC_TOOL			"VENGINETOOL003"
#define SRC_PINFO			"PlayerInfoManager001"
#define SRC_CLIENT			"VClient017"
#define SRC_GAMEMOVEMENT	"GameMovement001"
#define SRC_CVAR			"VEngineCvar004"
#define SRC_PREDICTION		"VClientPrediction001"
#define SRC_SURFACE			"VGUI_Surface030"
#define SRC_PANEL			"VGUI_Panel009"
#define SRC_ENGINEUI		"VEngineVGui001"
#define SRC_INPUTSYS		"InputSystemVersion001"
#define SRC_LUASHARED		"LUASHARED003"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \typedef	void* (*SrcCreateInterface_t)(const char*, int)
///
/// \brief	Simple static functions used with source interfaces.
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void* (*SrcCreateInterface_t)(const char*, int);

class SrcInterface
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	static void* SrcInterface::Resolve(const char* cModule, const char* cInterface)
	///
	/// \brief	Resolves an exported source interface.  Returns 0 if the interface wasn't found.
	///
	/// \param	cModule   	The module name.
	/// \param	cInterface	The interface name/version.
	///
	/// \return	.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static void* Resolve(const char* cModule, const char* cInterface)
	{
		HMODULE module = GetModuleHandle(cModule);

		// Check if the module exists
		if (!module)
			return 0;

		SrcCreateInterface_t fn = (SrcCreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle(cModule), "CreateInterface");

		// Check if the module exports anything
		if (!fn)
			return 0;

		// Return exported interface (can return 0!!)
		return fn(cInterface, NULL);
	}
};