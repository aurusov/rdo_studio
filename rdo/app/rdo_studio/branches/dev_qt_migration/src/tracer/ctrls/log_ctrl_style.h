/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_ctrl_style.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CTRLS_LOG_CTRL_STYLE_H_
#define _RDO_STUDIO_TRACER_CTRLS_LOG_CTRL_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qcolor.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- LogColorPair
// --------------------------------------------------------------------------------
class LogColorPair
{
public:
	LogColorPair();
	virtual ~LogColorPair();

	LogColorPair& operator =( const LogColorPair& colors );
	rbool operator ==( const LogColorPair& colors ) const;
	rbool operator !=( const LogColorPair& colors ) const;

	virtual void load( tstring regPath, tstring regParam );
	virtual void save( tstring regPath, tstring regParam ) const;

	QColor foregroundColor;
	QColor backgroundColor;
};

// --------------------------------------------------------------------------------
// -------------------- LogTheme
// --------------------------------------------------------------------------------
class LogTheme
{
public:
	LogTheme();
	virtual ~LogTheme();

	LogTheme& operator =( const LogTheme& theme );
	rbool operator ==( const LogTheme& theme ) const;
	rbool operator !=( const LogTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	rdoStyle::RDOStyleFont::style style;

	LogColorPair defaultColor;
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

	static LogTheme getDefaultTheme();
};

// --------------------------------------------------------------------------------
// -------------------- LogBorders
// --------------------------------------------------------------------------------
class LogBorders
{
public:
	LogBorders();
	virtual ~LogBorders();

	LogBorders& operator =( const LogBorders& borders );
	rbool operator ==( const LogBorders& borders ) const;
	rbool operator !=( const LogBorders& borders ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	int vertBorder;
	int horzBorder;
};

// --------------------------------------------------------------------------------
// -------------------- LogStyle
// --------------------------------------------------------------------------------
//class RDOStudioOptionsStylesAndColors;

class LogStyle: public rdoStyle::RDOStyle
{
//friend class RDOStudioOptionsStylesAndColors;

public:
	LogStyle();
	virtual ~LogStyle();

	virtual rbool getItemColors( int index, LogColorPair* &colors ) const;
	virtual rbool getItemColors( CREF(tstring) item, LogColorPair* &colors ) const;

	LogStyle& operator =( const LogStyle& style );
	rbool operator ==( const LogStyle& style ) const;
	rbool operator !=( const LogStyle& style ) const;

	virtual void  init( CREF(tstring) _regPath = "" );
	virtual rbool load();
	virtual rbool save() const;

	LogTheme*   theme;
	LogBorders* borders;

private:
	virtual void initTheme();
	        void initBorders();

	rbool getDefaultColor(LogColorPair* &colors) const;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CTRLS_LOG_CTRL_STYLE_H_
