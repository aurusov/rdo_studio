/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobuildeditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      25.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildeditstyle.h"
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
RDOBuildEditTheme::RDOBuildEditTheme(): RDOLogEditTheme()
{
	warning = true;
}

RDOBuildEditTheme::~RDOBuildEditTheme()
{
}

RDOBuildEditTheme& RDOBuildEditTheme::operator =( const RDOBuildEditTheme& theme )
{
	RDOLogEditTheme::operator=( theme );
	warning = theme.warning;

	return *this;
}

rbool RDOBuildEditTheme::operator ==( const RDOBuildEditTheme& theme ) const
{
	rbool flag = RDOLogEditTheme::operator==( theme );
	if ( flag )	flag &= warning == theme.warning ? true : false;
	return flag;
}

rbool RDOBuildEditTheme::operator !=( const RDOBuildEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOBuildEditTheme::load( tstring regPath )
{
	RDOLogEditTheme::load( regPath );

	regPath += "build";
	warning = AfxGetApp()->GetProfileInt( regPath.c_str(), "warning", warning ) ? true : false;
}

void RDOBuildEditTheme::save( tstring regPath ) const
{
	RDOLogEditTheme::save( regPath );

	regPath += "build";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "warning", warning );
}

RDOBuildEditTheme RDOBuildEditTheme::getDefaultTheme()
{
	RDOBuildEditTheme theme;
	return theme;
}

RDOBuildEditTheme RDOBuildEditTheme::getClassicTheme()
{
	RDOBuildEditTheme theme;
	*static_cast<RDOLogEditTheme*>(&theme) = RDOLogEditTheme::getClassicTheme();

	return theme;
}

RDOBuildEditTheme RDOBuildEditTheme::getTwilightTheme()
{
	RDOBuildEditTheme theme;
	*static_cast<RDOLogEditTheme*>(&theme) = RDOLogEditTheme::getTwilightTheme();

	return theme;
}

RDOBuildEditTheme RDOBuildEditTheme::getOceanTheme()
{
	RDOBuildEditTheme theme;
	*static_cast<RDOLogEditTheme*>(&theme) = RDOLogEditTheme::getOceanTheme();

	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEditStyle
// --------------------------------------------------------------------------------
RDOBuildEditStyle::RDOBuildEditStyle(): RDOLogEditStyle()
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
	RDOLogEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOBuildEditTheme*>(theme) = *static_cast<RDOBuildEditTheme*>(style.theme);

	return *this;
}

rbool RDOBuildEditStyle::operator ==( const RDOBuildEditStyle& style ) const
{
	rbool flag = RDOLogEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOBuildEditTheme*>(theme) == *static_cast<RDOBuildEditTheme*>(style.theme);
	return flag;
}

rbool RDOBuildEditStyle::operator !=( const RDOBuildEditStyle& style ) const
{
	return !(*this == style);
}
