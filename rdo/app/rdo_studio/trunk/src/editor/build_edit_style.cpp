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
// -------------------- BuildTheme
// --------------------------------------------------------------------------------
BuildTheme::BuildTheme(): LogEditTheme()
{
	warning = true;
}

BuildTheme::~BuildTheme()
{
}

BuildTheme& BuildTheme::operator =( const BuildTheme& theme )
{
	LogEditTheme::operator=( theme );
	warning = theme.warning;

	return *this;
}

rbool BuildTheme::operator ==( const BuildTheme& theme ) const
{
	rbool flag = LogEditTheme::operator==( theme );
	if ( flag )	flag &= warning == theme.warning ? true : false;
	return flag;
}

rbool BuildTheme::operator !=( const BuildTheme& theme ) const
{
	return !(*this == theme);
}

void BuildTheme::load(QSettings& settings)
{
	LogEditTheme::load(settings);
	settings >> *this;
}

void BuildTheme::save(QSettings& settings) const
{
	LogEditTheme::save(settings);
	settings << *this;
}

BuildTheme BuildTheme::getDefaultTheme()
{
	BuildTheme theme;
	return theme;
}

BuildTheme BuildTheme::getClassicTheme()
{
	BuildTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getClassicTheme();

	return theme;
}

BuildTheme BuildTheme::getTwilightTheme()
{
	BuildTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getTwilightTheme();

	return theme;
}

BuildTheme BuildTheme::getOceanTheme()
{
	BuildTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getOceanTheme();

	return theme;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const BuildTheme& theme)
{
	settings.setValue("warning", theme.warning);

	return settings;
}

QSettings& operator>> (QSettings& settings, BuildTheme& theme)
{
	theme.warning = settings.value("warning", theme.warning).toBool() ? true : false;

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- BuildStyle
// --------------------------------------------------------------------------------
BuildStyle::BuildStyle(): LogEditStyle()
{
	theme = new BuildTheme();
}

BuildStyle::~BuildStyle()
{
}

BuildStyle& BuildStyle::operator =( const BuildStyle& style )
{
	LogEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<BuildTheme*>(theme) = *static_cast<BuildTheme*>(style.theme);

	return *this;
}

rbool BuildStyle::operator ==( const BuildStyle& style ) const
{
	rbool flag = LogEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<BuildTheme*>(theme) == *static_cast<BuildTheme*>(style.theme);
	return flag;
}

rbool BuildStyle::operator !=( const BuildStyle& style ) const
{
	return !(*this == style);
}
