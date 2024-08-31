/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *      ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /  *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "titlebar.h"
#include "mouseutil.h"

#include "global.h"
//this->m_hFont = Src::Surface->CreateFont();
//Src::Surface->SetFontGlyphSet(this->m_hFont, "Lucida Console", 18, 400, 0, 0, FONTFLAG_ANTIALIAS);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void TitleBar::SetTitle(string sTitle)
///
/// \brief	Sets the title that will be drawn.  Wide characters are disallowed.
////////////////////////////////////////////////////////////////////////////////////////////////////

void TitleBar::SetTitle(string sTitle)
{
	this->m_sTitle = sTitle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	string TitleBar::GetTitle()
///
/// \brief	Returns the title that is drawn for this panel.
////////////////////////////////////////////////////////////////////////////////////////////////////

string TitleBar::GetTitle()
{
	return this->m_sTitle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool TitleBar::Paint(int w, int h)
////////////////////////////////////////////////////////////////////////////////////////////////////

bool TitleBar::Paint(int w, int h)
{
	if (!SymPanel::Paint(w, h))
		return false;

	PrintDebugL("being painted");

	this->DrawString(this->m_hFont, w / 2, 2, 0xFFFFFFFF, DRAW_CENTER_X, this->m_sTitle.c_str());
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void TitleBar::Think()
////////////////////////////////////////////////////////////////////////////////////////////////////

void TitleBar::Think()
{
	if (this->m_bDragging)
	{
		int mouseX, mouseY, thisX, thisY;
		MouseUtil::GetMousePos(&mouseX, &mouseY);
		this->GetPos(&thisX, &thisY);

		if (this->GetParent() && this->GetParent() != Var::WorldPanel)
		{
			this->SetPos(mouseX - this->m_iDragOffsetX + thisX, 
				mouseY - this->m_iDragOffsetY + thisY);
		}
	}

	BasePanel::Think();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void TitleBar::OnLeftClick()
////////////////////////////////////////////////////////////////////////////////////////////////////

void TitleBar::OnLeftClick()
{
	this->m_bDragging = true;
	this->GetMouseOffset(&this->m_iDragOffsetX, &this->m_iDragOffsetY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void TitleBar::OnLeftClickReleased()
////////////////////////////////////////////////////////////////////////////////////////////////////

void TitleBar::OnLeftClickReleased()
{
	this->m_bDragging = false;
}