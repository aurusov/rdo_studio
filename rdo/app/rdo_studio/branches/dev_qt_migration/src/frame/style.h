/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_STYLE_H_
#define _RDO_STUDIO_MFC_FRAME_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/style.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameTheme
// --------------------------------------------------------------------------------
class RDOStudioFrameTheme: public rdoStyle::RDOStyleTheme
{
public:
	RDOStudioFrameTheme();
	virtual ~RDOStudioFrameTheme();

	RDOStudioFrameTheme& operator =( const RDOStudioFrameTheme& theme );
	rbool operator ==( const RDOStudioFrameTheme& theme ) const;
	rbool operator !=( const RDOStudioFrameTheme& theme ) const;

	static RDOStudioFrameTheme getDefaultTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameStyle
// --------------------------------------------------------------------------------
class RDOStudioFrameStyle: public rdoStyle::RDOStyleWithTheme
{
protected:
	virtual void initTheme();

public:
	RDOStudioFrameStyle();
	virtual ~RDOStudioFrameStyle();

	RDOStudioFrameStyle& operator =( const RDOStudioFrameStyle& style );
	rbool operator ==( const RDOStudioFrameStyle& style ) const;
	rbool operator !=( const RDOStudioFrameStyle& style ) const;

	virtual void init( CREF(tstring) _regPath = "" );
};

#endif // _RDO_STUDIO_MFC_FRAME_STYLE_H_
