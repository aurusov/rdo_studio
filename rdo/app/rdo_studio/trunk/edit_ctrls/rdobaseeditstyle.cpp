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

void RDOBaseEditTheme::load( QString regPath )
{
	QSettings settings;
	regPath.append("theme\\");
	defaultColor     = QColor(settings.value(QString(regPath + "default_color"), defaultColor.name()).toString());
	backgroundColor  = QColor(settings.value(QString(regPath + "background_color"), backgroundColor.name()).toString());
	caretColor       = QColor(settings.value(QString(regPath + "caret_color"), caretColor.name()).toString());
	selectionBgColor = QColor(settings.value(QString(regPath + "selection_bg_color"), selectionBgColor.name()).toString());
	bookmarkFgColor  = QColor(settings.value(QString(regPath + "bookmark_fg_color"), bookmarkFgColor.name()).toString());
	bookmarkBgColor  = QColor(settings.value(QString(regPath + "bookmark_bg_color"), bookmarkBgColor.name()).toString());
	defaultStyle     = static_cast<RDOStyleFont::style>(settings.value(QString(regPath + "default_style"), defaultStyle).toInt());
	bookmarkStyle    = static_cast<RDOBookmarkStyle>(settings.value(QString(regPath + "bookmark_style"), bookmarkStyle).toInt());
}

void RDOBaseEditTheme::save( QString regPath ) const
{
	QSettings settings;
	regPath.append("theme\\");
	settings.setValue(QString(regPath + "default_color"), defaultColor.name());
	settings.setValue(QString(regPath + "background_color"), backgroundColor.name());
	settings.setValue(QString(regPath + "caret_color"), caretColor.name());
	settings.setValue(QString(regPath + "selection_bg_color"), selectionBgColor.name());
	settings.setValue(QString(regPath + "bookmark_fg_color"), bookmarkFgColor.name());
	settings.setValue(QString(regPath + "bookmark_bg_color"), bookmarkBgColor.name());
	settings.setValue(QString(regPath + "default_style"), defaultStyle);
	settings.setValue(QString(regPath + "bookmark_style"), bookmarkStyle);
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

tstring RDOBaseEditTheme::colorToHEX( const QColor color )
{
	return rdo::format( "#%02X%02X%02X", color.red(), color.green(), color.blue() );
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

void RDOBaseEditTab::load( QString regPath )
{
	QSettings settings;
	regPath.append("tab\\");
	tabSize         = settings.value(QString(regPath + "tab_size"), tabSize).toInt();
	indentSize      = settings.value(QString(regPath + "indent_size"), indentSize).toInt();
	useTabs         = settings.value(QString(regPath + "use_tabs"), useTabs).toBool() ? true : false;
	tabIndents      = settings.value(QString(regPath + "tab_indents"), tabIndents).toBool() ? true : false;
	backspaceUntabs = settings.value(QString(regPath + "backspace_untabs"), backspaceUntabs).toBool() ? true : false;
	autoIndent      = settings.value(QString(regPath + "auto_indent"), autoIndent).toBool() ? true : false;
}

void RDOBaseEditTab::save( QString regPath ) const
{
	QSettings settings;
	regPath.append("tab\\");
	settings.setValue(QString(regPath + "tab_size"), tabSize);
	settings.setValue(QString(regPath + "indent_size"), indentSize);
	settings.setValue(QString(regPath + "use_tabs"), useTabs);
	settings.setValue(QString(regPath + "tab_indents"), tabIndents);
	settings.setValue(QString(regPath + "backspace_untabs"), backspaceUntabs);
	settings.setValue(QString(regPath + "auto_indent"), autoIndent);
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

void RDOBaseEditWindow::load( QString regPath )
{
	QSettings settings;
	regPath.append("window\\");
	wordWrap          = settings.value(QString(regPath + "word_wrap"), wordWrap).toBool() ? true : false;
	showHorzScrollBar = settings.value(QString(regPath + "show_horz_scroll_bar"), showHorzScrollBar).toBool() ? true : false;
}

void RDOBaseEditWindow::save( QString regPath ) const
{
	QSettings settings;
	regPath.append("window\\");
	settings.setValue(QString(regPath + "word_wrap"), wordWrap);
	settings.setValue(QString(regPath + "show_horz_scroll_bar"), showHorzScrollBar);
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

void RDOBaseEditStyle::init( CREF(QString) _regPath )
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
