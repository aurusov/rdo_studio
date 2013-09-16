/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_logger_view_style.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_LOGGER_VIEW_STYLE_H_
#define _RDO_STUDIO_TRACER_LOGGER_VIEW_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QColor>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
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

	void load(QSettings& settings, QString regParam);
	void save(QSettings& settings, QString regParam) const;

	QColor foregroundColor;
	QColor backgroundColor;
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

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	int vertBorder;
	int horzBorder;
};

QSettings& operator<< (QSettings& settings, const LogBorders& border);
QSettings& operator>> (QSettings& settings,       LogBorders& border);

// --------------------------------------------------------------------------------
// -------------------- LogStyle
// --------------------------------------------------------------------------------
class LogStyle: public style::StyleBase
{
public:
	LogStyle();
	~LogStyle();

	virtual rbool getItemColors( int index,          LogColorPair &colors ) const;
	virtual rbool getItemColors( CREF(tstring) item, LogColorPair &colors ) const;

	LogStyle& operator =( const LogStyle& style );
	rbool operator ==( const LogStyle& style ) const;
	rbool operator !=( const LogStyle& style ) const;

	static LogStyle getDefaultStyle();

	style::StyleFont::style fontStyle;

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

	LogBorders borders;

private:

	rbool getDefaultColor(LogColorPair &colors) const;
};

QSettings& operator<< (QSettings& settings, const LogStyle& style);
QSettings& operator>> (QSettings& settings,       LogStyle& style);

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_LOGGER_VIEW_STYLE_H_
