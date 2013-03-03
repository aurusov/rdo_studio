/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/log_edit_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

// --------------------------------------------------------------------------------
// -------------------- LogTheme
// --------------------------------------------------------------------------------
LogTheme::LogTheme(): EditTheme()
{
	selectLineBgColor = QColor( 0x00, 0xC0, 0xEF );
}

LogTheme::~LogTheme()
{}

LogTheme& LogTheme::operator =( const LogTheme& theme )
{
	EditTheme::operator=( theme );
	selectLineBgColor = theme.selectLineBgColor;

	return *this;
}

rbool LogTheme::operator ==( const LogTheme& theme ) const
{
	rbool flag = EditTheme::operator==( theme );
	if ( flag )	flag &= selectLineBgColor == theme.selectLineBgColor ? true : false;
	return flag;
}

rbool LogTheme::operator !=( const LogTheme& theme ) const
{
	return !(*this == theme);
}

void LogTheme::load(QSettings& settings)
{
	EditTheme::load(settings);

	settings >> *this;
}

void LogTheme::save(QSettings& settings) const
{
	EditTheme::save(settings);

	settings << *this;
}

LogTheme LogTheme::getDefaultTheme()
{
	LogTheme theme;
	return theme;
}

LogTheme LogTheme::getClassicTheme()
{
	LogTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getClassicTheme();

	theme.selectLineBgColor = QColor( 0x00, 0xC0, 0xEF );

	return theme;
}

LogTheme LogTheme::getTwilightTheme()
{
	LogTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getTwilightTheme();

	theme.selectLineBgColor = QColor( 0x00, 0x96, 0xBB );

	return theme;
}

LogTheme LogTheme::getOceanTheme()
{
	LogTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getOceanTheme();

	theme.selectLineBgColor = QColor( 0xCA, 0xF4, 0xFF );

	return theme;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const LogTheme& theme)
{
	settings.setValue("select_line_bg_color", theme.selectLineBgColor.name());

	return settings;
}

QSettings& operator>> (QSettings& settings, LogTheme& theme)
{
	theme.selectLineBgColor = QColor(settings.value("select_line_bg_color", theme.selectLineBgColor.name()).toString());

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- LogStyle
// --------------------------------------------------------------------------------
LogStyle::LogStyle(): EditStyle()
{
	theme = new LogTheme();
}

LogStyle::~LogStyle()
{}

LogStyle& LogStyle::operator =( const LogStyle& style )
{
	EditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<LogTheme*>(theme) = *static_cast<LogTheme*>(style.theme);

	return *this;
}

rbool LogStyle::operator ==( const LogStyle& style ) const
{
	rbool flag = EditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<LogTheme*>(theme) == *static_cast<LogTheme*>(style.theme);
	return flag;
}

rbool LogStyle::operator !=( const LogStyle& style ) const
{
	return !(*this == style);
}
