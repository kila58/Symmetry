/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *      ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "basepanel.h"
#include "panel.h"

class TitleBar : public SymPanel
{
private:
	
	// Are we being dragged
	bool m_bDragging;

	// Where did we start dragging from?
	int m_iDragOffsetX, m_iDragOffsetY;

	// Title of the title bar, drawn in the top left
	string		m_sTitle;

	// Font used in title
	SrcHFont	m_hFont;

public:

	// Overrides
	virtual bool Paint(int w, int h);
	virtual void Think();
	virtual void OnLeftClick();
	virtual void OnLeftClickReleased();

public:

	void	SetTitle(string sTitle);
	string	GetTitle();
};