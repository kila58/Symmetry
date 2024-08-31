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

struct Box2D
{
	int x1, y1;
	int x2, y2;
};

class RenderUtil
{
public:
	// Sets the blend alpga and render colour using a single integer
	static void	SetRenderColour(uint32 iColour);

	// Builds a basic 2D box around a 3D space
	static bool Build2DFor3DBasic(SrcEntity* pEnt, Vector vMins, Vector vMaxs, 
		Box2D* pBox);

	// Builds an advanced 2D box around a 3D space, more expensive than basic 2D box
	static bool Build2DFor3DAdvanced(SrcEntity* pEnt, Vector vMins, Vector vMaxs, 
		Box2D* pBox);

	// Methods for drawing text
	static void DrawString(SrcHFont hFont, int x, int y, dword dwCol, uint32 iFlags, const char* pszText, ...);
	static void DrawString(SrcHFont hFont, int x, int y, dword dwCol, uint32 iFlags, const wchar_t* pszText, ...);
};