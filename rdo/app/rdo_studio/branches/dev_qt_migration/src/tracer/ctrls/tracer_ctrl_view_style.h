/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl_view_style.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_VIEW_STYLE_H_
#define _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_VIEW_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_style.h"
// --------------------------------------------------------------------------------

class RDOStudioOptionsStylesAndColors;

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogTheme
// --------------------------------------------------------------------------------
class RDOTracerLogTheme: public RDOLogTheme
{
public:
	RDOTracerLogTheme();
	virtual ~RDOTracerLogTheme();

	RDOTracerLogTheme& operator =( const RDOTracerLogTheme& theme );
	rbool operator ==( const RDOTracerLogTheme& theme ) const;
	rbool operator !=( const RDOTracerLogTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	RDOLogColorPair es;
	RDOLogColorPair eb;
	RDOLogColorPair ef;
	RDOLogColorPair ei;
	RDOLogColorPair er;
	RDOLogColorPair rc;
	RDOLogColorPair re;
	RDOLogColorPair rk;
	RDOLogColorPair v;
	RDOLogColorPair s;
	RDOLogColorPair dps;
	RDOLogColorPair sb;
	RDOLogColorPair so;
	RDOLogColorPair stn;
	RDOLogColorPair std;
	RDOLogColorPair str;
	RDOLogColorPair src;
	RDOLogColorPair sre;
	RDOLogColorPair srk;
	RDOLogColorPair sd;
	RDOLogColorPair ses;
	RDOLogColorPair sen;
	RDOLogColorPair sem;
	RDOLogColorPair sef;
	RDOLogColorPair seu;

	static RDOTracerLogTheme getDefaultTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogStyle
// --------------------------------------------------------------------------------
class RDOTracerLogStyle : public RDOLogStyle
{
friend class RDOStudioOptionsStylesAndColors;

protected:
	virtual void initTheme();
	virtual void initBorders();

public:
	RDOTracerLogStyle();
	virtual ~RDOTracerLogStyle();

	rbool getItemColors( CREF(tstring) item, RDOLogColorPair* &colors ) const;

	RDOTracerLogStyle& operator =( const RDOTracerLogStyle& style );
	rbool operator ==( const RDOTracerLogStyle& style ) const;
	rbool operator !=( const RDOTracerLogStyle& style ) const;

	virtual void init( CREF(tstring) _regPath = "" );
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_VIEW_STYLE_H_
