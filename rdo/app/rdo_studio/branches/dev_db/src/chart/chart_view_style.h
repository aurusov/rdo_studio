/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_view_style.h
  \author    Захаров Павел
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_CHART_VIEW_STYLE_H_
#define _RDO_STUDIO_CHART_VIEW_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewTheme
// --------------------------------------------------------------------------------
class RDOStudioChartViewTheme: public rdoStyle::RDOStyleTheme
{
public:
	RDOStudioChartViewTheme();
	virtual ~RDOStudioChartViewTheme();

	RDOStudioChartViewTheme& operator =(const RDOStudioChartViewTheme& theme);
	rbool operator ==(const RDOStudioChartViewTheme& theme) const;
	rbool operator !=(const RDOStudioChartViewTheme& theme) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor axisFgColor;
	QColor titleFGColor;
	QColor legendFgColor;
	QColor chartBgColor;
	QColor timeBgColor;

	rdoStyle::RDOStyleFont::style titleStyle;
	rdoStyle::RDOStyleFont::style legendStyle;

	static RDOStudioChartViewTheme getDefaultTheme();
};

QSettings& operator<< (QSettings& settings, const RDOStudioChartViewTheme& theme);
QSettings& operator>> (QSettings& settings,       RDOStudioChartViewTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewFontsTicks
// --------------------------------------------------------------------------------
class RDOStudioChartViewFontsTicks
{
public:
	RDOStudioChartViewFontsTicks();
	virtual ~RDOStudioChartViewFontsTicks();

	RDOStudioChartViewFontsTicks& operator =(const RDOStudioChartViewFontsTicks& fonts_ticks);
	rbool operator ==(const RDOStudioChartViewFontsTicks& fonts_ticks) const;
	rbool operator !=(const RDOStudioChartViewFontsTicks& fonts_ticks) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	int titleFontSize;
	int legendFontSize;
	int tickWidth;
};

QSettings& operator<< (QSettings& settings, const RDOStudioChartViewFontsTicks& fonts_ticks);
QSettings& operator>> (QSettings& settings,       RDOStudioChartViewFontsTicks& fonts_ticks);

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewStyle
// --------------------------------------------------------------------------------
class RDOStudioChartViewStyle: public rdoStyle::RDOStyleWithTheme
{
public:
	RDOStudioChartViewStyle();
	virtual ~RDOStudioChartViewStyle();

	RDOStudioChartViewStyle& operator =(const RDOStudioChartViewStyle& style);
	rbool operator ==(const RDOStudioChartViewStyle& style) const;
	rbool operator !=(const RDOStudioChartViewStyle& style) const;

	virtual void  init(CREF(QString) _groupName = "");
	virtual rbool load();
	virtual rbool save() const;

	RDOStudioChartViewTheme* getTheme()
	{
		return static_cast<RDOStudioChartViewTheme*>(theme);
	}

	RDOStudioChartViewFontsTicks* pFontsTicks;

protected:
	virtual void initTheme();
	virtual void initFontsTicks();
};

#endif // _RDO_STUDIO_CHART_VIEW_STYLE_H_
