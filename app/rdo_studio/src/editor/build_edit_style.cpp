/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      build_edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      25.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/build_edit_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

// --------------------------------------------------------------------------------
// -------------------- BuildStyle
// --------------------------------------------------------------------------------
BuildStyle::BuildStyle(): LogStyle()
{
	warning = true;
}

BuildStyle::~BuildStyle()
{
}

BuildStyle& BuildStyle::operator =( const BuildStyle& style )
{
	LogStyle::operator=( style );
	warning = style.warning;

	return *this;
}

bool BuildStyle::operator ==( const BuildStyle& style ) const
{
	bool flag = LogStyle::operator==( style );
	if ( flag )	flag &= warning == style.warning ? true : false;
	return flag;
}

bool BuildStyle::operator !=( const BuildStyle& style ) const
{
	return !(*this == style);
}

BuildStyle BuildStyle::getDefaultStyle()
{
	BuildStyle style;
	return style;
}

BuildStyle BuildStyle::getClassicStyle()
{
	BuildStyle style;
	*static_cast<LogStyle*>(&style) = LogStyle::getClassicStyle();

	return style;
}

BuildStyle BuildStyle::getTwilightStyle()
{
	BuildStyle style;
	*static_cast<LogStyle*>(&style) = LogStyle::getTwilightStyle();

	return style;
}

BuildStyle BuildStyle::getOceanStyle()
{
	BuildStyle style;
	*static_cast<LogStyle*>(&style) = LogStyle::getOceanStyle();

	return style;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const BuildStyle& style)
{
	settings << static_cast<const LogStyle&>(style);

	settings.beginGroup("theme");
	settings.setValue("warning", style.warning);
	settings.endGroup();

	return settings;
}

QSettings& operator>> (QSettings& settings, BuildStyle& style)
{
	settings >> static_cast<LogStyle&>(style);

	settings.beginGroup("theme");
	style.warning = settings.value("warning", style.warning).toBool() ? true : false;
	settings.endGroup();

	return settings;
}

}}} // namespace rdo::gui::editor