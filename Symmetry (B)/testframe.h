/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#include "basepanel.h"

class TestFrame : public BasePanel
{
private:
	bool m_bDragging;
	int x, y;
public:
	virtual void Paint();
	virtual void Think();
	virtual void OnMouseEntered();
	virtual void OnMouseExited();
	virtual void OnLeftClick();
	virtual void OnLeftClickReleased();
};