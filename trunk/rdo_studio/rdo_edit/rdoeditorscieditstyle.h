#ifndef RDOEDITORSCIEDITSTYLE_H
#define RDOEDITORSCIEDITSTYLE_H
#pragma once

#include "sci/Scintilla.h"

using namespace std;

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditTheme
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

class RDOEditorSciEditTheme
{
public:
	RDOEditorSciEditTheme();
	virtual ~RDOEditorSciEditTheme();

	RDOEditorSciEditTheme& operator =( const RDOEditorSciEditTheme& theme );
	bool operator ==( const RDOEditorSciEditTheme& theme ) const;
	bool operator !=( const RDOEditorSciEditTheme& theme ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	COLORREF defaultColor;
	COLORREF backgroundColor;

	COLORREF caretColor;
	COLORREF selectionBgColor;
	COLORREF bookmarkFgColor;
	COLORREF bookmarkBgColor;
	COLORREF foldFgColor;
	COLORREF foldBgColor;

	RDOFontStyle defaultStyle;

	RDOBookmarkStyle bookmarkStyle;
	RDOFoldStyle     foldStyle;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual string styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;
	virtual string styleBGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOEditorSciEditTheme getDefaultTheme();
	static string colorToHEX( const COLORREF color );
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditFont
// ----------------------------------------------------------------------------
class RDOEditorSciEditFont
{
public:
	RDOEditorSciEditFont();
	virtual ~RDOEditorSciEditFont();

	RDOEditorSciEditFont& operator =( const RDOEditorSciEditFont& font );
	bool operator ==( const RDOEditorSciEditFont& font ) const;
	bool operator !=( const RDOEditorSciEditFont& font ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	string name;
	int    size;
	int    codepage;
	int    characterSet;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditTab
// ----------------------------------------------------------------------------
class RDOEditorSciEditTab
{
public:
	RDOEditorSciEditTab();
	virtual ~RDOEditorSciEditTab();

	RDOEditorSciEditTab& operator =( const RDOEditorSciEditTab& tab );
	bool operator ==( const RDOEditorSciEditTab& tab ) const;
	bool operator !=( const RDOEditorSciEditTab& tab ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	int tabSize;
	int indentSize;
	bool useTabs;
	bool tabIndents;
	bool backspaceUntabs;
	bool autoIndent;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditWindow
// ----------------------------------------------------------------------------
class RDOEditorSciEditWindow
{
public:
	RDOEditorSciEditWindow();
	virtual ~RDOEditorSciEditWindow();

	RDOEditorSciEditWindow& operator =( const RDOEditorSciEditWindow& window );
	bool operator ==( const RDOEditorSciEditWindow& window ) const;
	bool operator !=( const RDOEditorSciEditWindow& window ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	bool wordWrap;
	bool showHorzScrollBar;
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditStyle
// ----------------------------------------------------------------------------
class RDOEditorSciEditStyle
{
protected:
	string regPath;

	virtual void initTheme();
	virtual void initFont();
	virtual void initTab();
	virtual void initWindow();

public:
	RDOEditorSciEditStyle();
	virtual ~RDOEditorSciEditStyle();

	RDOEditorSciEditStyle& operator =( const RDOEditorSciEditStyle& style );
	bool operator ==( const RDOEditorSciEditStyle& style ) const;
	bool operator !=( const RDOEditorSciEditStyle& style ) const;

	virtual void init( const string& _regPath = "" );
	virtual void load();
	virtual void save() const;

	RDOEditorSciEditTheme*  theme;
	RDOEditorSciEditFont*   font;
	RDOEditorSciEditTab*    tab;
	RDOEditorSciEditWindow* window;
};

}; // namespace rdoEditor

#endif // RDOEDITORSCIEDITSTYLE_H
