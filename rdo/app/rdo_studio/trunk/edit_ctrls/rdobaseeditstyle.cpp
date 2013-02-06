/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobaseeditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseeditstyle.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoStyle;
using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditTheme
// --------------------------------------------------------------------------------
RDOBaseEditTheme::RDOBaseEditTheme(): RDOStyleTheme()
{
	defaultColor    = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );

	caretColor       = QColor( 0x00, 0x00, 0x00 );
	selectionBgColor = QColor( 0xC0, 0xC0, 0xC0 );
	bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	bookmarkBgColor  = QColor( 0x00, 0xFF, 0xFF );

	defaultStyle = RDOStyleFont::NONE;

	bookmarkStyle = RDOBOOKMARKS_CIRCLE;
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

	defaultStyle = theme.defaultStyle;

	bookmarkStyle = theme.bookmarkStyle;

	return *this;
}

rbool RDOBaseEditTheme::operator ==( const RDOBaseEditTheme& theme ) const
{
	return defaultColor    == theme.defaultColor &&
	       backgroundColor == theme.backgroundColor &&

	       caretColor       == theme.caretColor &&
	       selectionBgColor == theme.selectionBgColor &&
	       bookmarkFgColor  == theme.bookmarkFgColor &&
	       bookmarkBgColor  == theme.bookmarkBgColor &&

	       defaultStyle == theme.defaultStyle &&

	       bookmarkStyle == theme.bookmarkStyle;
}

rbool RDOBaseEditTheme::operator !=( const RDOBaseEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOBaseEditTheme::load( tstring regPath )
{
	regPath += "theme";
	defaultColor     = AfxGetApp()->GetProfileInt( regPath.c_str(), "defaultColor", defaultColor.rgb() );
	backgroundColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "backgroundColor", backgroundColor.rgb() );
	caretColor       = AfxGetApp()->GetProfileInt( regPath.c_str(), "caretColor", caretColor.rgb() );
	selectionBgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "selectionBgColor", selectionBgColor.rgb() );
	bookmarkFgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "bookmarkFgColor", bookmarkFgColor.rgb() );
	bookmarkBgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "bookmarkBgColor", bookmarkBgColor.rgb() );
	defaultStyle     = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "defaultStyle", defaultStyle ));
	bookmarkStyle    = static_cast<RDOBookmarkStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "bookmarkStyle", bookmarkStyle ));
}

void RDOBaseEditTheme::save( tstring regPath ) const
{
	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "defaultColor", defaultColor.rgb() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "backgroundColor", backgroundColor.rgb() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "caretColor", caretColor.rgb() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "selectionBgColor", selectionBgColor.rgb() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "bookmarkFgColor", bookmarkFgColor.rgb() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "bookmarkBgColor", bookmarkBgColor.rgb() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "defaultStyle", defaultStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "bookmarkStyle", bookmarkStyle );
}

rbool RDOBaseEditTheme::styleDefault( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

rbool RDOBaseEditTheme::styleUsing( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

rbool RDOBaseEditTheme::styleBold( const int /*styleType*/ ) const
{
	return defaultStyle & RDOStyleFont::BOLD ? true : false;
}

rbool RDOBaseEditTheme::styleItalic( const int /*styleType*/ ) const
{
	return defaultStyle & RDOStyleFont::ITALIC ? true : false;
}

tstring RDOBaseEditTheme::styleFGColorToHEX( const int /*styleType*/ ) const
{
	return colorToHEX( defaultColor.rgb() );
}

tstring RDOBaseEditTheme::styleBGColorToHEX( const int /*styleType*/ ) const
{
	return colorToHEX( backgroundColor.rgb() );
}

RDOBaseEditTheme RDOBaseEditTheme::getDefaultTheme()
{
	RDOBaseEditTheme theme;
	return theme;
}

RDOBaseEditTheme RDOBaseEditTheme::getClassicTheme()
{
	RDOBaseEditTheme theme;

	theme.defaultColor    = QColor( 0xFF, 0xFF, 0x00 );
	theme.backgroundColor = QColor( 0x00, 0x00, 0x80 );

	theme.caretColor       = QColor( 0xFF, 0xFF, 0x00 );
	theme.selectionBgColor = QColor( 0x00, 0x00, 0x40 );
	theme.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = QColor( 0x80, 0x80, 0x00 );

	theme.defaultStyle = RDOStyleFont::NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

RDOBaseEditTheme RDOBaseEditTheme::getTwilightTheme()
{
	RDOBaseEditTheme theme;

	theme.defaultColor    = QColor( 0xFF, 0xFF, 0xFF );
	theme.backgroundColor = QColor( 0x00, 0x00, 0x00 );

	theme.caretColor       = QColor( 0xFF, 0xFF, 0xFF );
	theme.selectionBgColor = QColor( 0x70, 0x70, 0x70 );
	theme.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = QColor( 0x00, 0x00, 0xFF );

	theme.defaultStyle = RDOStyleFont::NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

RDOBaseEditTheme RDOBaseEditTheme::getOceanTheme()
{
	RDOBaseEditTheme theme;

	theme.defaultColor    = QColor( 0x00, 0x00, 0xFF );
	theme.backgroundColor = QColor( 0x00, 0xFF, 0xFF );

	theme.caretColor       = QColor( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = QColor( 0xC0, 0xC0, 0xD0 );
	theme.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = QColor( 0xBA, 0xCC, 0xFC );

	theme.defaultStyle = RDOStyleFont::NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;

	return theme;
}

tstring RDOBaseEditTheme::colorToHEX( const COLORREF color )
{
	return rdo::format( "#%02X%02X%02X", GetRValue( color ), GetGValue( color ), GetBValue( color ) );
}

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditTab
// --------------------------------------------------------------------------------
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

rbool RDOBaseEditTab::operator ==( const RDOBaseEditTab& tab ) const
{
	return tabSize         == tab.tabSize &&
	       indentSize      == tab.indentSize &&
	       useTabs         == tab.useTabs &&
	       tabIndents      == tab.tabIndents &&
	       backspaceUntabs == tab.backspaceUntabs &&
	       autoIndent      == tab.autoIndent;
}

rbool RDOBaseEditTab::operator !=( const RDOBaseEditTab& tab ) const
{
	return !(*this == tab);
}

void RDOBaseEditTab::load( tstring regPath )
{
	regPath += "tab";
	tabSize         = AfxGetApp()->GetProfileInt( regPath.c_str(), "tabSize", tabSize );
	indentSize      = AfxGetApp()->GetProfileInt( regPath.c_str(), "indentSize", indentSize );
	useTabs         = AfxGetApp()->GetProfileInt( regPath.c_str(), "useTabs", useTabs ) ? true : false;
	tabIndents      = AfxGetApp()->GetProfileInt( regPath.c_str(), "tabIndents", tabIndents ) ? true : false;
	backspaceUntabs = AfxGetApp()->GetProfileInt( regPath.c_str(), "backspaceUntabs", backspaceUntabs ) ? true : false;
	autoIndent      = AfxGetApp()->GetProfileInt( regPath.c_str(), "autoIndent", autoIndent ) ? true : false;
}

void RDOBaseEditTab::save( tstring regPath ) const
{
	regPath += "tab";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "tabSize", tabSize );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "indentSize", indentSize );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "useTabs", useTabs );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "tabIndents", tabIndents );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "backspaceUntabs", backspaceUntabs );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "autoIndent", autoIndent );
}

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditWindow
// --------------------------------------------------------------------------------
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

