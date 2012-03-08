/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologstyle.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOLOGSTYLE_H_
#define _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOLOGSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/style.h"
// --------------------------------------------------------------------------------

namespace rdoTracerLog {

// --------------------------------------------------------------------------------
// -------------------- RDOLogColorPair
// --------------------------------------------------------------------------------
class RDOLogColorPair
{
public:
	RDOLogColorPair();
	virtual ~RDOLogColorPair();

	RDOLogColorPair& operator =( const RDOLogColorPair& colors );
	rbool operator ==( const RDOLogColorPair& colors ) const;
	rbool operator !=( const RDOLogColorPair& colors ) const;

	virtual void load( tstring regPath, tstring regParam );
	virtual void save( tstring regPath, tstring regParam ) const;

	COLORREF foregroundColor;
	COLORREF backgroundColor;
};

// --------------------------------------------------------------------------------
// -------------------- RDOLogTheme
// --------------------------------------------------------------------------------
class RDOLogTheme
{
public:
	RDOLogTheme();
	virtual ~RDOLogTheme();

	RDOLogTheme& operator =( const RDOLogTheme& theme );
	rbool operator ==( const RDOLogTheme& theme ) const;
	rbool operator !=( const RDOLogTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	rdoStyle::RDOStyleFont::style style;

	RDOLogColorPair defaultColor;
};

// --------------------------------------------------------------------------------
// -------------------- RDOLogBorders
// --------------------------------------------------------------------------------
class RDOLogBorders
{
public:
	RDOLogBorders();
	virtual ~RDOLogBorders();

	RDOLogBorders& operator =( const RDOLogBorders& borders );
	rbool operator ==( const RDOLogBorders& borders ) const;
	rbool operator !=( const RDOLogBorders& borders ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	int vertBorder;
	int horzBorder;
};

// --------------------------------------------------------------------------------
// -------------------- RDOLogStyle
// --------------------------------------------------------------------------------
class RDOLogStyle: public rdoStyle::RDOStyle
{
protected:
	virtual void initTheme();
	virtual void initBorders();

public:
	RDOLogStyle();
	virtual ~RDOLogStyle();

	virtual rbool getItemColors( const int index, RDOLogColorPair* &colors ) const;
	virtual rbool getItemColors( CREF(tstring) item, RDOLogColorPair* &colors ) const;

	RDOLogStyle& operator =( const RDOLogStyle& style );
	rbool operator ==( const RDOLogStyle& style ) const;
	rbool operator !=( const RDOLogStyle& style ) const;

	virtual void  init( CREF(tstring) _regPath = "" );
	virtual rbool load();
	virtual rbool save() const;

	RDOLogTheme*   theme;
	RDOLogBorders* borders;

	//static RDOLogStyle getDefaultStyle();
};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_RDO_TRACER_CTRLS_RDOLOGSTYLE_H_
