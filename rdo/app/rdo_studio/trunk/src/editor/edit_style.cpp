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

using namespace rdo::gui::style;
using namespace rdo::gui::editor;

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
	: StyleBase()
	, tab   ()
	, window()
{
	defaultColor    = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );

	caretColor       = QColor( 0x00, 0x00, 0x00 );
	selectionBgColor = QColor( 0xC0, 0xC0, 0xC0 );
	bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	bookmarkBgColor  = QColor( 0x00, 0xFF, 0xFF );

	defaultStyle  = StyleFont::NONE;
	bookmarkStyle = EditStyle::B_CIRCLE;
}

EditStyle::~EditStyle()
{
}

EditStyle& EditStyle::operator =( const EditStyle& style )
{
	StyleBase::operator=( style );
	
	defaultColor    = style.defaultColor;
	backgroundColor = style.backgroundColor;

	caretColor       = style.caretColor;
	selectionBgColor = style.selectionBgColor;
	bookmarkFgColor  = style.bookmarkFgColor;
	bookmarkBgColor  = style.bookmarkBgColor;

	defaultStyle = style.defaultStyle;

	bookmarkStyle = style.bookmarkStyle;
	
	tab    = style.tab;
	window = style.window;

	return *this;
}

rbool EditStyle::operator ==( const EditStyle& style ) const
{
	rbool flag = StyleBase::operator==( style );
	flag &= tab    == style.tab;
	flag &= window == style.window;
	return flag &&
		defaultColor     == style.defaultColor &&
		backgroundColor  == style.backgroundColor &&
		caretColor       == style.caretColor &&
		selectionBgColor == style.selectionBgColor &&
		bookmarkFgColor  == style.bookmarkFgColor &&
		bookmarkBgColor  == style.bookmarkBgColor &&
		defaultStyle     == style.defaultStyle &&
		bookmarkStyle    == style.bookmarkStyle;
}

rbool EditStyle::operator !=( const EditStyle& style ) const
{
	return !(*this == style);
}

void EditStyle::loadStyle(QSettings& settings)
{
	StyleBase::loadStyle(settings);	
	
	settings.beginGroup("theme");
	settings >> *this;
	settings.endGroup();
	settings.beginGroup("tab");
	tab.load(settings);
	settings.endGroup();
	settings.beginGroup("window");
	window.load(settings);
	settings.endGroup();
}

void EditStyle::saveStyle(QSettings& settings) const
{
	StyleBase::saveStyle(settings);
	
	settings.beginGroup("theme");
	settings << *this;
	settings.endGroup();
	settings.beginGroup("tab");
	tab.save(settings);
	settings.endGroup();
	settings.beginGroup("window");
	window.save(settings);
	settings.endGroup();
}

rbool EditStyle::styleDefault( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

rbool EditStyle::styleUsing( const int styleType ) const
{
	return styleType == STYLE_DEFAULT;
}

rbool EditStyle::styleBold( const int /*styleType*/ ) const
{
	return defaultStyle & StyleFont::BOLD ? true : false;
}

rbool EditStyle::styleItalic( const int /*styleType*/ ) const
{
	return defaultStyle & StyleFont::ITALIC ? true : false;
}

tstring EditStyle::styleFGColorToHEX( const int /*styleType*/ ) const
{
	return colorToHEX( defaultColor );
}

tstring EditStyle::styleBGColorToHEX( const int /*styleType*/ ) const
{
	return colorToHEX( backgroundColor );
}

EditStyle EditStyle::getDefaultStyle()
{
	EditStyle style;
	return style;
}

EditStyle EditStyle::getClassicStyle()
{
	EditStyle style;

	style.defaultColor    = QColor( 0xFF, 0xFF, 0x00 );
	style.backgroundColor = QColor( 0x00, 0x00, 0x80 );

	style.caretColor       = QColor( 0xFF, 0xFF, 0x00 );
	style.selectionBgColor = QColor( 0x00, 0x00, 0x40 );
	style.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	style.bookmarkBgColor  = QColor( 0x80, 0x80, 0x00 );

	style.defaultStyle  = StyleFont::NONE;
	style.bookmarkStyle = EditStyle::B_CIRCLE;

	return style;
}

EditStyle EditStyle::getTwilightStyle()
{
	EditStyle style;

	style.defaultColor    = QColor( 0xFF, 0xFF, 0xFF );
	style.backgroundColor = QColor( 0x00, 0x00, 0x00 );

	style.caretColor       = QColor( 0xFF, 0xFF, 0xFF );
	style.selectionBgColor = QColor( 0x70, 0x70, 0x70 );
	style.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	style.bookmarkBgColor  = QColor( 0x00, 0x00, 0xFF );

	style.defaultStyle  = StyleFont::NONE;
	style.bookmarkStyle = EditStyle::B_CIRCLE;

	return style;
}

EditStyle EditStyle::getOceanStyle()
{
	EditStyle style;

	style.defaultColor    = QColor( 0x00, 0x00, 0xFF );
	style.backgroundColor = QColor( 0x00, 0xFF, 0xFF );

	style.caretColor       = QColor( 0x00, 0x00, 0x00 );
	style.selectionBgColor = QColor( 0xC0, 0xC0, 0xD0 );
	style.bookmarkFgColor  = QColor( 0x00, 0x00, 0x00 );
	style.bookmarkBgColor  = QColor( 0xBA, 0xCC, 0xFC );

	style.defaultStyle  = StyleFont::NONE;
	style.bookmarkStyle = EditStyle::B_CIRCLE;

	return style;
}

tstring EditStyle::colorToHEX( const QColor color )
{
	return rdo::format( "#%02X%02X%02X", color.red(), color.green(), color.blue() );
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const EditStyle& style)
{
	settings.setValue("default_color", style.defaultColor.name());
	settings.setValue("background_color", style.backgroundColor.name());
	settings.setValue("caret_color", style.caretColor.name());
	settings.setValue("selection_bg_color", style.selectionBgColor.name());
	settings.setValue("bookmark_fg_color", style.bookmarkFgColor.name());
	settings.setValue("bookmark_bg_color", style.bookmarkBgColor.name());
	settings.setValue("default_style", style.defaultStyle);
	settings.setValue("bookmark_style", style.bookmarkStyle);

	return settings;
}

QSettings& operator>> (QSettings& settings, EditStyle& style)
{
	style.defaultColor     = QColor(settings.value("default_color", style.defaultColor.name()).toString());
	style.backgroundColor  = QColor(settings.value("background_color", style.backgroundColor.name()).toString());
	style.caretColor       = QColor(settings.value("caret_color", style.caretColor.name()).toString());
	style.selectionBgColor = QColor(settings.value("selection_bg_color", style.selectionBgColor.name()).toString());
	style.bookmarkFgColor  = QColor(settings.value("bookmark_fg_color", style.bookmarkFgColor.name()).toString());
	style.bookmarkBgColor  = QColor(settings.value("bookmark_bg_color", style.bookmarkBgColor.name()).toString());
	style.defaultStyle     = static_cast<StyleFont::style>(settings.value("default_style", style.defaultStyle).toInt());
	style.bookmarkStyle    = static_cast<EditStyle::Bookmark>(settings.value("bookmark_style", style.bookmarkStyle).toInt());

	return settings;
}

}}} // namespace rdo::gui::editor