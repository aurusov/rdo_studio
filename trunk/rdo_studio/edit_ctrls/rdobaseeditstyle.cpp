#include "stdafx.h"
#include "rdobaseeditstyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoBaseEdit;

// ----------------------------------------------------------------------------
// ---------- RDOBaseEditTheme
// ----------------------------------------------------------------------------
RDOBaseEditTheme::RDOBaseEditTheme()
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

RDOBaseEditTheme::~RDOBaseEditTheme()
{
}

RDOBaseEditTheme& RDOBaseEditTheme::operator =( const RDOBaseEditTheme& theme )
{
	defaultColor    = theme.defaultColor;
	backgroundColor = theme.backgroundColor;

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

bool RDOBaseEditTheme::operator ==( const RDOBaseEditTheme& theme ) const
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

bool RDOBaseEditTheme::operator !=( const RDOBaseEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOBaseEditTheme::load( string regPath )
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

void RDOBaseEditTheme::save( string regPath ) const
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

bool RDOBaseEditTheme::styleDefault( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

bool RDOBaseEditTheme::styleUsing( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

bool RDOBaseEditTheme::styleBold( const int styleType ) const
{
	return defaultStyle & RDOFS_BOLD ? true : false;
}

bool RDOBaseEditTheme::styleItalic( const int styleType ) const
{
	return defaultStyle & RDOFS_ITALIC ? true : false;
}

string RDOBaseEditTheme::styleFGColorToHEX( const int styleType ) const
{
	return colorToHEX( defaultColor );
}

string RDOBaseEditTheme::styleBGColorToHEX( const int styleType ) const
{
	return colorToHEX( backgroundColor );
}

RDOBaseEditTheme RDOBaseEditTheme::getDefaultTheme()
{
	RDOBaseEditTheme theme;
	return theme;
}

string RDOBaseEditTheme::colorToHEX( const COLORREF color )
{
	return format( "#%02X%02X%02X", GetRValue( color ), GetGValue( color ), GetBValue( color ) );
}

// ----------------------------------------------------------------------------
// ---------- RDOBaseEditFont
// ----------------------------------------------------------------------------
RDOBaseEditFont::RDOBaseEditFont()
{
	name         = "Courier New";
	size         = 10;
	codepage     = 0;
	characterSet = 1;
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		characterSet = RUSSIAN_CHARSET;
	}
}

RDOBaseEditFont::~RDOBaseEditFont()
{
}

RDOBaseEditFont& RDOBaseEditFont::operator =( const RDOBaseEditFont& font )
{
	name         = font.name;
	size         = font.size;
	codepage     = font.codepage;
	characterSet = font.characterSet;

	return *this;
}

bool RDOBaseEditFont::operator ==( const RDOBaseEditFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

bool RDOBaseEditFont::operator !=( const RDOBaseEditFont& font ) const
{
	return !(*this == font);
}

void RDOBaseEditFont::load( string regPath )
{
	regPath += "font";
	name         = AfxGetApp()->GetProfileString( regPath.c_str(), "name", name.c_str() );
	size         = AfxGetApp()->GetProfileInt( regPath.c_str(), "size", size );
	codepage     = AfxGetApp()->GetProfileInt( regPath.c_str(), "codepage", codepage );
	characterSet = AfxGetApp()->GetProfileInt( regPath.c_str(), "characterSet", characterSet );
}

void RDOBaseEditFont::save( string regPath ) const
{
	regPath += "font";
	AfxGetApp()->WriteProfileString( regPath.c_str(), "name", name.c_str() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "size", size );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "codepage", codepage );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "characterSet", characterSet );
}

// ----------------------------------------------------------------------------
// ---------- RDOBaseEditTab
// ----------------------------------------------------------------------------
RDOBaseEditTab::RDOBaseEditTab()
{
	tabSize         = 4;
	indentSize      = 4;
	useTabs         = true;
	tabIndents      = true;
	backspaceUntabs = false;
	autoIndent      = true;
}

RDOBaseEditTab::~RDOBaseEditTab()
{
}

RDOBaseEditTab& RDOBaseEditTab::operator =( const RDOBaseEditTab& tab )
{
	tabSize         = tab.tabSize;
	indentSize      = tab.indentSize;
	useTabs         = tab.useTabs;
	tabIndents      = tab.tabIndents;
	backspaceUntabs = tab.backspaceUntabs;
	autoIndent      = tab.autoIndent;

	return *this;
}

bool RDOBaseEditTab::operator ==( const RDOBaseEditTab& tab ) const
{
	return tabSize         == tab.tabSize &&
	       indentSize      == tab.indentSize &&
	       useTabs         == tab.useTabs &&
	       tabIndents      == tab.tabIndents &&
	       backspaceUntabs == tab.backspaceUntabs &&
	       autoIndent      == tab.autoIndent;
}

bool RDOBaseEditTab::operator !=( const RDOBaseEditTab& tab ) const
{
	return !(*this == tab);
}

void RDOBaseEditTab::load( string regPath )
{
	regPath += "tab";
	tabSize         = AfxGetApp()->GetProfileInt( regPath.c_str(), "tabSize", tabSize );
	indentSize      = AfxGetApp()->GetProfileInt( regPath.c_str(), "indentSize", indentSize );
	useTabs         = AfxGetApp()->GetProfileInt( regPath.c_str(), "useTabs", useTabs ) ? true : false;
	tabIndents      = AfxGetApp()->GetProfileInt( regPath.c_str(), "tabIndents", tabIndents ) ? true : false;
	backspaceUntabs = AfxGetApp()->GetProfileInt( regPath.c_str(), "backspaceUntabs", backspaceUntabs ) ? true : false;
	autoIndent      = AfxGetApp()->GetProfileInt( regPath.c_str(), "autoIndent", autoIndent ) ? true : false;
}

void RDOBaseEditTab::save( string regPath ) const
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
// ---------- RDOBaseEditWindow
// ----------------------------------------------------------------------------
RDOBaseEditWindow::RDOBaseEditWindow()
{
	wordWrap          = false;
	showHorzScrollBar = true;
}

RDOBaseEditWindow::~RDOBaseEditWindow()
{
}

RDOBaseEditWindow& RDOBaseEditWindow::operator =( const RDOBaseEditWindow& window )
{
	wordWrap          = window.wordWrap;
	showHorzScrollBar = window.showHorzScrollBar;

	return *this;
}

bool RDOBaseEditWindow::operator ==( const RDOBaseEditWindow& window ) const
{
	return wordWrap          == window.wordWrap&&
	       showHorzScrollBar == window.showHorzScrollBar;
}

bool RDOBaseEditWindow::operator !=( const RDOBaseEditWindow& window ) const
{
	return !(*this == window);
}

void RDOBaseEditWindow::load( string regPath )
{
	regPath += "window";
	wordWrap          = AfxGetApp()->GetProfileInt( regPath.c_str(), "wordWrap", wordWrap ) ? true : false;
	showHorzScrollBar = AfxGetApp()->GetProfileInt( regPath.c_str(), "showHorzScrollBar", showHorzScrollBar ) ? true : false;
}

void RDOBaseEditWindow::save( string regPath ) const
{
	regPath += "window";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "wordWrap", wordWrap );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "showHorzScrollBar", showHorzScrollBar );
}

