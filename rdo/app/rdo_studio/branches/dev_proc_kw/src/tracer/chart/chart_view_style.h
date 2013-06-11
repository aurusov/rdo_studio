/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_view_style.h
  \author    Захаров Павел
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CHART_VIEW_STYLE_H_
#define _RDO_STUDIO_TRACER_CHART_VIEW_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- ChartViewFontsTicks
// --------------------------------------------------------------------------------
class ChartViewFontsTicks
{
public:
	ChartViewFontsTicks();
	virtual ~ChartViewFontsTicks();

	ChartViewFontsTicks& operator =(const ChartViewFontsTicks& fonts_ticks);
	rbool operator ==(const ChartViewFontsTicks& fonts_ticks) const;
	rbool operator !=(const ChartViewFontsTicks& fonts_ticks) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	int titleFontSize;
	int legendFontSize;
	int tickWidth;
};

QSettings& operator<< (QSettings& settings, const ChartViewFontsTicks& fonts_ticks);
QSettings& operator>> (QSettings& settings,       ChartViewFontsTicks& fonts_ticks);

// --------------------------------------------------------------------------------
// -------------------- ChartViewStyle
// --------------------------------------------------------------------------------
class ChartViewStyle: public style::StyleBase
{
public:
	ChartViewStyle();
	~ChartViewStyle();

	ChartViewStyle& operator =(const ChartViewStyle& style);
	rbool operator ==(const ChartViewStyle& style) const;
	rbool operator !=(const ChartViewStyle& style) const;

	QColor axisFgColor;
	QColor titleFGColor;
	QColor legendFgColor;
	QColor chartBgColor;
	QColor timeBgColor;

	style::StyleFont::style titleStyle;
	style::StyleFont::style legendStyle;

	static ChartViewStyle getDefaultStyle();

	ChartViewFontsTicks pFontsTicks;
};

QSettings& operator<< (QSettings& settings, const ChartViewStyle& style);
QSettings& operator>> (QSettings& settings,       ChartViewStyle& style);

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_VIEW_STYLE_H_
