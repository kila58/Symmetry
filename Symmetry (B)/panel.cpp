/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *      ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /  *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "panel.h"
#include "global.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymPanel::Paint(int w, int h)
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymPanel::Paint(int w, int h)
{
	if (!BasePanel::Paint(w, h))
		return false;

	if (this->GetActive())
	{
		Src::Surface->DrawSetColor(THEME_ACTIVE);
	}
	else
		Src::Surface->DrawSetColor(THEME_MAIN);

	// Main body
	this->DrawFilledRect(0, 0, w, h);

	// Outline
	Src::Surface->DrawSetColor(THEME_HIGHLIGHT);
	this->DrawOutlinedRect(0, 0, w, h);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void SymPanel::SetActive(bool bActive)
////////////////////////////////////////////////////////////////////////////////////////////////////

void SymPanel::SetActive(bool bActive)
{
	this->m_bActive = bActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool SymPanel::GetActive()
////////////////////////////////////////////////////////////////////////////////////////////////////

bool SymPanel::GetActive()
{
	return this->m_bActive;
}