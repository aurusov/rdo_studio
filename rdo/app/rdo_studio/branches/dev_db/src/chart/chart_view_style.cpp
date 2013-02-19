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
#include "app/rdo_studio/src/chart/chart_view_style.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewTheme
// --------------------------------------------------------------------------------
RDOStudioChartViewTheme::RDOStudioChartViewTheme()
	: RDOStyleTheme()
{
	defaultColor    = QColor(0x80, 0x80, 0x80);
	backgroundColor = QColor(0xFF, 0xFF, 0xFF);

	axisFgColor   = QColor(0x00, 0x00, 0x80);
	titleFGColor  = QColor(0x80, 0x00, 0x00);
	legendFgColor = QColor(0x58, 0x46, 0x3A);
	chartBgColor  = QColor(0xF3, 0xFC, 0xFC);
	timeBgColor   = QColor(0xBA, 0xEB, 0xEB);

	titleStyle  = RDOStyleFont::BOLD;
	legendStyle = RDOStyleFont::NONE;
}

RDOStudioChartViewTheme::~RDOStudioChartViewTheme()
{}

RDOStudioChartViewTheme& RDOStudioChartViewTheme::operator =(const RDOStudioChartViewTheme& theme)
{
//	defaultColor = theme.defaultColor;

	RDOStyleTheme::operator=(theme);

	axisFgColor   = theme.axisFgColor;
	titleFGColor  = theme.titleFGColor;
	legendFgColor = theme.legendFgColor;
	chartBgColor  = theme.chartBgColor;
	timeBgColor   = theme.timeBgColor;

	titleStyle  = theme.titleStyle;
	legendStyle = theme.legendStyle;

	return *this;
}

rbool RDOStudioChartViewTheme::operator ==(const RDOStudioChartViewTheme& theme) const
{
	rbool flag = RDOStyleTheme::operator==(theme);
	
	if (flag) flag &= axisFgColor   == theme.axisFgColor &&
	                  titleFGColor  == theme.titleFGColor &&
	                  legendFgColor == theme.legendFgColor &&
	                  chartBgColor  == theme.chartBgColor &&
	                  timeBgColor   == theme.timeBgColor &&
	                  titleStyle    == theme.titleStyle &&
	                  legendStyle   == theme.legendStyle;
	return flag;
}

rbool RDOStudioChartViewTheme::operator !=(const RDOStudioChartViewTheme& theme) const
{
	return !(*this == theme);
}

void RDOStudioChartViewTheme::load(QSettings& settings)
{
	RDOStyleTheme::load(settings);
	settings >> *this;
}

void RDOStudioChartViewTheme::save(REF(QSettings) settings) const
{
	RDOStyleTheme::save(settings);
	settings << *this;
}

RDOStudioChartViewTheme RDOStudioChartViewTheme::getDefaultTheme()
{
	RDOStudioChartViewTheme theme;
	return theme;
}

QSettings& operator<< (QSettings& settings, const RDOStudioChartViewTheme& theme)
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

