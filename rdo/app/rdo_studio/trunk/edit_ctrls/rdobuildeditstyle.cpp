/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobuildeditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      25.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobuildeditstyle.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEditTheme
// --------------------------------------------------------------------------------
RDOBuildEditTheme::RDOBuildEditTheme(): LogEditTheme()
{
	warning = true;
}

RDOBuildEditTheme::~RDOBuildEditTheme()
{
}

RDOBuildEditTheme& RDOBuildEditTheme::operator =( const RDOBuildEditTheme& theme )
{
	LogEditTheme::operator=( theme );
	warning = theme.warning;

	return *this;
}

rbool RDOBuildEditTheme::operator ==( const RDOBuildEditTheme& theme ) const
{
	rbool flag = LogEditTheme::operator==( theme );
	if ( flag )	flag &= warning == theme.warning ? true : false;
	return flag;
}

rbool RDOBuildEditTheme::operator !=( const RDOBuildEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOBuildEditTheme::load( QString regPath )
{
	LogEditTheme::load( regPath );
	
	QSettings settings;
	settings.beginGroup(regPath + "build");
	warning = settings.value("warning", warning).toBool() ? true : false;
	settings.endGroup();

}

void RDOBuildEditTheme::save( QString regPath ) const
{
	LogEditTheme::save( regPath );

	QSettings settings;
	settings.beginGroup(regPath + "build");
	settings.setValue("warning", warning);
	settings.endGroup();
}

RDOBuildEditTheme RDOBuildEditTheme::getDefaultTheme()
{
	RDOBuildEditTheme theme;
	return theme;
}

RDOBuildEditTheme RDOBuildEditTheme::getClassicTheme()
{
	RDOBuildEditTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getClassicTheme();

	return theme;
}

RDOBuildEditTheme RDOBuildEditTheme::getTwilightTheme()
{
	RDOBuildEditTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getTwilightTheme();

	return theme;
}

RDOBuildEditTheme RDOBuildEditTheme::getOceanTheme()
{
	RDOBuildEditTheme theme;
	*static_cast<LogEditTheme*>(&theme) = LogEditTheme::getOceanTheme();

	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEditStyle
// --------------------------------------------------------------------------------
RDOBuildEditStyle::RDOBuildEditStyle(): LogEditStyle()
{
}

RDOBuildEditStyle::~RDOBuildEditStyle()
{
}

void RDOBuildEditStyle::initTheme()
{
	theme = new RDOBuildEditTheme;
}

RDOBuildEditStyle& RDOBuildEditStyle::operator =( const RDOBuildEditStyle& style )
{
	LogEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOBuildEditTheme*>(theme) = *static_cast<RDOBuildEditTheme*>(style.theme);

	return *this;
}

rbool RDOBuildEditStyle::operator ==( const RDOBuildEditStyle& style ) const
{
	rbool flag = LogEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOBuildEditTheme*>(theme) == *static_cast<RDOBuildEditTheme*>(style.theme);
	return flag;
}

rbool RDOBuildEditStyle::operator !=( const RDOBuildEditStyle& style ) const
{
	return !(*this == style);
}
