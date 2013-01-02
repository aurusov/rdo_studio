/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl_style.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_STYLE_H_
#define _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_STYLE_H_

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
class LogStyle: public rdoStyle::RDOStyle
{
protected:
	virtual void initTheme();
	virtual void initBorders();

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

	//static LogStyle getDefaultStyle();
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_STYLE_H_
