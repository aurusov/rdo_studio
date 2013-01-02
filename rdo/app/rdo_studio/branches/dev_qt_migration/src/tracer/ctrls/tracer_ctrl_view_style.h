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
// -------------------- TracerLogTheme
// --------------------------------------------------------------------------------
class TracerLogTheme: public LogTheme
{
public:
	TracerLogTheme();
	virtual ~TracerLogTheme();

	TracerLogTheme& operator =( const TracerLogTheme& theme );
	rbool operator ==( const TracerLogTheme& theme ) const;
	rbool operator !=( const TracerLogTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	LogColorPair es;
	LogColorPair eb;
	LogColorPair ef;
	LogColorPair ei;
	LogColorPair er;
	LogColorPair rc;
	LogColorPair re;
	LogColorPair rk;
	LogColorPair v;
	LogColorPair s;
	LogColorPair dps;
	LogColorPair sb;
	LogColorPair so;
	LogColorPair stn;
	LogColorPair std;
	LogColorPair str;
	LogColorPair src;
	LogColorPair sre;
	LogColorPair srk;
	LogColorPair sd;
	LogColorPair ses;
	LogColorPair sen;
	LogColorPair sem;
	LogColorPair sef;
	LogColorPair seu;

	static TracerLogTheme getDefaultTheme();
};

// --------------------------------------------------------------------------------
// -------------------- TracerLogStyle
// --------------------------------------------------------------------------------
class TracerLogStyle : public LogStyle
{
friend class RDOStudioOptionsStylesAndColors;

protected:
	virtual void initTheme();
	virtual void initBorders();

public:
	TracerLogStyle();
	virtual ~TracerLogStyle();

	rbool getItemColors( CREF(tstring) item, LogColorPair* &colors ) const;

	TracerLogStyle& operator =( const TracerLogStyle& style );
	rbool operator ==( const TracerLogStyle& style ) const;
	rbool operator !=( const TracerLogStyle& style ) const;

	virtual void init( CREF(tstring) _regPath = "" );
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_VIEW_STYLE_H_
