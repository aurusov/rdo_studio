/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/edit_style.h"
// --------------------------------------------------------------------------------

using namespace rdoStyle;
using namespace rdo::gui::editor;

// --------------------------------------------------------------------------------
// -------------------- EditTheme
// --------------------------------------------------------------------------------
EditTheme::EditTheme(): RDOStyleTheme()
{
	defaultColor    = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );

	caretColor       = QColor( 0x00, 0x00, 0x00 );
	selectionBgColor = QColor( 0xC0, 0xC0, 0xC0 );
	bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	bookmarkBgColor  = QColor( 0x00, 0xFF, 0xFF );

	defaultStyle  = RDOStyleFont::NONE;
	bookmarkStyle = EditTheme::B_CIRCLE;
}

EditTheme::~EditTheme()
{
}

EditTheme& EditTheme::operator =( const EditTheme& theme )
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

rbool EditTheme::operator ==( const EditTheme& theme ) const
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

rbool EditTheme::operator !=( const EditTheme& theme ) const
{
	return !(*this == theme);
}

void EditTheme::load(QSettings& settings)
{
	settings >> *this;
}

void EditTheme::save(QSettings& settings) const
{
	settings << *this;
}

rbool EditTheme::styleDefault( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

rbool EditTheme::styleUsing( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

rbool EditTheme::styleBold( const int /*styleType*/ ) const
{
	return defaultStyle & RDOStyleFont::BOLD ? true : false;
}

rbool EditTheme::styleItalic( const int /*styleType*/ ) const
{
	return defaultStyle & RDOStyleFont::ITALIC ? true : false;
}

tstring EditTheme::styleFGColorToHEX( const int /*styleType*/ ) const
{
	return colorToHEX( defaultColor );
}

tstring EditTheme::styleBGColorToHEX( const int /*styleType*/ ) const
{
	return colorToHEX( backgroundColor );
}

EditTheme EditTheme::getDefaultTheme()
{
	EditTheme theme;
	return theme;
}

EditTheme EditTheme::getClassicTheme()
{
	EditTheme theme;

	theme.defaultColor    = QColor( 0xFF, 0xFF, 0x00 );
	theme.backgroundColor = QColor( 0x00, 0x00, 0x80 );

	theme.caretColor       = QColor( 0xFF, 0xFF, 0x00 );
	theme.selectionBgColor = QColor( 0x00, 0x00, 0x40 );
	theme.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = QColor( 0x80, 0x80, 0x00 );

	theme.defaultStyle  = RDOStyleFont::NONE;
	theme.bookmarkStyle = EditTheme::B_CIRCLE;

	return theme;
}

EditTheme EditTheme::getTwilightTheme()
{
	EditTheme theme;

	theme.defaultColor    = QColor( 0xFF, 0xFF, 0xFF );
	theme.backgroundColor = QColor( 0x00, 0x00, 0x00 );

	theme.caretColor       = QColor( 0xFF, 0xFF, 0xFF );
	theme.selectionBgColor = QColor( 0x70, 0x70, 0x70 );
	theme.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = QColor( 0x00, 0x00, 0xFF );

	theme.defaultStyle  = RDOStyleFont::NONE;
	theme.bookmarkStyle = EditTheme::B_CIRCLE;

	return theme;
}

EditTheme EditTheme::getOceanTheme()
{
	EditTheme theme;

	theme.defaultColor    = QColor( 0x00, 0x00, 0xFF );
	theme.backgroundColor = QColor( 0x00, 0xFF, 0xFF );

	theme.caretColor       = QColor( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = QColor( 0xC0, 0xC0, 0xD0 );
	theme.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = QColor( 0xBA, 0xCC, 0xFC );

	theme.defaultStyle  = RDOStyleFont::NONE;
	theme.bookmarkStyle = EditTheme::B_CIRCLE;

	return theme;
}

tstring EditTheme::colorToHEX( const QColor color )
{
	return rdo::format( "#%02X%02X%02X", color.red(), color.green(), color.blue() );
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const EditTheme& theme)
{
	settings.setValue("default_color", theme.defaultColor.name());
	settings.setValue("background_color", theme.backgroundColor.name());
	settings.setValue("caret_color", theme.caretColor.name());
	settings.setValue("selection_bg_color", theme.selectionBgColor.name());
	settings.setValue("bookmark_fg_color", theme.bookmarkFgColor.name());
	settings.setValue("bookmark_bg_color", theme.bookmarkBgColor.name());
	settings.setValue("default_style", theme.defaultStyle);
	settings.setValue("bookmark_style", theme.bookmarkStyle);

	return settings;
}

QSettings& operator>> (QSettings& settings, EditTheme& theme)
{
	theme.defaultColor     = QColor(settings.value("default_color", theme.defaultColor.name()).toString());
	theme.backgroundColor  = QColor(settings.value("background_color", theme.backgroundColor.name()).toString());
	theme.caretColor       = QColor(settings.value("caret_color", theme.caretColor.name()).toString());
	theme.selectionBgColor = QColor(settings.value("selection_bg_color", theme.selectionBgColor.name()).toString());
	theme.bookmarkFgColor  = QColor(settings.value("bookmark_fg_color", theme.bookmarkFgColor.name()).toString());
	theme.bookmarkBgColor  = QColor(settings.value("bookmark_bg_color", theme.bookmarkBgColor.name()).toString());
	theme.defaultStyle     = static_cast<RDOStyleFont::style>(settings.value("default_style", theme.defaultStyle).toInt());
	theme.bookmarkStyle    = static_cast<EditTheme::Bookmark>(settings.value("bookmark_style", theme.bookmarkStyle).toInt());
	
	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- EditTab
// --------------------------------------------------------------------------------
EditTab::EditTab()
{
	tabSize         = 4;
	indentSize      = 4;
	useTabs         = true;
	tabIndents      = true;
	backspaceUntabs = false;
	autoIndent      = true;
}

EditTab::~EditTab()
{}

EditTab& EditTab::operator =( const EditTab& tab )
{
	tabSize         = tab.tabSize;
	indentSize      = tab.indentSize;
	useTabs         = tab.useTabs;
	tabIndents      = tab.tabIndents;
	backspaceUntabs = tab.backspaceUntabs;
	autoIndent      = tab.autoIndent;

	return *this;
}

rbool EditTab::operator ==( const EditTab& tab ) const
{
	return tabSize         == tab.tabSize &&
	       indentSize      == tab.indentSize &&
	       useTabs         == tab.useTabs &&
	       tabIndents      == tab.tabIndents &&
	       backspaceUntabs == tab.backspaceUntabs &&
	       autoIndent      == tab.autoIndent;
}

rbool EditTab::operator !=( const EditTab& tab ) const
{
	return !(*this == tab);
}

void EditTab::load(QSettings& settings)
{
	settings >> *this;
}

void EditTab::save(QSettings& settings) const
{
	settings << *this;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const EditTab& tab)
{
	settings.setValue("tab_size", tab.tabSize);
	settings.setValue("indent_size", tab.indentSize);
	settings.setValue("use_tabs", tab.useTabs);
	settings.setValue("tab_indents", tab.tabIndents);
	settings.setValue("backspace_untabs", tab.backspaceUntabs);
	settings.setValue("auto_indent", tab.autoIndent);

	return settings;
}

QSettings& operator>> (QSettings& settings, EditTab& tab)
{
	tab.tabSize         = settings.value("tab_size", tab.tabSize).toInt();
	tab.indentSize      = settings.value("indent_size", tab.indentSize).toInt();
	tab.useTabs         = settings.value("use_tabs", tab.useTabs).toBool() ? true : false;
	tab.tabIndents      = settings.value("tab_indents", tab.tabIndents).toBool() ? true : false;
	tab.backspaceUntabs = settings.value("backspace_untabs", tab.backspaceUntabs).toBool() ? true : false;
	tab.autoIndent      = settings.value("auto_indent", tab.autoIndent).toBool() ? true : false;

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- EditWindow
// --------------------------------------------------------------------------------
EditWindow::EditWindow()
{
	wordWrap          = false;
	showHorzScrollBar = true;
}

EditWindow::~EditWindow()
{}

EditWindow& EditWindow::operator =( const EditWindow& window )
{
	wordWrap          = window.wordWrap;
	showHorzScrollBar = window.showHorzScrollBar;

	return *this;
}

rbool EditWindow::operator ==( const EditWindow& window ) const
{
	return wordWrap          == window.wordWrap &&
	       showHorzScrollBar == window.showHorzScrollBar;
}

rbool EditWindow::operator !=( const EditWindow& window ) const
{
	return !(*this == window);
}

void EditWindow::load(QSettings& settings)
{
	settings >> *this;
}

void EditWindow::save(QSettings& settings) const
{
	settings << *this;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const EditWindow& window)
{
	settings.setValue("word_wrap", window.wordWrap);
	settings.setValue("show_horz_scroll_bar", window.showHorzScrollBar);

	return settings;
}

QSettings& operator>> (QSettings& settings, EditWindow& window)
{
	window.wordWrap          = settings.value("word_wrap", window.wordWrap).toBool() ? true : false;
	window.showHorzScrollBar = settings.value("show_horz_scroll_bar", window.showHorzScrollBar).toBool() ? true : false;

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- EditStyle
// --------------------------------------------------------------------------------
EditStyle::EditStyle()
	: RDOStyleWithTheme()
	, tab   (NULL)
	, window(NULL)
{
	theme  = new EditTheme();
	tab    = new EditTab();
	window = new EditWindow();
}

EditStyle::~EditStyle()
{
	if ( tab )    { delete tab;    tab = NULL; };
	if ( window ) { delete window; window = NULL; };
}

EditStyle& EditStyle::operator =( const EditStyle& style )
{
	RDOStyleWithTheme::operator=( style );
	if ( theme  && style.theme )  *static_cast<EditTheme*>(theme) = *static_cast<EditTheme*>(style.theme);
	if ( tab    && style.tab )    *tab    = *style.tab;
	if ( window && style.window ) *window = *style.window;

	return *this;
}

rbool EditStyle::operator ==( const EditStyle& style ) const
{
	rbool flag = RDOStyleWithTheme::operator==( style );
	if ( theme  && style.theme  && flag ) flag &= *static_cast<EditTheme*>(theme) == *static_cast<EditTheme*>(style.theme);
	if ( tab    && style.tab    && flag ) flag &= *tab    == *style.tab;
	if ( window && style.window && flag ) flag &= *window == *style.window;
	return flag;
}

rbool EditStyle::operator !=( const EditStyle& style ) const
{
	return !(*this == style);
}

void EditStyle::init( CREF(QString) _groupName )
{
	RDOStyleWithTheme::init( _groupName );
}

rbool EditStyle::load()
{
	if ( RDOStyleWithTheme::load() ) {
		QSettings settings;
		settings.beginGroup(groupName + "tab");
		if (tab)    tab->load(settings);
		settings.endGroup();
		settings.beginGroup(groupName + "window");
		if (window) window->load(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

rbool EditStyle::save() const
{
	if ( RDOStyleWithTheme::save() ) {
		QSettings settings;
		settings.beginGroup(groupName + "tab");
		if (tab)    tab->save(settings);
		settings.endGroup();
		settings.beginGroup(groupName + "window");
		if (window) window->save(settings);
		settings.endGroup();
		return true;
	}
	return false;
}
