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
// -------------------- LogStyle
// --------------------------------------------------------------------------------
LogStyle::LogStyle(): EditStyle()
{
	selectLineBgColor = QColor( 0x00, 0xC0, 0xEF );
}

LogStyle::~LogStyle()
{}

LogStyle& LogStyle::operator =( const LogStyle& style )
{
	EditStyle::operator=( style );
	selectLineBgColor = style.selectLineBgColor;

	return *this;
}

bool LogStyle::operator ==( const LogStyle& style ) const
{
	bool flag = EditStyle::operator==( style );
	return flag && selectLineBgColor == style.selectLineBgColor;
}

bool LogStyle::operator !=( const LogStyle& style ) const
{
	return !(*this == style);
}

LogStyle LogStyle::getDefaultStyle()
{
	LogStyle style;
	return style;
}

LogStyle LogStyle::getClassicStyle()
{
	LogStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getClassicStyle();

	style.selectLineBgColor = QColor( 0x00, 0xC0, 0xEF );

	return style;
}

LogStyle LogStyle::getTwilightStyle()
{
	LogStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getTwilightStyle();

	style.selectLineBgColor = QColor( 0x00, 0x96, 0xBB );

	return style;
}

LogStyle LogStyle::getOceanStyle()
{
	LogStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getOceanStyle();

	style.selectLineBgColor = QColor( 0xCA, 0xF4, 0xFF );

	return style;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const LogStyle& style)
{
	settings << static_cast<const EditStyle&>(style);
	
	settings.beginGroup("theme");
	settings.setValue("select_line_bg_color", style.selectLineBgColor.name());
	settings.endGroup();
	return settings;
}

QSettings& operator>> (QSettings& settings, LogStyle& style)
{
	settings >> static_cast<EditStyle&>(style);
	
	settings.beginGroup("theme");
	style.selectLineBgColor = QColor(settings.value("select_line_bg_color", style.selectLineBgColor.name()).toString());
	settings.endGroup();

	return settings;
}

}}} // namespace rdo::gui::editor
