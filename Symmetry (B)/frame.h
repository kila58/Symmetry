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
#include "panel.h"
#include "titlebar.h"

class SymFrame : public SymPanel
{
private:
	TitleBar*	m_pTitleBar;

public:

	// Forwarded functions for m_pTitleBar
	void	SetTitle(string sTitle);
	string	GetTitle();

	// Overrides
	virtual void Think();
	virtual void Init();
};