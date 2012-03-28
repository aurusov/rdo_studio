/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/frame/style.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameTheme
// --------------------------------------------------------------------------------
RDOStudioFrameTheme::RDOStudioFrameTheme(): RDOStyleTheme()
{
	defaultColor    = RGB( 0x00, 0x00, 0x00 );
	backgroundColor = RGB( 0x00, 0x80, 0x80 );

	defaultStyle = RDOStyleFont::BOLD;
}

RDOStudioFrameTheme::~RDOStudioFrameTheme()
{
}

RDOStudioFrameTheme& RDOStudioFrameTheme::operator =( const RDOStudioFrameTheme& theme )
{
	RDOStyleTheme::operator=( theme );
	return *this;
}

rbool RDOStudioFrameTheme::operator ==( const RDOStudioFrameTheme& theme ) const
{
	return RDOStyleTheme::operator==( theme );
}

rbool RDOStudioFrameTheme::operator !=( const RDOStudioFrameTheme& theme ) const
{
	return !(*this == theme);
}

RDOStudioFrameTheme RDOStudioFrameTheme::getDefaultTheme()
{
	RDOStudioFrameTheme theme;
	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameStyle
// --------------------------------------------------------------------------------
RDOStudioFrameStyle::RDOStudioFrameStyle(): RDOStyleWithTheme()
{
}

RDOStudioFrameStyle::~RDOStudioFrameStyle()
{
}

void RDOStudioFrameStyle::initTheme()
{
	theme = new RDOStudioFrameTheme;
}

RDOStudioFrameStyle& RDOStudioFrameStyle::operator =( const RDOStudioFrameStyle& style )
{
	RDOStyleWithTheme::operator=( style );
	if ( theme  && style.theme )  *static_cast<RDOStudioFrameTheme*>(theme) = *static_cast<RDOStudioFrameTheme*>(style.theme);

	return *this;
}

rbool RDOStudioFrameStyle::operator ==( const RDOStudioFrameStyle& style ) const
{
	rbool flag = RDOStyleWithTheme::operator==( style );
	if ( theme  && style.theme && flag ) flag &= *static_cast<RDOStudioFrameTheme*>(theme) == *static_cast<RDOStudioFrameTheme*>(style.theme);
	return flag;
}

rbool RDOStudioFrameStyle::operator !=( const RDOStudioFrameStyle& style ) const
{
	return !(*this == style);
}

void RDOStudioFrameStyle::init( CREF(tstring) _regPath )
{
	RDOStyleWithTheme::init( _regPath );
	*font = rdoStyle::RDOStyleFont::getFrameFont();
}
