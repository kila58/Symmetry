/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "testframe.h"
#include "global.h"
#include "mouseutil.h"

void TestFrame::Paint()
{
	int x, y, w, h;
	this->GetPos(&x, &y);
	this->GetSize(&w, &h);

	Src::Surface->DrawSetColor(23, 23, 23, 255);
	Src::Surface->DrawFilledRect(x, y, x + w, y + h);

	Src::Surface->DrawSetColor(160, 160, 160, 255);
	Src::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void TestFrame::Think()
{
	if (this->m_bDragging)
	{
		int x, y;
		MouseUtil::GetMousePos(&x, &y);

		this->SetPos(x - this->x, y - this->y);
	}
}

void TestFrame::OnMouseEntered()
{

}

void TestFrame::OnMouseExited()
{

}

void TestFrame::OnLeftClick()
{
	this->m_bDragging = true;
	this->GetMouseOffset(&this->x, &this->y);
}

void TestFrame::OnLeftClickReleased()
{
	this->m_bDragging = false;
}