/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "symvisuals.h"

#include "src_interface.h"

#include "global.h"

#include "src_lib.h"

#include "symconvar.h"

#include "renderutil.h"

// Panel/UI
#include "frame.h"

using namespace Hooks;

MakeConVar4(uint32, esp_outline_col, "hexidecimal colour of outlines", 0x000000FF);
MakeConVar4(uint32, esp_box_col, "hexidecimal colour of the ESP box", 0xFF0000FF);
MakeConVar4(uint32, esp_fbox_col, "hexidecimal colour of the ESP box for steam friends", 0x00FF00FF);
MakeConVar4(uint32, esp_text_col, "hexidecimal colour of ESP text", 0xFFFFFFFF);
MakeConVar4(uint32, esp_health_col, "hexidecimal colour of ESP health bar", 0x00FF00FF);
MakeConVar4(uint32, esp_watermark_col, "hexidecimal colour of Symmetry version watermark in the menu", 0x7F7F7FFF);
MakeConVar4(uint32, esp_other_col, "hexidecimal colour of other non-player entities", 0xFFA500FF);

MakeConVar4(short, esp_draw, "enable esp.  0 to disable (overrides all other esp toggles)", 1);
MakeConVar4(short, esp_draw_outlines, "enable esp outlines. 0 to disable", 1);
MakeConVar4(short, esp_draw_box, "enable esp box. 0 to disable", 1);
MakeConVar4(short, esp_draw_health, "enable esp health bar. 0 to disable", 1);
MakeConVar4(short, esp_draw_weapon, "enable esp weapon name. 0 to disable", 1);
MakeConVar4(short, esp_draw_name, "enable esp player/entity name. 0 to disable", 1);
MakeConVar4(short, esp_draw_other, "enable esp on entities besides players, this will only draw the box and classname (use esp_regex to filter).  0 to disable", 0);
MakeConVar4(short, esp_basic_box, "uses a basic box for ESP, this will save FPS but looks a lot worse for entities.  0 to disable", 0);

MakeConVar4(uint32, wh_tone1_col, "hexidecimal colour of the first tone (visible parts)", 0x00FF00FF);
MakeConVar4(uint32, wh_tone2_col, "hexidecimal colour of the second tone (parts that are not visible)", 0xFF0000FF);
MakeConVar4(uint32, wh_other_col, "hexidecimal colour of other non-player entities", 0xFFFF00FF);

MakeConVar4(short, wh_draw, "draw xray/wallhack.  0 to disable  (overrides all other wallhac toggles)", 1);
MakeConVar4(short, wh_draw_other, "draw xray/wallhack for entities that are not players (use wh_regex to filter).  0 to disable", 0);
MakeConVar4(short, wh_twotone, "use two colour for wallhack, one for visible and invisible parts respectively.  0 to disable", 1);

MakeConVar4(short, esp_draw_other_sentonly, "(DOES NOT OVERRIDE REGEX) only render esp for scripted entities (esp_draw_other).  0 to disable", 0);
MakeConVar4(short, wh_draw_other_sentonly, "(DOES NOT OVERRIDE REGEX) only render wallhack for scripted entities (wh_draw_other).  0 to disable", 0);

MakeConVar4(string, esp_regex, "(SLOW OPERATION) esp regex pattern for other entities (esp_draw_other), use '.+' to match all entities", ".+");
MakeConVar4(string, wh_regex, "(SLOW OPERATION) wallhack regex pattern for other entities (wh_draw_other), use '.+' to match all entities", ".+");

MakeConVar4(short, v_draw_spectators, "draws the names of players spectating you in the top right.  0 to disable", 1);
MakeConVar4(short, v_hidescreen, "hides visuals when screenshots are being taken.  0 to disable", 1);

#ifdef BUILDMODE_BETA
MakeConVar5(short, v_showui, "shows the in-game UI.  0 = not shown, 1 = shown", 0, SymVisuals::ShowHideUI);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SymVisuals::SymVisuals()
///
/// \brief	Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

