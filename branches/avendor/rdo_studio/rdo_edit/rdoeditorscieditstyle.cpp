#include "stdafx.h"
#include "rdoeditorscieditstyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditTheme
// ----------------------------------------------------------------------------
RDOEditorSciEditTheme::RDOEditorSciEditTheme()
{
	defaultColor    = RGB( 0x80, 0x80, 0x80 );
	backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	caretColor       = RGB( 0x00, 0x00, 0x00 );
	selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	foldBgColor      = RGB( 0x00, 0x00, 0x00 );

	defaultStyle = RDOFS_NONE;

	bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	foldStyle     = RDOFOLDS_PLUS;
}

RDOEditorSciEditTheme::~RDOEditorSciEditTheme()
{
}

RDOEditorSciEditTheme& RDOEditorSciEditTheme::operator =( const RDOEditorSciEditTheme& theme )
{
	defaultColor    = theme.defaultColor;
	backgroundColor = theme.defaultColor;

	caretColor       = theme.caretColor;
	selectionBgColor = theme.selectionBgColor;
	bookmarkFgColor  = theme.bookmarkFgColor;
	bookmarkBgColor  = theme.bookmarkBgColor;
	foldFgColor      = theme.foldFgColor;
	foldBgColor      = theme.foldBgColor;

	defaultStyle = theme.defaultStyle;

	bookmarkStyle = theme.bookmarkStyle;
	foldStyle     = theme.foldStyle;

	return *this;
}

bool RDOEditorSciEditTheme::operator ==( const RDOEditorSciEditTheme& theme ) const
{
	return defaultColor    == theme.defaultColor &&
	       backgroundColor == theme.backgroundColor &&

	       caretColor       == theme.caretColor &&
	       selectionBgColor == theme.selectionBgColor &&
	       bookmarkFgColor  == theme.bookmarkFgColor &&
	       bookmarkBgColor  == theme.bookmarkBgColor &&
	       foldFgColor      == theme.foldFgColor &&
	       foldBgColor      == theme.foldBgColor &&

	       defaultStyle == theme.defaultStyle &&

	       bookmarkStyle == theme.bookmarkStyle &&
	       foldStyle     == theme.foldStyle;
}

bool RDOEditorSciEditTheme::operator !=( const RDOEditorSciEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOEditorSciEditTheme::load( string regPath )
{
	regPath += "theme";
	defaultColor     = AfxGetApp()->GetProfileInt( regPath.c_str(), "defaultColor", defaultColor );
	backgroundColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "backgroundColor", backgroundColor );
	caretColor       = AfxGetApp()->GetProfileInt( regPath.c_str(), "caretColor", caretColor );
	selectionBgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "selectionBgColor", selectionBgColor );
	bookmarkFgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "bookmarkFgColor", bookmarkFgColor );
	bookmarkBgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "bookmarkBgColor", bookmarkBgColor );
	foldFgColor      = AfxGetApp()->GetProfileInt( regPath.c_str(), "foldFgColor", foldFgColor );
	foldBgColor      = AfxGetApp()->GetProfileInt( regPath.c_str(), "foldBgColor", foldBgColor );
	defaultStyle     = (RDOFontStyle)AfxGetApp()->GetProfileInt( regPath.c_str(), "defaultStyle", defaultStyle );
	bookmarkStyle    = (RDOBookmarkStyle)AfxGetApp()->GetProfileInt( regPath.c_str(), "bookmarkStyle", bookmarkStyle );
	foldStyle        = (RDOFoldStyle)AfxGetApp()->GetProfileInt( regPath.c_str(), "foldStyle", foldStyle );
}

void RDOEditorSciEditTheme::save( string regPath ) const
{
	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "defaultColor", defaultColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "backgroundColor", backgroundColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "caretColor", caretColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "selectionBgColor", selectionBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "bookmarkFgColor", bookmarkFgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "bookmarkBgColor", bookmarkBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldFgColor", foldFgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldBgColor", foldBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "defaultStyle", defaultStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "bookmarkStyle", bookmarkStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldStyle", foldStyle );
}

