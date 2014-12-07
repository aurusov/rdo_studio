#pragma once

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
	bool operator ==(const ChartViewFontsTicks& fonts_ticks) const;
	bool operator !=(const ChartViewFontsTicks& fonts_ticks) const;

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
	bool operator ==(const ChartViewStyle& style) const;
	bool operator !=(const ChartViewStyle& style) const;

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
