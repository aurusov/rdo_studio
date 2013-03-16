/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/find_edit_style.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;
using namespace rdo::gui::style;

// --------------------------------------------------------------------------------
// -------------------- FindStyle
// --------------------------------------------------------------------------------
FindStyle::FindStyle(): LogStyle()
{
	keywordColor = QColor( 0x00, 0x00, 0x00 );
	keywordStyle = StyleFont::BOLD;
}

FindStyle::~FindStyle()
{}

FindStyle& FindStyle::operator =( const FindStyle& style )
{
	LogStyle::operator=( style );

	keywordColor = style.keywordColor;
	keywordStyle = style.keywordStyle;

	return *this;
}

rbool FindStyle::operator ==( const FindStyle& style ) const
{
	rbool flag = LogStyle::operator==( style );

	if ( flag ) flag &= keywordColor == style.keywordColor &&
		keywordStyle == style.keywordStyle;

	return flag;
}

rbool FindStyle::operator !=( const FindStyle& style ) const
{
	return !(*this == style);
}

rbool FindStyle::styleDefault( const int styleType ) const
{
	return styleType == SCE_FIND_DEFAULT;
}

rbool FindStyle::styleUsing( const int styleType ) const
{
	return styleType == SCE_FIND_DEFAULT || styleType == SCE_FIND_KEYWORD;
}

rbool FindStyle::styleBold( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return defaultStyle & StyleFont::BOLD ? true : false;
		case SCE_FIND_KEYWORD: return keywordStyle & StyleFont::BOLD ? true : false;
	}
	return false;
}

rbool FindStyle::styleItalic( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return defaultStyle & StyleFont::ITALIC ? true : false;
		case SCE_FIND_KEYWORD: return keywordStyle & StyleFont::ITALIC ? true : false;
	}
	return false;
}

tstring FindStyle::styleFGColorToHEX( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return colorToHEX( defaultColor );
		case SCE_FIND_KEYWORD: return colorToHEX( keywordColor );
	}
	return LogStyle::styleFGColorToHEX( styleType );
}

FindStyle FindStyle::getDefaultStyle()
{
	FindStyle style;
	return style;
}

FindStyle FindStyle::getClassicStyle()
{
	FindStyle style;
	*static_cast<LogStyle*>(&style) = LogStyle::getClassicStyle();

	style.keywordColor = QColor( 0xFF, 0xFF, 0xFF );
	style.keywordStyle = StyleFont::NONE;

	return style;
}

FindStyle FindStyle::getTwilightStyle()
{
	FindStyle style;
	*static_cast<LogStyle*>(&style) = LogStyle::getTwilightStyle();

	style.keywordColor = QColor( 0x00, 0xFF, 0xFF );
	style.keywordStyle = StyleFont::BOLD;

	return style;
}

FindStyle FindStyle::getOceanStyle()
{
	FindStyle style;
	*static_cast<LogStyle*>(&style) = LogStyle::getOceanStyle();

	style.keywordColor = QColor( 0x00, 0x00, 0x00 );
	style.keywordStyle = StyleFont::BOLD;

	return style;
}

void FindStyle::loadStyle(QSettings& settings)
{
	LogStyle::loadStyle(settings);
	
	settings.beginGroup("theme");
	settings >> *this;
	settings.endGroup();
}

void FindStyle::saveStyle(QSettings& settings) const
{
	LogStyle::saveStyle(settings);

	settings.beginGroup("theme");
	settings << *this;
	settings.endGroup();
	
}

namespace rdo { namespace gui { namespace editor {

	QSettings& operator<< (QSettings& settings, const FindStyle& style)
	{
		settings.setValue("keyword_color", style.keywordColor.name());
		settings.setValue("keyword_style", style.keywordStyle);

		return settings;
	}

	QSettings& operator>> (QSettings& settings, FindStyle& style)
	{
		style.keywordColor = QColor(settings.value("keyword_color", style.keywordColor.name()).toString());
		style.keywordStyle = static_cast<StyleFont::style>(settings.value("keyword_style", style.keywordStyle).toInt());

		return settings;
	}

}}} // namespace rdo::gui::editor