bool RDOEditorSciEditTheme::styleDefault( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

bool RDOEditorSciEditTheme::styleUsing( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

bool RDOEditorSciEditTheme::styleBold( const int styleType ) const
{
	return defaultStyle & RDOFS_BOLD ? true : false;
}

bool RDOEditorSciEditTheme::styleItalic( const int styleType ) const
{
	return defaultStyle & RDOFS_ITALIC ? true : false;
}

CString RDOEditorSciEditTheme::styleFGColorToHEX( const int styleType ) const
{
	return colorToHEX( defaultColor );
}

CString RDOEditorSciEditTheme::styleBGColorToHEX( const int styleType ) const
{
	return colorToHEX( backgroundColor );
}

RDOEditorSciEditTheme RDOEditorSciEditTheme::getDefaultTheme()
{
	RDOEditorSciEditTheme theme;
	return theme;
}

CString RDOEditorSciEditTheme::colorToHEX( const COLORREF color )
{
	CString s;
	s.Format( "#%02X%02X%02X", GetRValue( color ), GetGValue( color ), GetBValue( color ) );
	return s;
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditFont
// ----------------------------------------------------------------------------
RDOEditorSciEditFont::RDOEditorSciEditFont()
{
	name         = "Courier New";
	size         = 10;
	codepage     = 0;
	characterSet = 1;
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		characterSet = RUSSIAN_CHARSET;
	}
}

RDOEditorSciEditFont::~RDOEditorSciEditFont()
{
}

RDOEditorSciEditFont& RDOEditorSciEditFont::operator =( const RDOEditorSciEditFont& font )
{
	name         = font.name;
	size         = font.size;
	codepage     = font.codepage;
	characterSet = font.characterSet;

	return *this;
}

bool RDOEditorSciEditFont::operator ==( const RDOEditorSciEditFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

bool RDOEditorSciEditFont::operator !=( const RDOEditorSciEditFont& font ) const
{
	return !(*this == font);
}

void RDOEditorSciEditFont::load( string regPath )
{
	regPath += "font";
	name         = AfxGetApp()->GetProfileString( regPath.c_str(), "name", name );
	size         = AfxGetApp()->GetProfileInt( regPath.c_str(), "size", size );
	codepage     = AfxGetApp()->GetProfileInt( regPath.c_str(), "codepage", codepage );
	characterSet = AfxGetApp()->GetProfileInt( regPath.c_str(), "characterSet", characterSet );
}

void RDOEditorSciEditFont::save( string regPath ) const
{
	regPath += "font";
	AfxGetApp()->WriteProfileString( regPath.c_str(), "name", name );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "size", size );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "codepage", codepage );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "characterSet", characterSet );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditTab
// ----------------------------------------------------------------------------
RDOEditorSciEditTab::RDOEditorSciEditTab()
{
	tabSize         = 4;
	indentSize      = 4;
	useTabs         = true;
	tabIndents      = true;
	backspaceUntabs = false;
	autoIndent      = true;
}

RDOEditorSciEditTab::~RDOEditorSciEditTab()
{
}

RDOEditorSciEditTab& RDOEditorSciEditTab::operator =( const RDOEditorSciEditTab& tab )
{
	tabSize         = tab.tabSize;
	indentSize      = tab.indentSize;
	useTabs         = tab.useTabs;
	tabIndents      = tab.tabIndents;
	backspaceUntabs = tab.backspaceUntabs;
	autoIndent      = tab.autoIndent;

	return *this;
}

bool RDOEditorSciEditTab::operator ==( const RDOEditorSciEditTab& tab ) const
{
	return tabSize         == tab.tabSize &&
	       indentSize      == tab.indentSize &&
	       useTabs         == tab.useTabs &&
	       tabIndents      == tab.tabIndents &&
	       backspaceUntabs == tab.backspaceUntabs &&
	       autoIndent      == tab.autoIndent;
}

bool RDOEditorSciEditTab::operator !=( const RDOEditorSciEditTab& tab ) const
{
	return !(*this == tab);
}

void RDOEditorSciEditTab::load( string regPath )
{
	regPath += "tab";
	tabSize         = AfxGetApp()->GetProfileInt( regPath.c_str(), "tabSize", tabSize );
	indentSize      = AfxGetApp()->GetProfileInt( regPath.c_str(), "indentSize", indentSize );
	useTabs         = AfxGetApp()->GetProfileInt( regPath.c_str(), "useTabs", useTabs ) ? true : false;
	tabIndents      = AfxGetApp()->GetProfileInt( regPath.c_str(), "tabIndents", tabIndents ) ? true : false;
	backspaceUntabs = AfxGetApp()->GetProfileInt( regPath.c_str(), "backspaceUntabs", backspaceUntabs ) ? true : false;
	autoIndent      = AfxGetApp()->GetProfileInt( regPath.c_str(), "autoIndent", autoIndent ) ? true : false;
}

void RDOEditorSciEditTab::save( string regPath ) const
{
	regPath += "tab";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "tabSize", tabSize );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "indentSize", indentSize );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "useTabs", useTabs );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "tabIndents", tabIndents );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "backspaceUntabs", backspaceUntabs );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "autoIndent", autoIndent );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditWindow
