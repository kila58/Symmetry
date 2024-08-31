/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *      ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /  *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once
#include "basepanel.h"

// Definitions for colours used
#define THEME_HIGHLIGHT Color(160, 160, 160, 255)
#define THEME_ACTIVE	Color(46, 46, 46, 255)
#define THEME_MAIN		Color(32, 32, 32, 255)

// Like the basepanel, except with generic drawn visuals
class SymPanel : public BasePanel
{
private:
	bool	m_bActive;
	
public:

	// Overrides
	virtual bool Paint(int w, int h);

public:

	// Other
	void	SetActive(bool bActive);
	bool	GetActive();
};