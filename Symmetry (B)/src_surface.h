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
#include "src_generic.h"
#include "types.h"

// Panel Handle
typedef uint32 SrcVPANEL;

// Other handles
typedef dword SrcHScheme;
typedef dword SrcHTexture;
typedef dword SrcHPanel;
typedef dword SrcHFont;

const SrcHPanel		INVALID_PANEL = 0xffffffff;
const SrcHFont		INVALID_FONT = 0;

// Things I won't use 
class SrcIHTML;
class SrcIHTMLEvents;
class SrcIImage;
class IVguiMatInfo;
enum SurfaceFeature_e{};

typedef void* (*GetMouseCallback_t)();
typedef void* (*SetMouseCallback_t)();
typedef void* (*PlaySoundFunc_t)();

// FontDrawType_t
enum SrcFontDrawType_t
{
	SRC_FONT_DRAW_DEFAULT = 0,
	SRC_FONT_DRAW_NONADDITIVE,
	SRC_FONT_DRAW_ADDITIVE,
	SRC_FONT_DRAW_TYPE_COUNT = 2,
};

// Font flags, thanks to Sasha for these
enum SrcFontFlag
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC			= 0x001,
	FONTFLAG_UNDERLINE		= 0x002,
	FONTFLAG_STRIKEOUT		= 0x004,
	FONTFLAG_SYMBOL			= 0x008,
	FONTFLAG_ANTIALIAS		= 0x010,
	FONTFLAG_GAUSSIANBLUR	= 0x020,
	FONTFLAG_ROTARY			= 0x040,
	FONTFLAG_DROPSHADOW		= 0x080,
	FONTFLAG_ADDITIVE		= 0x100,
	FONTFLAG_OUTLINE		= 0x200,
	FONTFLAG_CUSTOM			= 0x400,
};

// Drawing flags for text
enum DrawFlags : uint32
{
	DRAW_NO_FLAGS = 0,
	DRAW_CENTER_X = 1 << 0,
	DRAW_LEFT_X = 1 << 1, // default
	DRAW_RIGHT_X = 1 << 2,
	DRAW_CENTER_Y = 1 << 3,
	DRAW_TOP_Y = 1 << 4, // default
	DRAW_BOTTOM_Y = 1 << 5,
};

// Vertex_t (not even a typedef???)
struct SrcVertex_t
{
	SrcVertex_t() {}
	SrcVertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	Vector2D	m_Position;
	Vector2D	m_TexCoord;
};

// IntRect
struct SrcIntRect
{
	int x0;
	int y0;
	int x1;
	int y1;
};

// CharRenderInfo
struct SrcCharRenderInfo
{
	SrcFontDrawType_t	drawType;
	wchar_t				ch;
	bool				valid;
	bool				shouldclip;
	int					x, y;
	SrcVertex_t			verts[2];
	int					textureId;
	int					abcA;
	int					abcB;
	int					abcC;
	int					fontTall;
	SrcHFont			currentFont;
};

// Different cursors
enum SrcHCursor
{
	CURSOR_USER,
	CURSOR_NONE,
	CURSOR_ARROW,
	CURSOR_IBEAM,
	CURSOR_HOURGLASS,
	CURSOR_WAITARROW,
	CURSOR_CROSSHAIR,
	CURSOR_UP,
	CURSOR_SIZENWSE,
	CURSOR_SIZENESW,
	CURSOR_SIZEEWE,
	CURSOR_SIZENS,
	CURSOR_SIZEALL,
	CURSOR_NO,
	CURSOR_HAND,
	CURSOR_BLANK,
	CURSOR_LAST
};

// CMatSystemSurface
class SrcSurface
{
public:
	// Methods of IAppSystem - USELESS
	virtual bool	Connect(CreateInterfaceFn* factory);
	virtual void	Disconnect();
	virtual void*	QueryInterface(const char *pInterfaceName);
	virtual int		Init();
	virtual void	Shutdown();

	// MORE USELESS PADDING
	virtual void RunFrame();										// v = 5
	virtual SrcVPANEL	GetEmbeddedPanel();
	virtual void		SetEmbeddedPanel(SrcVPANEL pEmbeddedPanel);
	virtual void PushMakeCurrent(SrcVPANEL panel, bool useInSets);
	virtual void PopMakeCurrent(SrcVPANEL panel);


	virtual void DrawSetColor(int r, int g, int b, int a);			// v = 10
	virtual void DrawSetColor(Color col);
	virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawFilledRectArray(SrcIntRect *pRects, int numRects) = 0;
	virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;		// v = 15
	virtual void DrawPolyLine(int *px, int *py, int numPoints) = 0;
	virtual void DrawSetTextFont(SrcHFont font) = 0;
	virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;
	virtual void DrawSetTextColor(Color col) = 0;
	virtual void DrawSetTextPos(int x, int y) = 0;					// v = 20
	virtual void DrawGetTextPos(int& x, int& y) = 0;	
	virtual void DrawPrintText(const wchar_t *text, int textLen, SrcFontDrawType_t drawType = SRC_FONT_DRAW_DEFAULT) = 0;
	virtual void DrawUnicodeChar(wchar_t wch, SrcFontDrawType_t drawType = SRC_FONT_DRAW_DEFAULT) = 0;
	virtual void DrawFlushText() = 0;

	// useless
	virtual SrcIHTML *CreateHTMLWindow(SrcIHTMLEvents *events, SrcVPANEL context) = 0;	// v = 25
	virtual void PaintHTMLWindow(SrcIHTML *htmlwin) = 0;
	virtual void DeleteHTMLWindow(SrcIHTML *htmlwin) = 0;

	virtual int	 DrawGetTextureId(char const *filename) = 0;
	virtual bool DrawGetTextureFile(int id, char *filename, int maxlen) = 0;
	virtual void DrawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload) = 0; // v = 30
	virtual void DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload) = 0;
	virtual void DrawSetTexture(int id) = 0;
	virtual void DrawGetTextureSize(int id, int &wide, int &tall) = 0;
	virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
	virtual bool IsTextureIDValid(int id) = 0;						// v = 35
	virtual void DeleteTextureByID(int id) = 0;
	virtual int CreateNewTextureID(bool procedural = false) = 0;

	// Panel stuff mostly
	virtual void GetScreenSize(int &wide, int &tall) = 0;
	virtual void SetAsTopMost(SrcVPANEL panel, bool state) = 0;
	virtual void BringToFront(SrcVPANEL panel) = 0;					// v = 40
	virtual void SetForegroundWindow(SrcVPANEL panel) = 0;
	virtual void SetPanelVisible(SrcVPANEL panel, bool state) = 0;
	virtual void SetMinimized(SrcVPANEL panel, bool state) = 0;
	virtual bool IsMinimized(SrcVPANEL panel) = 0;
	virtual void FlashWindow(SrcVPANEL panel, bool state) = 0;		// v = 45
	virtual void SetTitle(SrcVPANEL panel, const wchar_t *title) = 0;
	virtual void SetAsToolBar(SrcVPANEL panel, bool state) = 0;

	virtual void CreatePopup(SrcVPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
	virtual void SwapBuffers(SrcVPANEL panel) = 0;
	virtual void Invalidate(SrcVPANEL panel) = 0;					// v = 50
	virtual void SetCursor(SrcHCursor cursor) = 0;
	virtual void SetCursorAlwaysVisible(bool) = 0;
	virtual bool IsCursorVisible() = 0;
	virtual void ApplyChanges() = 0;
	virtual bool IsWithin(int x, int y) = 0;						// v = 55
	virtual bool HasFocus() = 0;									

	virtual bool SupportsFeature(SurfaceFeature_e feature) = 0;		
	virtual void RestrictPaintToSinglePanel(SrcVPANEL panel) = 0;
	virtual void SetModalPanel(SrcVPANEL) = 0;
	virtual SrcVPANEL GetModalPanel() = 0;							// v = 60
	virtual void UnlockCursor() = 0;								
	virtual void LockCursor() = 0;									
	virtual void SetTranslateExtendedKeys(bool state) = 0;
	virtual SrcVPANEL GetTopmostPopup() = 0;
	virtual void SetTopLevelFocus(SrcVPANEL panel) = 0;				// v = 65

	// font stuff
	virtual SrcHFont CreateFont() = 0;
	virtual bool SetFontGlyphSet(SrcHFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int a = 0, int b = 0) = 0;
	virtual bool AddCustomFontFile(const char *fontFileName) = 0;
	virtual int GetFontTall(SrcHFont font) = 0;
	virtual int GetFontTallRequested(SrcHFont font) = 0;
	virtual int GetFontAscent(SrcHFont font, wchar_t wch) = 0;
	virtual bool IsFontAdditive(SrcHFont font) = 0;
	virtual void GetCharABCwide(SrcHFont font, int ch, int &a, int &b, int &c) = 0;
	virtual int GetCharacterWidth(SrcHFont font, int ch) = 0;
	virtual void GetTextSize(SrcHFont font, const wchar_t *text, int &wide, int &tall) = 0;

	// LOTS MORE USELESS'NESS DOWN THIS WAY
};