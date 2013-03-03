/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/frame/frame_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::style;

// --------------------------------------------------------------------------------
// -------------------- FrameTheme
// --------------------------------------------------------------------------------
FrameTheme::FrameTheme(): StyleTheme()
{
	defaultColor    = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0x00, 0x80, 0x80 );

	defaultStyle = StyleFont::NONE;
}

FrameTheme::~FrameTheme()
{}

FrameTheme& FrameTheme::operator =( const FrameTheme& theme )
{
	StyleTheme::operator=( theme );
	return *this;
}

rbool FrameTheme::operator ==( const FrameTheme& theme ) const
{
	return StyleTheme::operator==( theme );
}

rbool FrameTheme::operator !=( const FrameTheme& theme ) const
{
	return !(*this == theme);
}

FrameTheme FrameTheme::getDefaultTheme()
{
	FrameTheme theme;
	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- FrameStyle
// --------------------------------------------------------------------------------
FrameStyle::FrameStyle(): StyleWithTheme()
{
	theme = new FrameTheme();
}

FrameStyle::~FrameStyle()
{}

FrameStyle& FrameStyle::operator =( const FrameStyle& style )
{
	StyleWithTheme::operator=( style );
	if ( theme  && style.theme )  *static_cast<FrameTheme*>(theme) = *static_cast<FrameTheme*>(style.theme);

	return *this;
}

rbool FrameStyle::operator ==( const FrameStyle& style ) const
{
	rbool flag = StyleWithTheme::operator==( style );
	if ( theme  && style.theme && flag ) flag &= *static_cast<FrameTheme*>(theme) == *static_cast<FrameTheme*>(style.theme);
	return flag;
}

rbool FrameStyle::operator !=( const FrameStyle& style ) const
{
	return !(*this == style);
}

void FrameStyle::init( CREF(QString) _groupName )
{
	StyleWithTheme::init( _groupName );
	*font = StyleFont::getFrameFont();
}
