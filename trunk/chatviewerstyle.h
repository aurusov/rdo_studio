#ifndef CHATVIEWERSTYLE_H
#define CHATVIEWERSTYLE_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- CChatViewerTheme
// ----------------------------------------------------------------------------
enum CChatFontStyle {
	CFS_NONE      = 0x00,
	CFS_BOLD      = 0x01,
	CFS_ITALIC    = 0x02,
	CFS_UNDERLINE = 0x04
};

class CChatViewerTheme
{
public:
	CChatViewerTheme();
	virtual ~CChatViewerTheme();

	CChatViewerTheme& operator =( const CChatViewerTheme& theme );
	bool operator ==( const CChatViewerTheme& theme ) const;
	bool operator !=( const CChatViewerTheme& theme ) const;

	COLORREF messageColor;
	COLORREF messageBg1Color;
	COLORREF messageBg2Color;
	COLORREF connectColor;
	COLORREF connectBgColor;
	COLORREF disconnectColor;
	COLORREF disconnectBgColor;
	COLORREF changeNameColor;
	COLORREF changeNameBgColor;
	COLORREF changeStatusModeColor;
	COLORREF changeStatusModeBgColor;
	COLORREF toCryOutColor;
	COLORREF toCryOutBgColor;
	COLORREF privateMsgSendColor;
	COLORREF privateMsgSendBgColor;
	COLORREF privateMsgReceiveColor;
	COLORREF privateMsgReceiveBgColor;
	COLORREF viewerBgColor;
	COLORREF selectedBgColor;

	CChatFontStyle messageFS;
	CChatFontStyle connectFS;
	CChatFontStyle disconnectFS;
	CChatFontStyle changeNameFS;
	CChatFontStyle changeStatusModeFS;
	CChatFontStyle toCryOutFS;
	CChatFontStyle privateMsgSendFS;
	CChatFontStyle privateMsgReceiveFS;

	static CChatViewerTheme getDefaultTheme();

	void init();
	void saveSetting() const;
};

// ----------------------------------------------------------------------------
// ---------- CChatViewerFont
// ----------------------------------------------------------------------------
class CChatViewerFont
{
public:
	CChatViewerFont();
	virtual ~CChatViewerFont();

	CChatViewerFont& operator =( const CChatViewerFont& font );
	bool operator ==( const CChatViewerFont& font ) const;
	bool operator !=( const CChatViewerFont& font ) const;

	std::string  name;
	int          size;
	int          codepage;
	int          characterSet;
	
	void init();
	void saveSetting() const;
};

// ----------------------------------------------------------------------------
// ---------- CChatViewerStyle
// ----------------------------------------------------------------------------
class CChatViewerStyle
{
protected:
	CChatViewerFont font;
	void fontChanged();

public:
	CChatViewerStyle();
	virtual ~CChatViewerStyle();

	CChatViewerTheme theme;

	int charWidth;
	int charHeight;
	int spaceWidth;
	CFont fontFSN;
	CFont fontFSB;
	CFont fontFSI;
	CFont fontFSU;
	CFont fontFSBI;
	CFont fontFSBU;
	CFont fontFSIU;
	CFont fontFSBIU;

	int horzBorder;

	CChatViewerStyle& operator =( const CChatViewerStyle& style );
	bool operator ==( const CChatViewerStyle& style ) const;
	bool operator !=( const CChatViewerStyle& style ) const;

	void init();
	void saveSetting() const;
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATVIEWERSTYLE_H
