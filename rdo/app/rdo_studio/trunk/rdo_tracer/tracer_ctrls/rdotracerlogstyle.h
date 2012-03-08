/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerlogstyle.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGSTYLE_H_
#define _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdologstyle.h"
// --------------------------------------------------------------------------------

class RDOStudioOptionsStylesAndColors;

namespace rdoTracerLog {

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogTheme
// --------------------------------------------------------------------------------
class RDOTracerLogTheme : public RDOLogTheme
{
public:
	RDOTracerLogTheme();
	virtual ~RDOTracerLogTheme();

	RDOTracerLogTheme& operator =( const RDOTracerLogTheme& theme );
	bool operator ==( const RDOTracerLogTheme& theme ) const;
	bool operator !=( const RDOTracerLogTheme& theme ) const;

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

	bool getItemColors( CREF(tstring) item, RDOLogColorPair* &colors ) const;

	RDOTracerLogStyle& operator =( const RDOTracerLogStyle& style );
	bool operator ==( const RDOTracerLogStyle& style ) const;
	bool operator !=( const RDOTracerLogStyle& style ) const;

	virtual void init( CREF(tstring) _regPath = "" );
};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOTRACERLOGSTYLE_H_
