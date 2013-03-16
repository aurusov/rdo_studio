/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      build_edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      25.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
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

rbool BuildStyle::operator ==( const BuildStyle& style ) const
{
	rbool flag = LogStyle::operator==( style );
	if ( flag )	flag &= warning == style.warning ? true : false;
	return flag;
}

rbool BuildStyle::operator !=( const BuildStyle& style ) const
{
	return !(*this == style);
}

void BuildStyle::loadStyle(QSettings& settings)
{
	LogStyle::loadStyle(settings);

	settings.beginGroup("theme");
	settings >> *this;
	settings.endGroup();
	
}

void BuildStyle::saveStyle(QSettings& settings) const
{
	LogStyle::saveStyle(settings);

	settings.beginGroup("theme");
	settings << *this;
	settings.endGroup();
	
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
		settings.setValue("warning", style.warning);

		return settings;
	}

	QSettings& operator>> (QSettings& settings, BuildStyle& style)
	{
		style.warning = settings.value("warning", style.warning).toBool() ? true : false;

		return settings;
	}

}}} // namespace rdo::gui::editor