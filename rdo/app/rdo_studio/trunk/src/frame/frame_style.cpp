/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/frame_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/frame/frame_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::frame;
using namespace rdo::gui::style;

// --------------------------------------------------------------------------------
// -------------------- FrameStyle
// --------------------------------------------------------------------------------
FrameStyle::FrameStyle(): StyleBase()
{
	defaultColor    = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0x00, 0x80, 0x80 );

	defaultStyle = StyleFont::NONE;
	font = StyleFont::getFrameFont();
}

FrameStyle::~FrameStyle()
{}

FrameStyle& FrameStyle::operator =( const FrameStyle& style )
{
	StyleBase::operator=( style );

	return *this;
}

bool FrameStyle::operator ==( const FrameStyle& style ) const
{
	bool flag = StyleBase::operator==( style );

	return flag;
}

bool FrameStyle::operator !=( const FrameStyle& style ) const
{
	return !(*this == style);
}

FrameStyle FrameStyle::getDefaultStyle()
{
	FrameStyle style;
	return style;
}