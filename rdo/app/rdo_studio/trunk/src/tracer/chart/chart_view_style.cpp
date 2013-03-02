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
// -------------------- ChartViewTheme
// --------------------------------------------------------------------------------
ChartViewTheme::ChartViewTheme()
	: StyleTheme()
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
}

ChartViewTheme::~ChartViewTheme()
{}

ChartViewTheme& ChartViewTheme::operator =(const ChartViewTheme& theme)
{
//	defaultColor = theme.defaultColor;

	StyleTheme::operator=(theme);

	axisFgColor   = theme.axisFgColor;
	titleFGColor  = theme.titleFGColor;
	legendFgColor = theme.legendFgColor;
	chartBgColor  = theme.chartBgColor;
	timeBgColor   = theme.timeBgColor;

	titleStyle  = theme.titleStyle;
	legendStyle = theme.legendStyle;

	return *this;
}

rbool ChartViewTheme::operator ==(const ChartViewTheme& theme) const
{
	rbool flag = StyleTheme::operator==(theme);
	
	if (flag) flag &= axisFgColor   == theme.axisFgColor &&
	                  titleFGColor  == theme.titleFGColor &&
	                  legendFgColor == theme.legendFgColor &&
	                  chartBgColor  == theme.chartBgColor &&
	                  timeBgColor   == theme.timeBgColor &&
	                  titleStyle    == theme.titleStyle &&
	                  legendStyle   == theme.legendStyle;
	return flag;
}

rbool ChartViewTheme::operator !=(const ChartViewTheme& theme) const
{
	return !(*this == theme);
}

void ChartViewTheme::load(QSettings& settings)
{
	StyleTheme::load(settings);
	settings >> *this;
}

void ChartViewTheme::save(REF(QSettings) settings) const
{
	StyleTheme::save(settings);
	settings << *this;
}

ChartViewTheme ChartViewTheme::getDefaultTheme()
{
	ChartViewTheme theme;
	return theme;
}

QSettings& operator<< (QSettings& settings, const ChartViewTheme& theme)
{
	settings.setValue("axis_fg_color", theme.axisFgColor.name());
	settings.setValue("title_fg_color", theme.titleFGColor.name());
	settings.setValue("legend_fg_color", theme.legendFgColor.name());
	settings.setValue("chart_bg_color", theme.chartBgColor.name());
	settings.setValue("time_bg_color", theme.timeBgColor.name());
	settings.setValue("title_style", theme.titleStyle);
	settings.setValue("legend_style", theme.legendStyle);

	return settings;
}

QSettings& operator>> (QSettings& settings, ChartViewTheme& theme)
{
	theme.axisFgColor   = QColor(settings.value("axis_fg_color", theme.axisFgColor.name()).toString());
	theme.titleFGColor  = QColor(settings.value("title_fg_color", theme.titleFGColor.name()).toString());
	theme.legendFgColor = QColor(settings.value("legend_fg_color", theme.legendFgColor.name()).toString());
	theme.chartBgColor  = QColor(settings.value("chart_bg_color", theme.chartBgColor.name()).toString());
	theme.timeBgColor   = QColor(settings.value("time_bg_color", theme.timeBgColor.name()).toString());
	theme.titleStyle    = static_cast<StyleFont::style>(settings.value("title_style", theme.titleStyle).toInt());
	theme.legendStyle   = static_cast<StyleFont::style>(settings.value("legend_style", theme.legendStyle).toInt());

	return settings;
}

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
	: StyleWithTheme()
{
	theme       = new ChartViewTheme();
	pFontsTicks = new ChartViewFontsTicks();
}

ChartViewStyle::~ChartViewStyle()
{
	if (pFontsTicks)
	{
		delete pFontsTicks;
		pFontsTicks = NULL;
	}
}

ChartViewStyle& ChartViewStyle::operator =(const ChartViewStyle& style)
{
	StyleWithTheme::operator=(style);
	if (theme && style.theme)
	{
		*static_cast<ChartViewTheme*>(theme) = *static_cast<ChartViewTheme*>(style.theme);
	}
	if (pFontsTicks && style.pFontsTicks)
	{
		*pFontsTicks = *style.pFontsTicks;
	}
	return *this;
}

rbool ChartViewStyle::operator ==(const ChartViewStyle& style) const
{
	rbool flag = StyleWithTheme::operator==(style);
	if (theme && style.theme && flag)
	{
		flag &= *static_cast<ChartViewTheme*>(theme) == *static_cast<ChartViewTheme*>(style.theme);
	}
	if (pFontsTicks && style.pFontsTicks && flag)
	{
		flag &= *pFontsTicks == *style.pFontsTicks;
	}
	return flag;
}

rbool ChartViewStyle::operator !=(const ChartViewStyle& style) const
{
	return !(*this == style);
}

void ChartViewStyle::init(CREF(QString) _groupName)
{
	StyleWithTheme::init(_groupName);
	*font = StyleFont::getChartViewFont();
}

rbool ChartViewStyle::load()
{
	if (StyleWithTheme::load())
	{
		if (pFontsTicks)
		{
			QSettings settings;
			settings.beginGroup(groupName + "fonts_ticks");
			pFontsTicks->load(settings);
			settings.endGroup();

		}
		return true;
	}
	return false;
}

rbool ChartViewStyle::save() const
{
	if (StyleWithTheme::save())
	{
		if (pFontsTicks)
		{
			QSettings settings;
			settings.beginGroup(groupName + "fonts_ticks");
			pFontsTicks->save(settings);
			settings.endGroup();
		}
		return true;
	}
	return false;
}

}}} // namespace rdo::gui::tracer
