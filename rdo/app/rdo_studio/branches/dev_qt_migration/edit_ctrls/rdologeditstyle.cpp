/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologeditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditTheme
// --------------------------------------------------------------------------------
RDOLogEditTheme::RDOLogEditTheme(): RDOBaseEditTheme()
{
	selectLineBgColor = RGB( 0x00, 0xC0, 0xEF );
}

RDOLogEditTheme::~RDOLogEditTheme()
{
}

RDOLogEditTheme& RDOLogEditTheme::operator =( const RDOLogEditTheme& theme )
{
	RDOBaseEditTheme::operator=( theme );
	selectLineBgColor = theme.selectLineBgColor;

	return *this;
}

rbool RDOLogEditTheme::operator ==( const RDOLogEditTheme& theme ) const
{
	rbool flag = RDOBaseEditTheme::operator==( theme );
	if ( flag )	flag &= selectLineBgColor == theme.selectLineBgColor ? true : false;
	return flag;
}

rbool RDOLogEditTheme::operator !=( const RDOLogEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOLogEditTheme::load( tstring regPath )
{
	RDOBaseEditTheme::load( regPath );

	regPath += "theme";
	selectLineBgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "selectLineBgColor", selectLineBgColor );
}

void RDOLogEditTheme::save( tstring regPath ) const
{
	RDOBaseEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "selectLineBgColor", selectLineBgColor );
}

RDOLogEditTheme RDOLogEditTheme::getDefaultTheme()
{
	RDOLogEditTheme theme;
	return theme;
}

RDOLogEditTheme RDOLogEditTheme::getClassicTheme()
{
	RDOLogEditTheme theme;
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getClassicTheme();

	theme.selectLineBgColor = RGB( 0x00, 0xC0, 0xEF );

	return theme;
}

RDOLogEditTheme RDOLogEditTheme::getTwilightTheme()
{
	RDOLogEditTheme theme;
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getTwilightTheme();

	theme.selectLineBgColor = RGB( 0x00, 0x96, 0xBB );

	return theme;
}

RDOLogEditTheme RDOLogEditTheme::getOceanTheme()
{
	RDOLogEditTheme theme;
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getOceanTheme();

	theme.selectLineBgColor = RGB( 0xCA, 0xF4, 0xFF );

	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditStyle
// --------------------------------------------------------------------------------
RDOLogEditStyle::RDOLogEditStyle(): RDOBaseEditStyle()
{
}

RDOLogEditStyle::~RDOLogEditStyle()
{
}

void RDOLogEditStyle::initTheme()
{
	theme = new RDOLogEditTheme;
}

RDOLogEditStyle& RDOLogEditStyle::operator =( const RDOLogEditStyle& style )
{
	RDOBaseEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOLogEditTheme*>(theme) = *static_cast<RDOLogEditTheme*>(style.theme);

	return *this;
}

rbool RDOLogEditStyle::operator ==( const RDOLogEditStyle& style ) const
{
	rbool flag = RDOBaseEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOLogEditTheme*>(theme) == *static_cast<RDOLogEditTheme*>(style.theme);
	return flag;
}

rbool RDOLogEditStyle::operator !=( const RDOLogEditStyle& style ) const
{
	return !(*this == style);
}
