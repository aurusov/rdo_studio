/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_FRAME_STYLE_H_
#define _RDO_STUDIO_FRAME_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace frame {

// --------------------------------------------------------------------------------
// -------------------- FrameTheme
// --------------------------------------------------------------------------------
class FrameTheme: public rdo::gui::style::StyleTheme
{
public:
	FrameTheme();
	virtual ~FrameTheme();

	FrameTheme& operator =( const FrameTheme& theme );
	rbool operator ==( const FrameTheme& theme ) const;
	rbool operator !=( const FrameTheme& theme ) const;

	static FrameTheme getDefaultTheme();
};

// --------------------------------------------------------------------------------
// -------------------- FrameStyle
// --------------------------------------------------------------------------------
class FrameStyle: public rdo::gui::style::StyleWithTheme
{
public:
	FrameStyle();
	virtual ~FrameStyle();

	FrameStyle& operator =( const FrameStyle& style );
	rbool operator ==( const FrameStyle& style ) const;
	rbool operator !=( const FrameStyle& style ) const;

	void init( CREF(QString) _groupName = "" );
};

}}} // namespace rdo::gui::frame

#endif // _RDO_STUDIO_FRAME_STYLE_H_