SymVisuals::SymVisuals()
{
	// Font shit
	this->m_hWatermarkFont	= Src::Surface->CreateFont();
	this->m_hESPFont		= Src::Surface->CreateFont();

	if (!Src::Surface->SetFontGlyphSet(this->m_hWatermarkFont, "Arial", 40, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_ITALIC))
	{
		PrintBadL("Couldn't create visual fonts!");
	}

	if (!Src::Surface->SetFontGlyphSet(this->m_hESPFont, "Arial", 13, 600, 0, 0, FONTFLAG_OUTLINE))
	{
		PrintBadL("Couldn't create visual fonts!");
	}

	// Wallhack material shit
	this->m_mWallhackTone1 = Src::MatSys->CreateMaterial("models/debug/debugwhite", NULL);
	this->m_mWallhackTone2 = Src::MatSys->CreateMaterial("models/debug/debugwhite", NULL);

	// Friend shit
	this->m_hSteamPipe = Src::SteamClient->CreateSteamPipe();
	this->m_hSteamUser = Src::SteamClient->ConnectToGlobalUser(this->m_hSteamPipe);
	this->m_pStreamFriends = Src::SteamClient->GetISteamFriends(this->m_hSteamUser, this->m_hSteamPipe, SRC_STEAM_FRIENDS);

	if (!this->m_pStreamFriends)
	{
		PrintDebugL("Warning, " SRC_STEAM_FRIENDS " is invalid");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SymVisuals::~SymVisuals()
///
/// \brief	Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////

SymVisuals::~SymVisuals()
{
	if (!Src::SteamClient->BReleaseSteamPipe(this->m_hSteamPipe))
	{
		PrintWarningL("Couldn't close steam pipe");
	}

	Src::SteamClient->ReleaseUser(this->m_hSteamPipe, this->m_hSteamUser);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymVisuals::Cycle()
///
/// \brief	Processes HUD visuals, rendering isn't done here.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymVisuals::HUDCycle()
{
	if ((SrcLib::IsTakingScreenshot() || Var::IsTakingScreenshot) && v_hidescreen.GetValue())
		return;

	this->DrawESP();
	this->DrawSpectators();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymVisuals::RenderCycle(void* pRenderer, const DrawModelState_t &state,
/// 	const ModelRenderInfo_t &info, matrix3x4_t*, DrawModelExecute_t original)
///
/// \brief	Called for each entity that is rendered.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymVisuals::RenderCycle(void* pRenderer, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t* bone,
	DrawModelExecute_t original)
{
	if ((SrcLib::IsTakingScreenshot() || Var::IsTakingScreenshot) && v_hidescreen.GetValue())
		return true;

	return this->DrawChams(pRenderer, state, info, bone, original);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymVisuals::DrawESP()
///
/// \brief	Draws the ESP (health, weapon, name and box)
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymVisuals::DrawESP()
{
	if (!esp_draw.GetValue())
		return;

	for (int i = 0; i <= Src::CEntList->GetHighestEntityIndex(); i++)
	{
		if (i == Src::Engine->GetLocalPlayer())
			continue;

		SrcEntity* ent = IdxToEnt(i);

		if (!ent)
			continue;

		bool isPlayer = IsPlayerIndex(i);
		bool steamFriend = false;

		if (isPlayer)
		{
			CSteamID steamID;
			SrcLib::GetSteamID(ent, &steamID);

			steamFriend = this->m_pStreamFriends->HasFriend(steamID, k_EFriendFlagImmediate);
		}

		// Do valid player checks
		if (isPlayer)
        {
			if (SrcLib::GetLifeState(ent) != LIFE_ALIVE)
				continue;

			// Don't want to see people that are spectating people or not moving at all
			if (SrcLib::GetMoveType(ent) == MOVETYPE_OBSERVER ||
				SrcLib::GetMoveType(ent) == MOVETYPE_NONE)
				continue;
		}
		else
		{
			// Needs regex match
			if (!Mem::IsRegexMatch(SrcLib::GetClassname(ent), esp_regex.GetValue()))
				continue;

			// Sent only
			if (esp_draw_other_sentonly.GetValue() && !(strstr(ent->GetClientClass()->GetName(), "SENT")))
				continue;

			// Don't wanna know about entities owned by players
			if (SrcLib::HasOwnerEntity(ent))
				continue;

			if (!esp_draw_other.GetValue())
				continue;
		}

		Vector mins, maxs;
		ent->GetRenderBounds(mins, maxs);

		Box2D box;

		if (esp_basic_box.GetValue())
		{
			if (!RenderUtil::Build2DFor3DBasic(ent, mins, maxs, &box))
				continue;
		}
		else
		{
			if (!RenderUtil::Build2DFor3DAdvanced(ent, mins, maxs, &box))
				continue;
		}
		
		// Draw outline (entities, players)
        if (esp_draw_outlines.GetValue())
        {
            Src::Surface->DrawSetColor(ConvarToColor(esp_outline_col));
            Src::Surface->DrawOutlinedRect(box.x1 - 1, box.y1 - 1, box.x2 + 1, box.y2 + 1);
        }

		if (isPlayer && !steamFriend)
			Src::Surface->DrawSetColor(ConvarToColor(esp_box_col));
		else if (steamFriend && isPlayer)
			Src::Surface->DrawSetColor(ConvarToColor(esp_fbox_col));
		else if (!isPlayer)
			Src::Surface->DrawSetColor(ConvarToColor(esp_other_col));
		Src::Surface->DrawOutlinedRect(box.x1, box.y1, box.x2, box.y2);

		if (esp_draw_outlines.GetValue())
		{
			Src::Surface->DrawSetColor(ConvarToColor(esp_outline_col));
			Src::Surface->DrawOutlinedRect(box.x1 + 1, box.y1 + 1, box.x2 - 1, box.y2 - 1);
		}

		float h = box.y2 - box.y1;
		float w = box.x2 - box.x1;

		// Draw name (entities, players (class name for entities, player name for players))
		if (esp_draw_name.GetValue())
		{
			if (isPlayer)
			{
				RenderUtil::DrawString(this->m_hESPFont, box.x1 + (w * 0.5), box.y1 + 1, esp_text_col.GetValue(),
					DRAW_CENTER_X | DRAW_BOTTOM_Y, SrcLib::GetPlayerName(ent));
			}
			else
			{
				RenderUtil::DrawString(this->m_hESPFont, box.x1 + (w * 0.5), box.y1 + 1, esp_text_col.GetValue(),
					DRAW_CENTER_X | DRAW_BOTTOM_Y, SrcLib::GetClassname(ent));
			}
		}

		// Draw weapon & health (players only)
		if (isPlayer)
		{
			if (esp_draw_weapon.GetValue() && SrcLib::GetActiveWeapon(ent))
			{
				char* weapon = SrcLib::GetClassname((SrcEntity*)SrcLib::GetActiveWeapon(ent));
				RenderUtil::DrawString(this->m_hESPFont, box.x1 + (w * 0.5), box.y2 - 1, esp_text_col.GetValue(),
					DRAW_CENTER_X, weapon);
			}

			if (esp_draw_health.GetValue())
			{
				// Draw health bar
				float mhealth = ent->GetMaxHealth();
				float health = min(mhealth, ent->GetHealth());

				int hpos = h * ((mhealth * health) / 10000.0f);

				Src::Surface->DrawSetColor(ConvarToColor(esp_health_col));
				Src::Surface->DrawFilledRect(box.x1 - 4, box.y2 - hpos, box.x1 - 3, box.y2);

				if (esp_draw_outlines.GetValue())
				{
					Src::Surface->DrawSetColor(ConvarToColor(esp_outline_col));
					Src::Surface->DrawOutlinedRect(box.x1 - 5, (box.y2 - 1) - hpos, box.x1 - 2, box.y2 + 1);
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymVisuals::DrawSpectators()
///
/// \brief	Draw spectator's names in the top right corner.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymVisuals::DrawSpectators()
{
	if (v_draw_spectators.GetValue())
	{
		// No server will have more than 512 players
		int spectators[512];

		if (uint32 count = SrcLib::GetSpectators(SrcLib::GetLocalPlayer(), spectators, sizeof(spectators)))
		{
			RenderUtil::DrawString(this->m_hESPFont, Var::ScreenW - 5, 5, 0xFF0000FF,
				DRAW_RIGHT_X | DRAW_TOP_Y, "| SPECTATORS |");

			for (uint32 i = 0; i < count; i++)
			{
				SrcEntity* ply = IdxToEnt(spectators[i]);

				if (!ply || !IsPlayerIndex(spectators[i]))
					continue;

				RenderUtil::DrawString(this->m_hESPFont, Var::ScreenW - 5, 5 + ((i + 1) * 20), 0x7F7F7FFF,
					DRAW_RIGHT_X | DRAW_TOP_Y, SrcLib::GetPlayerName(ply));
			}
		}
		else
		{
			RenderUtil::DrawString(this->m_hESPFont, Var::ScreenW - 5, 5, 0x00FF00FF,
				DRAW_RIGHT_X | DRAW_TOP_Y, "| NO SPECTATORS |");
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymVisuals::DrawChams(void* pRenderer, const DrawModelState_t &state,
/// 	const ModelRenderInfo_t &info, matrix3x4_t* bone, DrawModelExecute_t original)
///
/// \brief	Draws chams.  This is actually a render cycle.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymVisuals::DrawChams(void* pRenderer, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t* bone,
	DrawModelExecute_t original)
{
	if (wh_draw.GetValue())
	{
		this->m_mWallhackTone1->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
		this->m_mWallhackTone1->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, true);
		this->m_mWallhackTone2->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
		this->m_mWallhackTone2->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, true);

		if (IsPlayerIndex(info.entity_index) && Src::Engine->GetLocalPlayer() != info.entity_index)
		{
			SrcEntity* ply = IdxToEnt(info.entity_index);

			if (ply)
			{
				RenderUtil::SetRenderColour(wh_tone2_col.GetValue());
				Src::ModelRender->ForcedMaterialOverride(this->m_mWallhackTone1);
				original(pRenderer, state, info, bone);

				if (wh_twotone.GetValue())
				{
					RenderUtil::SetRenderColour(wh_tone1_col.GetValue());
					Src::ModelRender->ForcedMaterialOverride(this->m_mWallhackTone2);
					original(pRenderer, state, info, bone);
				}

				Src::ModelRender->ForcedMaterialOverride(NULL);

				return false;
			}
		}
		else if (wh_draw_other.GetValue())
		{
			SrcEntity* ent = IdxToEnt(info.entity_index);

			if (ent && Mem::IsRegexMatch(SrcLib::GetClassname(ent), wh_regex.GetValue()))
			{
				RenderUtil::SetRenderColour(wh_other_col.GetValue());
				Src::ModelRender->ForcedMaterialOverride(this->m_mWallhackTone1);
				original(pRenderer, state, info, bone);

				Src::ModelRender->ForcedMaterialOverride(NULL);

				return false;
			}
		}
	}

	// Call original normally
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymVisuals::DrawWatermark()
///
/// \brief	Draws the watermark in the menu.
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymVisuals::DrawWatermark()
{
	RenderUtil::DrawString(this->m_hWatermarkFont, 0, 0, esp_watermark_col.GetValue(), DRAW_NO_FLAGS, "Symmetry %s", Var::Version.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymVisuals::ShowHideUI(SymConVarBase* pBase, short sOld, short sNew)
///
/// \brief	Shows the 
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymVisuals::ShowHideUI(SymConVarBase* pBase, short sOld, short sNew)
{
	if (!Src::Engine)
		return;

	int screenW, screenH;
	int width = 600, height = 650;

	Src::Engine->GetScreenSize(screenW, screenH);

	static SymFrame frame;
	static bool created = false;

	if (!created)
	{
		frame.SetParent(Var::WorldPanel);
		frame.SetSize(width, height);
		frame.SetPos((screenW / 2) - (width / 2), (screenH / 2) - (height / 2));
		frame.SetTitle("Symmetry UI");

		created = true;
	}

	frame.SetVisible(sNew ? true : false);
}