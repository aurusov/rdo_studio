#include "stdafx.h"
#include "rdoeditoreditstyle.h"
#include "rdoeditorapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTheme
// ----------------------------------------------------------------------------
RDOEditorEditTheme::RDOEditorEditTheme()
{
	defaultColor    = RGB( 0x80, 0x80, 0x80 );
	identifierColor = RGB( 0x00, 0x00, 0x00 );
	keywordColor    = RGB( 0x00, 0x00, 0x00 );
	functionsColor  = RGB( 0x00, 0x80, 0x00 );
	traceColor      = RGB( 0x80, 0x00, 0x00 );
	commentColor    = RGB( 0x00, 0x80, 0x00 );
	numberColor     = RGB( 0x00, 0x00, 0x80 );
	stringColor     = RGB( 0x80, 0x00, 0x80 );
	operatorColor   = RGB( 0x00, 0x00, 0x00 );
	backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	caretColor       = RGB( 0x00, 0x00, 0x00 );
	selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	defaultStyle    = RDOFS_NONE;
	identifierStyle = RDOFS_NONE;
	keywordStyle    = RDOFS_BOLD;
	functionsStyle  = RDOFS_BOLD;
	traceStyle      = RDOFS_BOLD;
	commentStyle    = RDOFS_ITALIC;
	numberStyle     = RDOFS_NONE;
	stringStyle     = RDOFS_NONE;
	operatorStyle   = RDOFS_NONE;

	bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	foldStyle     = RDOFOLDS_PLUS;
}

RDOEditorEditTheme::~RDOEditorEditTheme()
{
}

RDOEditorEditTheme& RDOEditorEditTheme::operator =( const RDOEditorEditTheme& theme )
{
	defaultColor    = theme.defaultColor;
	identifierColor = theme.identifierColor;
	keywordColor    = theme.keywordColor;
	functionsColor  = theme.functionsColor;
	traceColor      = theme.traceColor;
	commentColor    = theme.commentColor;
	numberColor     = theme.numberColor;
	stringColor     = theme.stringColor;
	operatorColor   = theme.operatorColor;
	backgroundColor = theme.backgroundColor;

	caretColor       = theme.caretColor;
	selectionBgColor = theme.selectionBgColor;
	bookmarkFgColor  = theme.bookmarkFgColor;
	bookmarkBgColor  = theme.bookmarkBgColor;
	foldFgColor      = theme.foldFgColor;
	foldBgColor      = theme.foldBgColor;
	errorBgColor     = theme.errorBgColor;

	defaultStyle    = theme.defaultStyle;
	identifierStyle = theme.identifierStyle;
	keywordStyle    = theme.keywordStyle;
	functionsStyle  = theme.functionsStyle;
	traceStyle      = theme.traceStyle;
	commentStyle    = theme.commentStyle;
	numberStyle     = theme.numberStyle;
	stringStyle     = theme.stringStyle;
	operatorStyle   = theme.operatorStyle;

	bookmarkStyle = theme.bookmarkStyle;
	foldStyle     = theme.foldStyle;

	return *this;
}

bool RDOEditorEditTheme::operator ==( const RDOEditorEditTheme& theme ) const
{
	return defaultColor    == theme.defaultColor &&
	       identifierColor == theme.identifierColor &&
	       keywordColor    == theme.keywordColor &&
	       functionsColor  == theme.functionsColor &&
	       traceColor      == theme.traceColor &&
	       commentColor    == theme.commentColor &&
	       numberColor     == theme.numberColor &&
	       stringColor     == theme.stringColor &&
	       operatorColor   == theme.operatorColor &&
	       backgroundColor == theme.backgroundColor &&

	       caretColor       == theme.caretColor &&
	       selectionBgColor == theme.selectionBgColor &&
	       bookmarkFgColor  == theme.bookmarkFgColor &&
	       bookmarkBgColor  == theme.bookmarkBgColor &&
	       foldFgColor      == theme.foldFgColor &&
	       foldBgColor      == theme.foldBgColor &&
	       errorBgColor     == theme.errorBgColor &&

	       defaultStyle    == theme.defaultStyle &&
	       identifierStyle == theme.identifierStyle &&
	       keywordStyle    == theme.keywordStyle &&
	       functionsStyle  == theme.functionsStyle &&
	       traceStyle      == theme.traceStyle &&
	       commentStyle    == theme.commentStyle &&
	       numberStyle     == theme.numberStyle &&
	       stringStyle     == theme.stringStyle &&
	       operatorStyle   == theme.operatorStyle &&

	       bookmarkStyle == theme.bookmarkStyle &&
	       foldStyle     == theme.foldStyle;
}