// ----------------------------------------------------------------------------
RDOEditorSciEditWindow::RDOEditorSciEditWindow()
{
	wordWrap          = false;
	showHorzScrollBar = true;
}

RDOEditorSciEditWindow::~RDOEditorSciEditWindow()
{
}

RDOEditorSciEditWindow& RDOEditorSciEditWindow::operator =( const RDOEditorSciEditWindow& window )
{
	wordWrap          = window.wordWrap;
	showHorzScrollBar = window.showHorzScrollBar;

	return *this;
}

bool RDOEditorSciEditWindow::operator ==( const RDOEditorSciEditWindow& window ) const
{
	return wordWrap          == window.wordWrap&&
	       showHorzScrollBar == window.showHorzScrollBar;
}

bool RDOEditorSciEditWindow::operator !=( const RDOEditorSciEditWindow& window ) const
{
	return !(*this == window);
}

void RDOEditorSciEditWindow::load( string regPath )
{
	regPath += "window";
	wordWrap          = AfxGetApp()->GetProfileInt( regPath.c_str(), "wordWrap", wordWrap ) ? true : false;
	showHorzScrollBar = AfxGetApp()->GetProfileInt( regPath.c_str(), "showHorzScrollBar", showHorzScrollBar ) ? true : false;
}

void RDOEditorSciEditWindow::save( string regPath ) const
{
	regPath += "window";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "wordWrap", wordWrap );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "showHorzScrollBar", showHorzScrollBar );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEditStyle
// ----------------------------------------------------------------------------
RDOEditorSciEditStyle::RDOEditorSciEditStyle():
	regPath( "" ),
	theme( NULL ),
	font( NULL ),
	tab( NULL ),
	window( NULL )
{
}

RDOEditorSciEditStyle::~RDOEditorSciEditStyle()
{
	if ( theme )  { delete theme;  theme = NULL; };
	if ( font )   { delete font;   font = NULL; };
	if ( tab )    { delete tab;    tab = NULL; };
	if ( window ) { delete window; window = NULL; };
}

void RDOEditorSciEditStyle::initTheme()
{
	theme = new RDOEditorSciEditTheme;
}

void RDOEditorSciEditStyle::initFont()
{
	font = new RDOEditorSciEditFont;
}

void RDOEditorSciEditStyle::initTab()
{
	tab = new RDOEditorSciEditTab;
}

void RDOEditorSciEditStyle::initWindow()
{
	window = new RDOEditorSciEditWindow;
}

RDOEditorSciEditStyle& RDOEditorSciEditStyle::operator =( const RDOEditorSciEditStyle& style )
{
	if ( theme  && style.theme )  *theme  = *style.theme;
	if ( font   && style.font )   *font   = *style.font;
	if ( tab    && style.tab )    *tab    = *style.tab;
	if ( window && style.window ) *window = *style.window;

	return *this;
}

bool RDOEditorSciEditStyle::operator ==( const RDOEditorSciEditStyle& style ) const
{
	bool flag = true;
	if ( theme  && style.theme  && flag ) flag &= *theme  == *style.theme;
	if ( font   && style.font   && flag ) flag &= *font   == *style.font;
	if ( tab    && style.tab    && flag ) flag &= *tab    == *style.tab;
	if ( window && style.window && flag ) flag &= *window == *style.window;
	return flag;
}

bool RDOEditorSciEditStyle::operator !=( const RDOEditorSciEditStyle& style ) const
{
	return !(*this == style);
}

void RDOEditorSciEditStyle::init( const string& _regPath )
{
	regPath = _regPath;
	if ( regPath.empty() ) {
		regPath = "editStyle\\";
	}
	if ( regPath.find_last_of( '\\' ) != regPath.length() - 1 ) {
		regPath += '\\';
	}
	initTheme();
	initFont();
	initTab();
	initWindow();
}

void RDOEditorSciEditStyle::load()
{
	if ( theme )  theme->load( regPath );
	if ( font )   font->load( regPath );
	if ( tab )    tab->load( regPath );
	if ( window ) window->load( regPath );
}

void RDOEditorSciEditStyle::save() const
{
	if ( theme )  theme->save( regPath );
	if ( font )   font->save( regPath );
	if ( tab )    tab->save( regPath );
	if ( window ) window->save( regPath );
}
