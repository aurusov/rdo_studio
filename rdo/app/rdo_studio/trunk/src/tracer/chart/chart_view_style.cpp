/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_view_style.cpp
  \author    Захаров Павел
  \date      28.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_view_style.h"
#include "app/rdo_studio/src/tracer/chart/chart_view_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::style;

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- ChartViewFontsTicks
// --------------------------------------------------------------------------------
ChartViewFontsTicks::ChartViewFontsTicks()
{
	titleFontSize  = 12;
	legendFontSize = 8;
	tickWidth      = 5;
}

ChartViewFontsTicks::~ChartViewFontsTicks()
{}

ChartViewFontsTicks& ChartViewFontsTicks::operator =(const ChartViewFontsTicks& fonts_ticks)
{
	titleFontSize  = fonts_ticks.titleFontSize;
	legendFontSize = fonts_ticks.legendFontSize;
	tickWidth      = fonts_ticks.tickWidth;

	return *this;
}

rbool ChartViewFontsTicks::operator ==(const ChartViewFontsTicks& fonts_ticks) const
{
	return titleFontSize  == fonts_ticks.titleFontSize &&
	       legendFontSize == fonts_ticks.legendFontSize &&
	       tickWidth      == fonts_ticks.tickWidth;
}

rbool ChartViewFontsTicks::operator !=(const ChartViewFontsTicks& fonts_ticks) const
{
	return !(*this == fonts_ticks);
}

void ChartViewFontsTicks::load(QSettings& settings)
{
	settings >> *this;
}

void ChartViewFontsTicks::save(QSettings& settings) const
{
	settings << *this;
}

QSettings& operator<< (QSettings& settings, const ChartViewFontsTicks& fonts_ticks)
{
	settings.setValue("title_font_size", fonts_ticks.titleFontSize);
	settings.setValue("legend_font_size", fonts_ticks.legendFontSize);
	settings.setValue("tick_width", fonts_ticks.tickWidth);

	return settings;
}

QSettings& operator>> (QSettings& settings, ChartViewFontsTicks& fonts_ticks)
{
	fonts_ticks.titleFontSize  = settings.value("title_font_size", fonts_ticks.titleFontSize).toInt();
	fonts_ticks.legendFontSize = settings.value("legend_font_size", fonts_ticks.legendFontSize).toInt();
	fonts_ticks.tickWidth      = settings.value("tick_width", fonts_ticks.tickWidth).toInt();

	return settings;
}

// --------------------------------------------------------------------------------
// -------------------- ChartViewStyle
// --------------------------------------------------------------------------------
ChartViewStyle::ChartViewStyle()
	: StyleBase()
	, pFontsTicks()
{
	defaultColor    = QColor(0x80, 0x80, 0x80);
	backgroundColor = QColor(0xFF, 0xFF, 0xFF);

	axisFgColor   = QColor(0x00, 0x00, 0x80);
	titleFGColor  = QColor(0x80, 0x00, 0x00);
	legendFgColor = QColor(0x58, 0x46, 0x3A);
	chartBgColor  = QColor(0xF3, 0xFC, 0xFC);
	timeBgColor   = QColor(0xBA, 0xEB, 0xEB);

	titleStyle  = StyleFont::BOLD;
	legendStyle = StyleFont::NONE;

	font = StyleFont::getChartViewFont();
}

ChartViewStyle::~ChartViewStyle()
{
}

ChartViewStyle& ChartViewStyle::operator =(const ChartViewStyle& style)
{
	StyleBase::operator=(style);
	//	defaultColor = style.defaultColor;

	axisFgColor   = style.axisFgColor;
	titleFGColor  = style.titleFGColor;
	legendFgColor = style.legendFgColor;
	chartBgColor  = style.chartBgColor;
	timeBgColor   = style.timeBgColor;

	titleStyle  = style.titleStyle;
	legendStyle = style.legendStyle;

	pFontsTicks = style.pFontsTicks;

	return *this;
}

rbool ChartViewStyle::operator ==(const ChartViewStyle& style) const
{
	rbool flag = StyleBase::operator==(style);

	flag &= pFontsTicks == style.pFontsTicks;

	if (flag) flag &= axisFgColor   == style.axisFgColor &&
		titleFGColor  == style.titleFGColor &&
		legendFgColor == style.legendFgColor &&
		chartBgColor  == style.chartBgColor &&
		timeBgColor   == style.timeBgColor &&
		titleStyle    == style.titleStyle &&
		legendStyle   == style.legendStyle;

	return flag;
}

rbool ChartViewStyle::operator !=(const ChartViewStyle& style) const
{
	return !(*this == style);
}

ChartViewStyle ChartViewStyle::getDefaultStyle()
{
	ChartViewStyle style;
	return style;
}

QSettings& operator<< (QSettings& settings, const ChartViewStyle& style)
{
	settings << static_cast<StyleBase>(style);

	settings.beginGroup("theme");
	settings.setValue("axis_fg_color", style.axisFgColor.name());
	settings.setValue("title_fg_color", style.titleFGColor.name());
	settings.setValue("legend_fg_color", style.legendFgColor.name());
	settings.setValue("chart_bg_color", style.chartBgColor.name());
	settings.setValue("time_bg_color", style.timeBgColor.name());
	settings.setValue("title_style", style.titleStyle);
	settings.setValue("legend_style", style.legendStyle);
	settings.endGroup();

	settings.beginGroup("fonts_ticks");
	settings << style.pFontsTicks;
	settings.endGroup();

	return settings;
}

QSettings& operator>> (QSettings& settings, ChartViewStyle& style)
{
	settings >> static_cast<StyleBase&>(style);

	settings.beginGroup("theme");	
	style.axisFgColor   = QColor(settings.value("axis_fg_color", style.axisFgColor.name()).toString());
	style.titleFGColor  = QColor(settings.value("title_fg_color", style.titleFGColor.name()).toString());
	style.legendFgColor = QColor(settings.value("legend_fg_color", style.legendFgColor.name()).toString());
	style.chartBgColor  = QColor(settings.value("chart_bg_color", style.chartBgColor.name()).toString());
	style.timeBgColor   = QColor(settings.value("time_bg_color", style.timeBgColor.name()).toString());
	style.titleStyle    = static_cast<StyleFont::style>(settings.value("title_style", style.titleStyle).toInt());
	style.legendStyle   = static_cast<StyleFont::style>(settings.value("legend_style", style.legendStyle).toInt());
	settings.endGroup();

	settings.beginGroup("fonts_ticks");
	settings >> style.pFontsTicks;
	settings.endGroup();

	return settings;
}
}}} // namespace rdo::gui::tracer
