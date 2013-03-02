/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindeditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdofindeditstyle.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;
using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- FindTheme
// --------------------------------------------------------------------------------
FindTheme::FindTheme(): LogEditTheme()
{
	keywordColor = QColor( 0x00, 0x00, 0x00 );
	keywordStyle = RDOStyleFont::BOLD;
}

FindTheme::~FindTheme()
{}

FindTheme& FindTheme::operator =( const FindTheme& theme )
{
	LogEditTheme::operator=( theme );

	keywordColor = theme.keywordColor;
	keywordStyle = theme.keywordStyle;

	return *this;
}

rbool FindTheme::operator ==( const FindTheme& theme ) const
{
	rbool flag = LogEditTheme::operator==( theme );

	if ( flag ) flag &= keywordColor == theme.keywordColor &&
	                    keywordStyle == theme.keywordStyle;
	return flag;
}

rbool FindTheme::operator !=( const FindTheme& theme ) const
{
	return !(*this == theme);
}

void FindTheme::load(QSettings& settings)
{
	LogEditTheme::load(settings);

	settings >> *this;
}

void FindTheme::save(QSettings& settings) const
{
	LogEditTheme::save(settings);

	settings << *this;
}

rbool FindTheme::styleDefault( const int styleType ) const
{
	return styleType == SCE_FIND_DEFAULT;
}

rbool FindTheme::styleUsing( const int styleType ) const
{
	return styleType == SCE_FIND_DEFAULT || styleType == SCE_FIND_KEYWORD;
}

rbool FindTheme::styleBold( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return defaultStyle & RDOStyleFont::BOLD ? true : false;
		case SCE_FIND_KEYWORD: return keywordStyle & RDOStyleFont::BOLD ? true : false;
	}
	return false;
}

rbool FindTheme::styleItalic( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return defaultStyle & RDOStyleFont::ITALIC ? true : false;
		case SCE_FIND_KEYWORD: return keywordStyle & RDOStyleFont::ITALIC ? true : false;
	}
	return false;
}

tstring FindTheme::styleFGColorToHEX( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return colorToHEX( defaultColor );
		case SCE_FIND_KEYWORD: return colorToHEX( keywordColor );
	}
	return LogEditTheme::styleFGColorToHEX( styleType );
}

FindTheme FindTheme::getDefaultTheme()
{
	FindTheme theme;
	return theme;
}

FindTheme FindTheme::getClassicTheme()
{
	FindTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getClassicTheme();

	theme.keywordColor = QColor( 0xFF, 0xFF, 0xFF );
	theme.keywordStyle = RDOStyleFont::NONE;

	return theme;
}

FindTheme FindTheme::getTwilightTheme()
{
	FindTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getTwilightTheme();

	theme.keywordColor = QColor( 0x00, 0xFF, 0xFF );
	theme.keywordStyle = RDOStyleFont::BOLD;

	return theme;
}

FindTheme FindTheme::getOceanTheme()
{
	FindTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getOceanTheme();

	theme.keywordColor = QColor( 0x00, 0x00, 0x00 );
	theme.keywordStyle = RDOStyleFont::BOLD;

	return theme;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const FindTheme& theme)
{
	settings.setValue("keyword_color", theme.keywordColor.name());
	settings.setValue("keyword_style", theme.keywordStyle);

	return settings;
}

QSettings& operator>> (QSettings& settings, FindTheme& theme)
{
	theme.keywordColor = QColor(settings.value("keyword_color", theme.keywordColor.name()).toString());
	theme.keywordStyle = static_cast<RDOStyleFont::style>(settings.value("keyword_style", theme.keywordStyle).toInt());

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- FindStyle
// --------------------------------------------------------------------------------
FindStyle::FindStyle(): LogEditStyle()
{
	theme = new FindTheme();
}

FindStyle::~FindStyle()
{}

FindStyle& FindStyle::operator =( const FindStyle& style )
{
	LogEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<FindTheme*>(theme) = *static_cast<FindTheme*>(style.theme);

	return *this;
}

rbool FindStyle::operator ==( const FindStyle& style ) const
{
	rbool flag = LogEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<FindTheme*>(theme) == *static_cast<FindTheme*>(style.theme);
	return flag;
}

rbool FindStyle::operator !=( const FindStyle& style ) const
{
	return !(*this == style);
}
