#ifndef RDOEDITOREDITSTYLE_H
#define RDOEDITOREDITSTYLE_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTheme
// ----------------------------------------------------------------------------
enum RDOFontStyle {
	RDOFS_NONE      = 0x00,
	RDOFS_BOLD      = 0x01,
	RDOFS_ITALIC    = 0x02,
	RDOFS_UNDERLINE = 0x04
};

enum RDOBookmarkStyle {
	RDOBOOKMARKS_NONE = 0,
	RDOBOOKMARKS_CIRCLE,
	RDOBOOKMARKS_RECT,
	RDOBOOKMARKS_ROUNDRECT,
	RDOBOOKMARKS_ARROW
};

enum RDOFoldStyle {
	RDOFOLDS_NONE = 0,
	RDOFOLDS_PLUS,
	RDOFOLDS_PLUSCONNECTED,
	RDOFOLDS_ARROW,
	RDOFOLDS_ARROWCONNECTED,
	RDOFOLDS_BOXCONNECTED,
	RDOFOLDS_CIRCLECONNECTED
};

class RDOEditorEditTheme
{
public:
	RDOEditorEditTheme();
	virtual ~RDOEditorEditTheme();

	RDOEditorEditTheme& operator =( const RDOEditorEditTheme& theme );
	bool operator ==( const RDOEditorEditTheme& theme ) const;
	bool operator !=( const RDOEditorEditTheme& theme ) const;

	COLORREF defaultColor;
	COLORREF identifierColor;
	COLORREF keywordColor;
	COLORREF functionsColor;
	COLORREF traceColor;
	COLORREF commentColor;
	COLORREF numberColor;
	COLORREF stringColor;
	COLORREF operatorColor;
	COLORREF backgroundColor;

	COLORREF caretColor;
	COLORREF selectionBgColor;
	COLORREF bookmarkFgColor;
	COLORREF bookmarkBgColor;
	COLORREF foldFgColor;
	COLORREF foldBgColor;
	COLORREF errorBgColor;

	RDOFontStyle defaultStyle;
	RDOFontStyle identifierStyle;
	RDOFontStyle keywordStyle;
	RDOFontStyle functionsStyle;
	RDOFontStyle traceStyle;
	RDOFontStyle commentStyle;
	RDOFontStyle numberStyle;
	RDOFontStyle stringStyle;
	RDOFontStyle operatorStyle;

	RDOBookmarkStyle bookmarkStyle;
	RDOFoldStyle     foldStyle;

	static RDOEditorEditTheme getDefaultTheme();
	static RDOEditorEditTheme getCppTheme();
	static RDOEditorEditTheme getPascalTheme();
	static RDOEditorEditTheme getHtmlTheme();
	static RDOEditorEditTheme getClassicTheme();
	static RDOEditorEditTheme getTwilightTheme();
	static RDOEditorEditTheme getOceanTheme();

	static CString colorToHEX( const COLORREF color );
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditFont
// ----------------------------------------------------------------------------
class RDOEditorEditFont
{
public:
	RDOEditorEditFont();
	virtual ~RDOEditorEditFont();

	RDOEditorEditFont& operator =( const RDOEditorEditFont& font );
	bool operator ==( const RDOEditorEditFont& font ) const;
	bool operator !=( const RDOEditorEditFont& font ) const;

	CString name;
	int     size;
	int     codepage;
	int     characterSet;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTab
// ----------------------------------------------------------------------------
class RDOEditorEditTab
{
public:
	RDOEditorEditTab();
	virtual ~RDOEditorEditTab();

	RDOEditorEditTab& operator =( const RDOEditorEditTab& tab );
	bool operator ==( const RDOEditorEditTab& tab ) const;
	bool operator !=( const RDOEditorEditTab& tab ) const;

	int tabSize;
	int indentSize;
	bool useTabs;
	bool tabIndents;
	bool backspaceUntabs;
	bool autoIndent;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditAutoComplete
// ----------------------------------------------------------------------------
class RDOEditorEditAutoComplete
{
public:
	RDOEditorEditAutoComplete();
	virtual ~RDOEditorEditAutoComplete();

	RDOEditorEditAutoComplete& operator =( const RDOEditorEditAutoComplete& autoComplete );
	bool operator ==( const RDOEditorEditAutoComplete& autoComplete ) const;
	bool operator !=( const RDOEditorEditAutoComplete& autoComplete ) const;

	bool useAutoComplete;
	bool showFullList;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditWindow
// ----------------------------------------------------------------------------
class RDOEditorEditWindow
{
public:
	RDOEditorEditWindow();
	virtual ~RDOEditorEditWindow();

	RDOEditorEditWindow& operator =( const RDOEditorEditWindow& window );
	bool operator ==( const RDOEditorEditWindow& window ) const;
	bool operator !=( const RDOEditorEditWindow& window ) const;

	bool wordWrap;
	bool showHorzScrollBar;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditStyle
// ----------------------------------------------------------------------------
class RDOEditorEditStyle
{
public:
	RDOEditorEditStyle();
	virtual ~RDOEditorEditStyle();

	RDOEditorEditStyle& operator =( const RDOEditorEditStyle& style );
	bool operator ==( const RDOEditorEditStyle& style ) const;
	bool operator !=( const RDOEditorEditStyle& style ) const;

	void load();
	void save() const;

	RDOEditorEditTheme theme;
	RDOEditorEditFont  font;
	RDOEditorEditTab   tab;
	RDOEditorEditAutoComplete autoComplete;
	RDOEditorEditWindow       window;
};

#endif // RDOEDITOREDITSTYLE_H
