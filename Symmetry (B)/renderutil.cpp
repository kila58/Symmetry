/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                    ====================                      **/

#include "renderutil.h"

#include "global.h"

#include "src_lib.h"

#include "minmax.h"

void debug(int x, int y)
{
	Src::Surface->DrawLine(x - 3, y, x + 3, y);
	Src::Surface->DrawLine(x, y - 3, x, y + 3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void RenderUtil::SetRenderColour(uint32 iColour)
///
/// \brief	Helper function to convert a unsignd integer colour to a float array,
/// 		this function also calls SetColorModulation and SetBlend for convenience.
///
/// \param	iColour	The colour.
////////////////////////////////////////////////////////////////////////////////////////////////////

void RenderUtil::SetRenderColour(uint32 iColour)
{
	uchar r = (byte)((iColour & 0xFF000000) >> 24);
	uchar g = (byte)((iColour & 0x00FF0000) >> 16);
	uchar b = (byte)((iColour & 0x0000FF00) >> 8);
	uchar a = (byte)(iColour & 0x000000FF);

	float col[3] = { r / 255.0f, g / 255.0f, b / 255.0f };

	Src::RenderView->SetColorModulation(col);
	Src::RenderView->SetBlend(a / 255.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool RenderUtil::Draw2DFor3DBasic(SrcEntity* pEnt, Vector vMins, Vector vMaxs,
/// 	uint32 iPadCol, uint32 iMainCol, bool bDoOutlines)
///
/// \brief	Draws a basic 2D box around a 3D space.
///
/// \param [in,out]	pEnt	The target entity.
/// \param	vMins			OBB mins.
/// \param	vMaxs			OBB maxs.
/// \param	iPadCol			Colour for padding.
/// \param	iMainCol		Main colour.
/// \param	bDoOutlines 	True to draw outlines.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool RenderUtil::Build2DFor3DBasic(SrcEntity* pEnt, Vector vMins, Vector vMaxs, 
	Box2D* pBox)
{
	Vector mid = pEnt->GetAbsOrigin() + Vector(0, 0, (vMaxs - vMins).z * 0.5);

	Vector top = mid + Vector(0, 0, (vMaxs - vMins).z * 0.5);
	Vector bot = mid - Vector(0, 0, (vMaxs - vMins).z * 0.5);

	// Screen positions
	Vector s_top, s_bot;

	// Can't draw if either bottom top sections are outside of our screen
	if (!SrcLib::WorldToScreen(top, s_top))
		return false;

	if (!SrcLib::WorldToScreen(bot, s_bot))
		return false;

	float h = s_bot.y - s_top.y;
	float w = h * 0.5f;

	pBox->x1 = s_top.x - (w * 0.5);
	pBox->y1 = s_top.y;
	pBox->x2 = s_top.x + (w * 0.5);
	pBox->y2 = s_bot.y;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool RenderUtil::Build2DFor3DAdvanced(SrcEntity* pEnt, Vector vMins, Vector vMaxs,
/// 	Box2D* pBox)
///
/// \brief	Builds an advanced 2D box around a 3D space.  The advanced box (compared to basic)
/// 		is a lot more expensive to run, but generally looks better as it doesn't estimate as
/// 		many factors.
///
/// \param [in,out]	pEnt	The target entity.
/// \param	vMins			OBB mins.
/// \param	vMaxs			OBB maxs.
/// \param [in,out]	pBox	Pointer to output box.
///
/// \return	true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool RenderUtil::Build2DFor3DAdvanced(SrcEntity* pEnt, Vector vMins, Vector vMaxs, 
	Box2D* pBox)
{
	matrix3x4_t rotateMatrix;
	AngleMatrix(pEnt->GetAbsAngles(), rotateMatrix);

	// Lowest and highest X-Y points
	FindMinMax<float> minmaxX, minmaxY;

	Vector pos, rotated;
	for (int i = 0; i < 8; i++)
	{
		pos[0] = (i & 0x1) ? vMaxs[0] : vMins[0];
		pos[1] = (i & 0x2) ? vMaxs[1] : vMins[1];
		pos[2] = (i & 0x4) ? vMaxs[2] : vMins[2];

		VectorRotate(pos, rotateMatrix, rotated);
		rotated += pEnt->GetAbsOrigin();

		Vector screen;
		if (!SrcLib::WorldToScreen(rotated, screen))
			return false;

		minmaxX << screen.x;
		minmaxY << screen.y;
	}

	pBox->x1 = minmaxX.GetMin();
	pBox->y1 = minmaxY.GetMin();
	pBox->x2 = minmaxX.GetMax();
	pBox->y2 = minmaxY.GetMax();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void RenderUtil::DrawString(SrcHFont hFont, int x, int y, dword dwCol, uint32 iFlags,
/// 	const char* pszText, ...)
///
/// \brief	Draws a string using the surface library.
///
/// \param	hFont  	The font.
/// \param	x	   	The x coordinate.
/// \param	y	   	The y coordinate.
/// \param	dwCol  	The colour.
/// \param	iFlags 	The flags.
/// \param	pszText	The text.
////////////////////////////////////////////////////////////////////////////////////////////////////

void RenderUtil::DrawString(SrcHFont hFont, int x, int y, dword dwCol, uint32 iFlags, const char* pszText, ...)
{
	va_list va_alist;
	char buf[1024] = { '\0' };
	wchar_t str[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(buf, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(str, L"%S", buf);

	RenderUtil::DrawString(hFont, x, y, dwCol, iFlags, str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void RenderUtil::DrawString(SrcHFont hFont, int x, int y, dword dwCol, uint32 iFlags,
/// 	const wchar_t* pszText, ...)
///
/// \brief	Draws a string using the surface library.
///
/// \param	hFont  	The font.
/// \param	x	   	The x coordinate.
/// \param	y	   	The y coordinate.
/// \param	dwCol  	The col.
/// \param	iFlags 	Zero-based index of the flags.
/// \param	pszText	The text.
////////////////////////////////////////////////////////////////////////////////////////////////////

void RenderUtil::DrawString(SrcHFont hFont, int x, int y, dword dwCol, uint32 iFlags, const wchar_t* pszText, ...)
{
	va_list va_alist;
	wchar_t str[1024] = { '\0' };

	va_start(va_alist, pszText);
	wsprintfW(str, pszText, va_alist);
	va_end(va_alist);

	char r = (byte)((dwCol & 0xFF000000) >> 24);
	char g = (byte)((dwCol & 0x00FF0000) >> 16);
	char b = (byte)((dwCol & 0x0000FF00) >> 8);
	char a = (byte)(dwCol & 0x000000FF);

	int w, h;

	if (iFlags != DRAW_NO_FLAGS)
		Src::Surface->GetTextSize(hFont, str, w, h);

	// RIGHT text|
	// LEFT |text
	// BOTTOM TEXT
	//		  ====
	// TOP	  ====
	//		  TEXT

	if ((iFlags & DRAW_CENTER_X) == DRAW_CENTER_X)
		x -= (w * 0.50);
	if ((iFlags & DRAW_CENTER_Y) == DRAW_CENTER_Y)
		y -= (h * 0.50);
	else if ((iFlags & DRAW_RIGHT_X) == DRAW_RIGHT_X)
		x -= w;
	else if ((iFlags & DRAW_TOP_Y) == DRAW_TOP_Y)
		y += h;
	else if ((iFlags & DRAW_BOTTOM_Y) == DRAW_BOTTOM_Y)
		y -= h;

	Src::Surface->DrawSetTextFont(hFont);
	Src::Surface->DrawSetTextPos(x, y);
	Src::Surface->DrawSetTextColor(r, g, b, a);
	Src::Surface->DrawPrintText(str, wcslen(str));
}