bool RDOEditorEditTheme::operator !=( const RDOEditorEditTheme& theme ) const
{
	return !(*this == theme);
}

RDOEditorEditTheme RDOEditorEditTheme::getDefaultTheme()
{
	RDOEditorEditTheme theme;
	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getCppTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x00, 0x00, 0xFF );
	theme.functionsColor  = RGB( 0x00, 0x00, 0xFF );
	theme.traceColor      = RGB( 0x00, 0x00, 0xFF );
	theme.commentColor    = RGB( 0x00, 0x80, 0x00 );
	theme.numberColor     = RGB( 0x00, 0x00, 0x00 );
	theme.stringColor     = RGB( 0x00, 0x00, 0x00 );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getPascalTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x00, 0x00, 0x00 );
	theme.functionsColor  = RGB( 0x00, 0x00, 0x00 );
	theme.traceColor      = RGB( 0x00, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x00, 0x80 );
	theme.numberColor     = RGB( 0x00, 0x00, 0x00 );
	theme.stringColor     = RGB( 0x00, 0x00, 0x00 );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = RDOFS_BOLD;
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getHtmlTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x80, 0x00, 0x80 );
	theme.functionsColor  = RGB( 0xFF, 0x00, 0x00 );
	theme.traceColor      = RGB( 0xFF, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x80, 0x00 );
	theme.numberColor     = RGB( 0x00, 0x00, 0xFF );
	theme.stringColor     = RGB( 0x00, 0x00, 0xFF );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getClassicTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0xFF, 0xFF, 0x00 );
	theme.identifierColor = RGB( 0xFF, 0xFF, 0x00 );
	theme.keywordColor    = RGB( 0xFF, 0xFF, 0xFF );
	theme.functionsColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.traceColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.commentColor    = RGB( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = RGB( 0xFF, 0xFF, 0x00 );
	theme.stringColor     = RGB( 0xFF, 0xFF, 0x00 );
	theme.operatorColor   = RGB( 0xFF, 0xFF, 0x00 );
	theme.backgroundColor = RGB( 0x00, 0x00, 0x80 );

	theme.caretColor       = RGB( 0xFF, 0xFF, 0x00 );
	theme.selectionBgColor = RGB( 0x00, 0x00, 0x40 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x00, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getTwilightTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0xFF, 0xFF, 0xFF );
	theme.identifierColor = RGB( 0xFF, 0xFF, 0xFF );
	theme.keywordColor    = RGB( 0x00, 0xFF, 0xFF );
	theme.functionsColor  = RGB( 0xFF, 0xFF, 0x00 );
	theme.traceColor      = RGB( 0x00, 0xFF, 0x00 );
	theme.commentColor    = RGB( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = RGB( 0xC0, 0xC0, 0xC0 );
	theme.stringColor     = RGB( 0xFF, 0xFF, 0xFF );
	theme.operatorColor   = RGB( 0xFF, 0xFF, 0xFF );
	theme.backgroundColor = RGB( 0x00, 0x00, 0x00 );

	theme.caretColor       = RGB( 0xFF, 0xFF, 0xFF );
	theme.selectionBgColor = RGB( 0x70, 0x70, 0x70 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = (RDOFontStyle)(RDOFS_BOLD | RDOFS_ITALIC);
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getOceanTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0xFF );
	theme.identifierColor = RGB( 0x00, 0x00, 0xFF );
	theme.keywordColor    = RGB( 0x00, 0x00, 0x00 );
	theme.functionsColor  = RGB( 0x00, 0x00, 0x00 );
	theme.traceColor      = RGB( 0x00, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x80, 0x80 );
	theme.numberColor     = RGB( 0x00, 0x00, 0xFF );
	theme.stringColor     = RGB( 0x00, 0x00, 0xFF );
	theme.operatorColor   = RGB( 0x00, 0x00, 0xFF );
	theme.backgroundColor = RGB( 0x00, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xD0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0xFF, 0x00 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = RDOFS_BOLD;
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

CString RDOEditorEditTheme::colorToHEX( const COLORREF color )
{
	CString s;
	s.Format( "#%02X%02X%02X", GetRValue( color ), GetGValue( color ), GetBValue( color ) );
	return s;
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditFont
// ----------------------------------------------------------------------------
RDOEditorEditFont::RDOEditorEditFont()
{
	name         = "Courier New";
	size         = 10;
	codepage     = 0;
	characterSet = 1;
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		characterSet = RUSSIAN_CHARSET;
	}
}

RDOEditorEditFont::~RDOEditorEditFont()
{
}

RDOEditorEditFont& RDOEditorEditFont::operator =( const RDOEditorEditFont& font )
{
	name         = font.name;
	size         = font.size;
	codepage     = font.codepage;
	characterSet = font.characterSet;

	return *this;
}

bool RDOEditorEditFont::operator ==( const RDOEditorEditFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

bool RDOEditorEditFont::operator !=( const RDOEditorEditFont& font ) const
{
	return !(*this == font);
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTab
// ----------------------------------------------------------------------------
RDOEditorEditTab::RDOEditorEditTab()
{
	tabSize         = 4;
	indentSize      = 4;
	useTabs         = true;
	tabIndents      = true;
	backspaceUntabs = false;
	autoIndent      = true;
}

RDOEditorEditTab::~RDOEditorEditTab()
{
}

RDOEditorEditTab& RDOEditorEditTab::operator =( const RDOEditorEditTab& tab )
{
	tabSize         = tab.tabSize;
	indentSize      = tab.indentSize;
	useTabs         = tab.useTabs;
	tabIndents      = tab.tabIndents;
	backspaceUntabs = tab.backspaceUntabs;
	autoIndent      = tab.autoIndent;

	return *this;
}

bool RDOEditorEditTab::operator ==( const RDOEditorEditTab& tab ) const
{
	return tabSize         == tab.tabSize &&
	       indentSize      == tab.indentSize &&
	       useTabs         == tab.useTabs &&
	       tabIndents      == tab.tabIndents &&
	       backspaceUntabs == tab.backspaceUntabs &&
	       autoIndent      == tab.autoIndent;
}

bool RDOEditorEditTab::operator !=( const RDOEditorEditTab& tab ) const
{
	return !(*this == tab);
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditAutoComplete
// ----------------------------------------------------------------------------
RDOEditorEditAutoComplete::RDOEditorEditAutoComplete()
{
	useAutoComplete = true;
	showFullList    = true;
}

RDOEditorEditAutoComplete::~RDOEditorEditAutoComplete()
{
}

RDOEditorEditAutoComplete& RDOEditorEditAutoComplete::operator =( const RDOEditorEditAutoComplete& autoComplete )
{
	useAutoComplete = autoComplete.useAutoComplete;
	showFullList    = autoComplete.showFullList;

	return *this;
}

bool RDOEditorEditAutoComplete::operator ==( const RDOEditorEditAutoComplete& autoComplete ) const
{
	return useAutoComplete == autoComplete.useAutoComplete &&
	       showFullList    == autoComplete.showFullList;
}

bool RDOEditorEditAutoComplete::operator !=( const RDOEditorEditAutoComplete& autoComplete ) const
{
	return !(*this == autoComplete);
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditWindow
// ----------------------------------------------------------------------------
RDOEditorEditWindow::RDOEditorEditWindow()
{
	wordWrap          = false;
	showHorzScrollBar = true;
}

RDOEditorEditWindow::~RDOEditorEditWindow()
{
}

RDOEditorEditWindow& RDOEditorEditWindow::operator =( const RDOEditorEditWindow& window )
{
	wordWrap          = window.wordWrap;
	showHorzScrollBar = window.showHorzScrollBar;

	return *this;
}

bool RDOEditorEditWindow::operator ==( const RDOEditorEditWindow& window ) const
{
	return wordWrap          == window.wordWrap&&
	       showHorzScrollBar == window.showHorzScrollBar;
}

bool RDOEditorEditWindow::operator !=( const RDOEditorEditWindow& window ) const
{
	return !(*this == window);
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditStyle
// ----------------------------------------------------------------------------
RDOEditorEditStyle::RDOEditorEditStyle()
{
}

RDOEditorEditStyle::~RDOEditorEditStyle()
{
}

RDOEditorEditStyle& RDOEditorEditStyle::operator =( const RDOEditorEditStyle& style )
{
	theme        = style.theme;
	font         = style.font;
	tab          = style.tab;
	autoComplete = style.autoComplete;
	window       = style.window;

	return *this;
}

bool RDOEditorEditStyle::operator ==( const RDOEditorEditStyle& style ) const
{
	return theme        == style.theme &&
	       font         == style.font &&
	       tab          == style.tab &&
	       autoComplete == style.autoComplete &&
	       window       == style.window;
}

bool RDOEditorEditStyle::operator !=( const RDOEditorEditStyle& style ) const
{
	return !(*this == style);
}

void RDOEditorEditStyle::load()
{
	theme.defaultColor           = rdoEditorApp.GetProfileInt( "style\\theme", "defaultColor", theme.defaultColor );
	theme.identifierColor        = rdoEditorApp.GetProfileInt( "style\\theme", "identifierColor", theme.identifierColor );
	theme.keywordColor           = rdoEditorApp.GetProfileInt( "style\\theme", "keywordColor", theme.keywordColor );
	theme.functionsColor         = rdoEditorApp.GetProfileInt( "style\\theme", "functionsColor", theme.functionsColor );
	theme.traceColor             = rdoEditorApp.GetProfileInt( "style\\theme", "traceColor", theme.traceColor );
	theme.commentColor           = rdoEditorApp.GetProfileInt( "style\\theme", "commentColor", theme.commentColor );
	theme.numberColor            = rdoEditorApp.GetProfileInt( "style\\theme", "numberColor", theme.numberColor );
	theme.stringColor            = rdoEditorApp.GetProfileInt( "style\\theme", "stringColor", theme.stringColor );
	theme.operatorColor          = rdoEditorApp.GetProfileInt( "style\\theme", "operatorColor", theme.operatorColor );
	theme.backgroundColor        = rdoEditorApp.GetProfileInt( "style\\theme", "backgroundColor", theme.backgroundColor );
	theme.caretColor             = rdoEditorApp.GetProfileInt( "style\\theme", "caretColor", theme.caretColor );
	theme.selectionBgColor       = rdoEditorApp.GetProfileInt( "style\\theme", "selectionBgColor", theme.selectionBgColor );
	theme.bookmarkFgColor        = rdoEditorApp.GetProfileInt( "style\\theme", "bookmarkFgColor", theme.bookmarkFgColor );
	theme.bookmarkBgColor        = rdoEditorApp.GetProfileInt( "style\\theme", "bookmarkBgColor", theme.bookmarkBgColor );
	theme.foldFgColor            = rdoEditorApp.GetProfileInt( "style\\theme", "foldFgColor", theme.foldFgColor );
	theme.foldBgColor            = rdoEditorApp.GetProfileInt( "style\\theme", "foldBgColor", theme.foldBgColor );
	theme.errorBgColor           = rdoEditorApp.GetProfileInt( "style\\theme", "errorBgColor", theme.errorBgColor );
	theme.defaultStyle           = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "defaultStyle", theme.defaultStyle );
	theme.identifierStyle        = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "identifierStyle", theme.identifierStyle );
	theme.keywordStyle           = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "keywordStyle", theme.keywordStyle );
	theme.functionsStyle         = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "functionsStyle", theme.functionsStyle );
	theme.traceStyle             = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "traceStyle", theme.traceStyle );
	theme.commentStyle           = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "commentStyle", theme.commentStyle );
	theme.numberStyle            = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "numberStyle", theme.numberStyle );
	theme.stringStyle            = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "stringStyle", theme.stringStyle );
	theme.operatorStyle          = (RDOFontStyle)rdoEditorApp.GetProfileInt( "style\\theme", "operatorStyle", theme.operatorStyle );
	theme.bookmarkStyle          = (RDOBookmarkStyle)rdoEditorApp.GetProfileInt( "style\\theme", "bookmarkStyle", theme.bookmarkStyle );
	theme.foldStyle              = (RDOFoldStyle)rdoEditorApp.GetProfileInt( "style\\theme", "foldStyle", theme.foldStyle );
	font.name                    = rdoEditorApp.GetProfileString( "style\\font", "name", font.name );
	font.size                    = rdoEditorApp.GetProfileInt( "style\\font", "size", font.size );
	font.codepage                = rdoEditorApp.GetProfileInt( "style\\font", "codepage", font.codepage );
	font.characterSet            = rdoEditorApp.GetProfileInt( "style\\font", "characterSet", font.characterSet );
	tab.tabSize                  = rdoEditorApp.GetProfileInt( "style\\tab", "tabSize", tab.tabSize );
	tab.indentSize               = rdoEditorApp.GetProfileInt( "style\\tab", "indentSize", tab.indentSize );
	tab.useTabs                  = rdoEditorApp.GetProfileInt( "style\\tab", "useTabs", tab.useTabs ) ? true : false;
	tab.tabIndents               = rdoEditorApp.GetProfileInt( "style\\tab", "tabIndents", tab.tabIndents ) ? true : false;
	tab.backspaceUntabs          = rdoEditorApp.GetProfileInt( "style\\tab", "backspaceUntabs", tab.backspaceUntabs ) ? true : false;
	tab.autoIndent               = rdoEditorApp.GetProfileInt( "style\\tab", "autoIndent", tab.autoIndent ) ? true : false;
	autoComplete.useAutoComplete = rdoEditorApp.GetProfileInt( "style\\autoComplete", "useAutoComplete", autoComplete.useAutoComplete ) ? true : false;
	autoComplete.showFullList    = rdoEditorApp.GetProfileInt( "style\\autoComplete", "showFullList", autoComplete.showFullList ) ? true : false;
	window.wordWrap              = rdoEditorApp.GetProfileInt( "style\\window", "wordWrap", window.wordWrap ) ? true : false;
	window.showHorzScrollBar     = rdoEditorApp.GetProfileInt( "style\\window", "showHorzScrollBar", window.showHorzScrollBar ) ? true : false;
}

