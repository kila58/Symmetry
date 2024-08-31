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
#include "src_surface.h"

enum PanelAttributes : uint32
{
	PA_HOVERED	= 1 << 0,	// there's a visible cursor hovering over this panel
	PA_LCLICKED = 1 << 1,	// there's a visible cursor hovering over this panel and left click is depressed
	PA_RCLICKED = 1 << 2,	// there's a visible cursor hovering over this panel and the right click is depressed
};

// Defines methods and members every panel should inherit
class BasePanel
{
private:

	// Basic x/y/w/h members
	int m_iX, m_iY;
	uint32 m_iWidth, m_iHeight;

	int m_iLastMouseX, m_iLastMouseY;

	uint32 m_iPanelAttributes;

	// Pointer to this panel's parent
	BasePanel*	m_pParent;

	// Children
	vector<BasePanel*> m_vChildren;

	// Other
	bool	m_bVisible;
	bool	m_bInitialized;

public:

	BasePanel();
	~BasePanel();

	void SetPos(int x, int y);
	void GetPos(int* x, int* y);

	void GetRealPos(int* x, int* y);
	void TranslateToRealPos(int* x, int* y);

	void SetSize(int w, int h);
	void GetSize(int* w, int* h);

	void SetVisible(bool bVisible);
	bool GetVisible();

	void		SetParent(BasePanel*);
	BasePanel*	GetParent();

	vector<BasePanel*>	GetChildren();
	BasePanel*			GetChild(int iChild);
	int					GetChildCount();

	void Remove();
	void Remove(int iChild);
	void Remove(BasePanel* pChild);
	void Add(BasePanel*);

	bool IsHovered();
	bool IsLeftClicked();
	bool IsRightClicked();

	void GetMouseOffset(int* x, int* y);

	// Virtual functions/events
	virtual bool Paint(int w, int h);
	virtual void Think();

	virtual void Init();
	virtual void Shutdown();

	virtual void OnMouseEntered();
	virtual void OnMouseExited();
	virtual void OnClick();
	virtual void OnClickReleased();
	virtual void OnLeftClick();
	virtual void OnLeftClickReleased();
	virtual void OnRightClick();
	virtual void OnRightClickReleased();
	virtual void OnMouseMove(int x, int y);

protected:

	// RenderUtil forwarded functions
	void DrawString(SrcHFont hFont, int x, int y, dword dwCol, uint32 iFlags, const char* pszText, ...);
	void DrawString(SrcHFont hFont, int x, int y, dword dwCol, uint32 iFlags, const wchar_t* pszText, ...);

	// Src::Surfacef forwarded functions
	void DrawFilledRect(int x0, int y0, int x1, int y1);
	void DrawOutlinedRect(int x0, int y0, int x1, int y1);
	void DrawLine(int x0, int y0, int x1, int y1);
	void DrawPolyLine(int *px, int *py, int numPoints);
	void DrawTexturedRect(int x0, int y0, int x1, int y1);
};