rbool RDOBaseEditWindow::operator ==( const RDOBaseEditWindow& window ) const
{
	return wordWrap          == window.wordWrap &&
	       showHorzScrollBar == window.showHorzScrollBar;
}

rbool RDOBaseEditWindow::operator !=( const RDOBaseEditWindow& window ) const
{
	return !(*this == window);
}

void RDOBaseEditWindow::load( tstring regPath )
{
	regPath += "window";
	wordWrap          = AfxGetApp()->GetProfileInt( regPath.c_str(), "wordWrap", wordWrap ) ? true : false;
	showHorzScrollBar = AfxGetApp()->GetProfileInt( regPath.c_str(), "showHorzScrollBar", showHorzScrollBar ) ? true : false;
}

void RDOBaseEditWindow::save( tstring regPath ) const
{
	regPath += "window";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "wordWrap", wordWrap );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "showHorzScrollBar", showHorzScrollBar );
}

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditStyle
// --------------------------------------------------------------------------------
RDOBaseEditStyle::RDOBaseEditStyle():
	RDOStyleWithTheme(),
	tab( NULL ),
	window( NULL )
{
}

RDOBaseEditStyle::~RDOBaseEditStyle()
{
	if ( tab )    { delete tab;    tab = NULL; };
	if ( window ) { delete window; window = NULL; };
}

void RDOBaseEditStyle::initTheme()
{
	theme = new RDOBaseEditTheme;
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
	RDOStyleWithTheme::operator=( style );
	if ( theme  && style.theme )  *static_cast<RDOBaseEditTheme*>(theme) = *static_cast<RDOBaseEditTheme*>(style.theme);
	if ( tab    && style.tab )    *tab    = *style.tab;
	if ( window && style.window ) *window = *style.window;

	return *this;
}

rbool RDOBaseEditStyle::operator ==( const RDOBaseEditStyle& style ) const
{
	rbool flag = RDOStyleWithTheme::operator==( style );
	if ( theme  && style.theme  && flag ) flag &= *static_cast<RDOBaseEditTheme*>(theme) == *static_cast<RDOBaseEditTheme*>(style.theme);
	if ( tab    && style.tab    && flag ) flag &= *tab    == *style.tab;
	if ( window && style.window && flag ) flag &= *window == *style.window;
	return flag;
}

rbool RDOBaseEditStyle::operator !=( const RDOBaseEditStyle& style ) const
{
	return !(*this == style);
}

void RDOBaseEditStyle::init( CREF(tstring) _regPath )
{
	RDOStyleWithTheme::init( _regPath );
	initTab();
	initWindow();
}

rbool RDOBaseEditStyle::load()
{
	if ( RDOStyleWithTheme::load() ) {
		if ( tab )    tab->load( regPath );
		if ( window ) window->load( regPath );
		return true;
	}
	return false;
}

rbool RDOBaseEditStyle::save() const
{
	if ( RDOStyleWithTheme::save() ) {
		if ( tab )    tab->save( regPath );
		if ( window ) window->save( regPath );
		return true;
	}
	return false;
}