// ----------------------------------------------------------------------------
// ---------- RDOBaseEditStyle
// ----------------------------------------------------------------------------
RDOBaseEditStyle::RDOBaseEditStyle():
	regPath( "" ),
	theme( NULL ),
	font( NULL ),
	tab( NULL ),
	window( NULL )
{
}

RDOBaseEditStyle::~RDOBaseEditStyle()
{
	if ( theme )  { delete theme;  theme = NULL; };
	if ( font )   { delete font;   font = NULL; };
	if ( tab )    { delete tab;    tab = NULL; };
	if ( window ) { delete window; window = NULL; };
}

void RDOBaseEditStyle::initTheme()
{
	theme = new RDOBaseEditTheme;
}

void RDOBaseEditStyle::initFont()
{
	font = new RDOBaseEditFont;
}

void RDOBaseEditStyle::initTab()
{
	tab = new RDOBaseEditTab;
}

void RDOBaseEditStyle::initWindow()
{
	window = new RDOBaseEditWindow;
}

RDOBaseEditStyle& RDOBaseEditStyle::operator =( const RDOBaseEditStyle& style )
{
	if ( theme  && style.theme )  *theme  = *style.theme;
	if ( font   && style.font )   *font   = *style.font;
	if ( tab    && style.tab )    *tab    = *style.tab;
	if ( window && style.window ) *window = *style.window;

	return *this;
}

bool RDOBaseEditStyle::operator ==( const RDOBaseEditStyle& style ) const
{
	bool flag = true;
	if ( theme  && style.theme  && flag ) flag &= *theme  == *style.theme;
	if ( font   && style.font   && flag ) flag &= *font   == *style.font;
	if ( tab    && style.tab    && flag ) flag &= *tab    == *style.tab;
	if ( window && style.window && flag ) flag &= *window == *style.window;
	return flag;
}

bool RDOBaseEditStyle::operator !=( const RDOBaseEditStyle& style ) const
{
	return !(*this == style);
}

void RDOBaseEditStyle::init( const string& _regPath )
{
	regPath = _regPath;
	if ( regPath.empty() ) {
		regPath = "editStyle\\";
	}
	if ( regPath.find_last_of( '\\' ) != regPath.length() - 1 ) {
		regPath += '\\';
	}
	regPath = "style\\" + regPath;
	initTheme();
	initFont();
	initTab();
	initWindow();
}

void RDOBaseEditStyle::load()
{
	if ( theme )  theme->load( regPath );
	if ( font )   font->load( regPath );
	if ( tab )    tab->load( regPath );
	if ( window ) window->load( regPath );
}

void RDOBaseEditStyle::save() const
{
	if ( theme )  theme->save( regPath );
	if ( font )   font->save( regPath );
	if ( tab )    tab->save( regPath );
	if ( window ) window->save( regPath );
}