QSettings& operator>> (QSettings& settings, RDOStudioChartViewTheme& theme)
{
	theme.axisFgColor   = QColor(settings.value("axis_fg_color", theme.axisFgColor.name()).toString());
	theme.titleFGColor  = QColor(settings.value("title_fg_color", theme.titleFGColor.name()).toString());
	theme.legendFgColor = QColor(settings.value("legend_fg_color", theme.legendFgColor.name()).toString());
	theme.chartBgColor  = QColor(settings.value("chart_bg_color", theme.chartBgColor.name()).toString());
	theme.timeBgColor   = QColor(settings.value("time_bg_color", theme.timeBgColor.name()).toString());
	theme.titleStyle    = static_cast<RDOStyleFont::style>(settings.value("title_style", theme.titleStyle).toInt());
	theme.legendStyle   = static_cast<RDOStyleFont::style>(settings.value("legend_style", theme.legendStyle).toInt());

	return settings;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewFontsTicks
// --------------------------------------------------------------------------------
RDOStudioChartViewFontsTicks::RDOStudioChartViewFontsTicks()
{
	titleFontSize  = 12;
	legendFontSize = 8;
	tickWidth      = 5;
}

RDOStudioChartViewFontsTicks::~RDOStudioChartViewFontsTicks()
{
}

RDOStudioChartViewFontsTicks& RDOStudioChartViewFontsTicks::operator =(const RDOStudioChartViewFontsTicks& fonts_ticks)
{
	titleFontSize  = fonts_ticks.titleFontSize;
	legendFontSize = fonts_ticks.legendFontSize;
	tickWidth      = fonts_ticks.tickWidth;

	return *this;
}

rbool RDOStudioChartViewFontsTicks::operator ==(const RDOStudioChartViewFontsTicks& fonts_ticks) const
{
	return titleFontSize  == fonts_ticks.titleFontSize &&
	       legendFontSize == fonts_ticks.legendFontSize &&
	       tickWidth      == fonts_ticks.tickWidth;
}

rbool RDOStudioChartViewFontsTicks::operator !=(const RDOStudioChartViewFontsTicks& fonts_ticks) const
{
	return !(*this == fonts_ticks);
}

void RDOStudioChartViewFontsTicks::load(QSettings& settings)
{
	settings >> *this;
}

void RDOStudioChartViewFontsTicks::save(QSettings& settings) const
{
	settings << *this;
}

QSettings& operator<< (QSettings& settings, const RDOStudioChartViewFontsTicks& fonts_ticks)
{
	settings.setValue("title_font_size", fonts_ticks.titleFontSize);
	settings.setValue("legend_font_size", fonts_ticks.legendFontSize);
	settings.setValue("tick_width", fonts_ticks.tickWidth);

	return settings;
}

QSettings& operator>> (QSettings& settings, RDOStudioChartViewFontsTicks& fonts_ticks)
{
	fonts_ticks.titleFontSize  = settings.value("title_font_size", fonts_ticks.titleFontSize).toInt();
	fonts_ticks.legendFontSize = settings.value("legend_font_size", fonts_ticks.legendFontSize).toInt();
	fonts_ticks.tickWidth      = settings.value("tick_width", fonts_ticks.tickWidth).toInt();

	return settings;
}
// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewStyle
// --------------------------------------------------------------------------------
RDOStudioChartViewStyle::RDOStudioChartViewStyle()
	: RDOStyleWithTheme()
{}

RDOStudioChartViewStyle::~RDOStudioChartViewStyle()
{
	if (pFontsTicks)
	{
		delete pFontsTicks;
		pFontsTicks = NULL;
	}
}

void RDOStudioChartViewStyle::initTheme()
{
	theme = new RDOStudioChartViewTheme;
}

void RDOStudioChartViewStyle::initFontsTicks()
{
	pFontsTicks = new RDOStudioChartViewFontsTicks;
}

RDOStudioChartViewStyle& RDOStudioChartViewStyle::operator =(const RDOStudioChartViewStyle& style)
{
	RDOStyleWithTheme::operator=(style);
	if (theme && style.theme)
	{
		*static_cast<RDOStudioChartViewTheme*>(theme) = *static_cast<RDOStudioChartViewTheme*>(style.theme);
	}
	if (pFontsTicks && style.pFontsTicks)
	{
		*pFontsTicks = *style.pFontsTicks;
	}
	return *this;
}

rbool RDOStudioChartViewStyle::operator ==(const RDOStudioChartViewStyle& style) const
{
	rbool flag = RDOStyleWithTheme::operator==(style);
	if (theme && style.theme && flag)
	{
		flag &= *static_cast<RDOStudioChartViewTheme*>(theme) == *static_cast<RDOStudioChartViewTheme*>(style.theme);
	}
	if (pFontsTicks && style.pFontsTicks && flag)
	{
		flag &= *pFontsTicks == *style.pFontsTicks;
	}
	return flag;
}

rbool RDOStudioChartViewStyle::operator !=(const RDOStudioChartViewStyle& style) const
{
	return !(*this == style);
}

void RDOStudioChartViewStyle::init(CREF(QString) _groupName)
{
	RDOStyleWithTheme::init(_groupName);
	*font = rdoStyle::RDOStyleFont::getChartViewFont();
	initFontsTicks();
}

rbool RDOStudioChartViewStyle::load()
{
	if (RDOStyleWithTheme::load())
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

rbool RDOStudioChartViewStyle::save() const
{
	if (RDOStyleWithTheme::save())
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