void RDOEditorEditStyle::save() const
{
	rdoEditorApp.WriteProfileInt( "style\\theme", "defaultColor", theme.defaultColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "identifierColor", theme.identifierColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "keywordColor", theme.keywordColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "functionsColor", theme.functionsColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "traceColor", theme.traceColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "commentColor", theme.commentColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "numberColor", theme.numberColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "stringColor", theme.stringColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "operatorColor", theme.operatorColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "backgroundColor", theme.backgroundColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "caretColor", theme.caretColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "selectionBgColor", theme.selectionBgColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "bookmarkFgColor", theme.bookmarkFgColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "bookmarkBgColor", theme.bookmarkBgColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "foldFgColor", theme.foldFgColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "foldBgColor", theme.foldBgColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "errorBgColor", theme.errorBgColor );
	rdoEditorApp.WriteProfileInt( "style\\theme", "defaultStyle", theme.defaultStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "identifierStyle", theme.identifierStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "keywordStyle", theme.keywordStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "functionsStyle", theme.functionsStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "traceStyle", theme.traceStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "commentStyle", theme.commentStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "numberStyle", theme.numberStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "stringStyle", theme.stringStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "operatorStyle", theme.operatorStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "bookmarkStyle", theme.bookmarkStyle );
	rdoEditorApp.WriteProfileInt( "style\\theme", "foldStyle", theme.foldStyle );
	rdoEditorApp.WriteProfileString( "style\\font", "name", font.name );
	rdoEditorApp.WriteProfileInt( "style\\font", "size", font.size );
	rdoEditorApp.WriteProfileInt( "style\\font", "codepage", font.codepage );
	rdoEditorApp.WriteProfileInt( "style\\font", "characterSet", font.characterSet );
	rdoEditorApp.WriteProfileInt( "style\\tab", "tabSize", tab.tabSize );
	rdoEditorApp.WriteProfileInt( "style\\tab", "indentSize", tab.indentSize );
	rdoEditorApp.WriteProfileInt( "style\\tab", "useTabs", tab.useTabs );
	rdoEditorApp.WriteProfileInt( "style\\tab", "tabIndents", tab.tabIndents );
	rdoEditorApp.WriteProfileInt( "style\\tab", "backspaceUntabs", tab.backspaceUntabs );
	rdoEditorApp.WriteProfileInt( "style\\tab", "autoIndent", tab.autoIndent );
	rdoEditorApp.WriteProfileInt( "style\\autoComplete", "useAutoComplete", autoComplete.useAutoComplete );
	rdoEditorApp.WriteProfileInt( "style\\autoComplete", "showFullList", autoComplete.showFullList );
	rdoEditorApp.WriteProfileInt( "style\\window", "wordWrap", window.wordWrap );
	rdoEditorApp.WriteProfileInt( "style\\window", "showHorzScrollBar", window.showHorzScrollBar );
}
