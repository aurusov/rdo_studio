/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologeditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

// --------------------------------------------------------------------------------
// -------------------- LogEditTheme
// --------------------------------------------------------------------------------
LogEditTheme::LogEditTheme(): EditBaseTheme()
{
	selectLineBgColor = QColor( 0x00, 0xC0, 0xEF );
}

LogEditTheme::~LogEditTheme()
{
}

LogEditTheme& LogEditTheme::operator =( const LogEditTheme& theme )
{
	EditBaseTheme::operator=( theme );
	selectLineBgColor = theme.selectLineBgColor;

	return *this;
}

rbool LogEditTheme::operator ==( const LogEditTheme& theme ) const
{
	rbool flag = EditBaseTheme::operator==( theme );
	if ( flag )	flag &= selectLineBgColor == theme.selectLineBgColor ? true : false;
	return flag;
}

rbool LogEditTheme::operator !=( const LogEditTheme& theme ) const
{
	return !(*this == theme);
}

void LogEditTheme::load(QSettings& settings)
{
	EditBaseTheme::load(settings);

	settings >> *this;
}

void LogEditTheme::save(QSettings& settings) const
{
	EditBaseTheme::save(settings);

	settings << *this;
}

LogEditTheme LogEditTheme::getDefaultTheme()
{
	LogEditTheme theme;
	return theme;
}

LogEditTheme LogEditTheme::getClassicTheme()
{
	LogEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getClassicTheme();

	theme.selectLineBgColor = QColor( 0x00, 0xC0, 0xEF );

	return theme;
}

LogEditTheme LogEditTheme::getTwilightTheme()
{
	LogEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getTwilightTheme();

	theme.selectLineBgColor = QColor( 0x00, 0x96, 0xBB );

	return theme;
}

LogEditTheme LogEditTheme::getOceanTheme()
{
	LogEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getOceanTheme();

	theme.selectLineBgColor = QColor( 0xCA, 0xF4, 0xFF );

	return theme;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const LogEditTheme& theme)
{
	settings.setValue("select_line_bg_color", theme.selectLineBgColor.name());

	return settings;
}

QSettings& operator>> (QSettings& settings, LogEditTheme& theme)
{
	theme.selectLineBgColor = QColor(settings.value("select_line_bg_color", theme.selectLineBgColor.name()).toString());

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- LogEditStyle
// --------------------------------------------------------------------------------
LogEditStyle::LogEditStyle(): EditBaseStyle()
{
	theme = new LogEditTheme();
}

LogEditStyle::~LogEditStyle()
{
}

LogEditStyle& LogEditStyle::operator =( const LogEditStyle& style )
{
	EditBaseStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<LogEditTheme*>(theme) = *static_cast<LogEditTheme*>(style.theme);

	return *this;
}

rbool LogEditStyle::operator ==( const LogEditStyle& style ) const
{
	rbool flag = EditBaseStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<LogEditTheme*>(theme) == *static_cast<LogEditTheme*>(style.theme);
	return flag;
}

rbool LogEditStyle::operator !=( const LogEditStyle& style ) const
{
	return !(*this == style);
}
