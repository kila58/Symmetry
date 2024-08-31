/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "mouseutil.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	inline void MouseUtil::GetMousePos(int* x, int* y)
///
/// \brief	Gets the mouse position relative to the active window.  The active window is 
/// 		the game's window.
////////////////////////////////////////////////////////////////////////////////////////////////////

void MouseUtil::GetMousePos(int* x, int* y)
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(GetActiveWindow(), &point);

	*x = point.x;
	*y = point.y;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool MouseUtil::GetCursorVisible()
///
/// \brief	Returns true if the cursor is visible.  False if the cursor is hidden.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool MouseUtil::GetCursorVisible()
{
	CURSORINFO info;
	info.cbSize = sizeof(CURSORINFO);

	GetCursorInfo(&info);

	return (info.flags & CURSOR_SHOWING) == CURSOR_SHOWING;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void MouseUtil::SetCursorVisible(bool bVisible)
///
/// \brief	Sets whether or not the cursor is currently visible.
////////////////////////////////////////////////////////////////////////////////////////////////////

void MouseUtil::SetCursorVisible(bool bVisible)
{
	ShowCursor(bVisible ? TRUE : FALSE);
}