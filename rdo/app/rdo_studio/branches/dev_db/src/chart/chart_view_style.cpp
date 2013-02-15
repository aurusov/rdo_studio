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
RDOStudioChartViewTheme::RDOStudioChartViewTheme(): RDOStyleTheme()
{
	defaultColor    = RGB( 0x80, 0x80, 0x80 );
	backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	axisFgColor   = RGB( 0x00, 0x00, 0x80 );
	titleFGColor  = RGB( 0x80, 0x00, 0x00 );
	legendFgColor = RGB( 0x58, 0x46, 0x3A );
	chartBgColor  = RGB( 0xF3, 0xFC, 0xFC );
	timeBgColor   = RGB( 0xBA, 0xEB, 0xEB );

	titleStyle  = RDOStyleFont::BOLD;
	legendStyle = RDOStyleFont::NONE;
}

RDOStudioChartViewTheme::~RDOStudioChartViewTheme()
{
}

RDOStudioChartViewTheme& RDOStudioChartViewTheme::operator =( const RDOStudioChartViewTheme& theme )
{
//	defaultColor = theme.defaultColor;

	RDOStyleTheme::operator=( theme );

	axisFgColor   = theme.axisFgColor;
	titleFGColor  = theme.titleFGColor;
	legendFgColor = theme.legendFgColor;
	chartBgColor  = theme.chartBgColor;
	timeBgColor   = theme.timeBgColor;

	titleStyle  = theme.titleStyle;
	legendStyle = theme.legendStyle;

	return *this;
}

rbool RDOStudioChartViewTheme::operator ==( const RDOStudioChartViewTheme& theme ) const
{
	rbool flag = RDOStyleTheme::operator==( theme );
	
	if ( flag ) flag &= axisFgColor   == theme.axisFgColor &&
	                    titleFGColor  == theme.titleFGColor &&
	                    legendFgColor == theme.legendFgColor &&
	                    chartBgColor  == theme.chartBgColor &&
	                    timeBgColor   == theme.timeBgColor &&

	                    titleStyle  == theme.titleStyle &&
	                    legendStyle == theme.legendStyle;
	return flag;
}

rbool RDOStudioChartViewTheme::operator !=( const RDOStudioChartViewTheme& theme ) const
{
	return !(*this == theme);
}

void RDOStudioChartViewTheme::load( CREF(QString) groupName )
{
	QSettings settings;
	RDOStyleTheme::load( groupName );
	settings.beginGroup(groupName + "theme");
	axisFgColor   = QColor(settings.value("axis_fg_color", axisFgColor.name()).toString());
	titleFGColor  = QColor(settings.value("title_fg_color", titleFGColor.name()).toString());
	legendFgColor = QColor(settings.value("legend_fg_color", legendFgColor.name()).toString());
	chartBgColor  = QColor(settings.value("chart_bg_color", chartBgColor.name()).toString());
	timeBgColor   = QColor(settings.value("time_bg_color", timeBgColor.name()).toString());
	titleStyle    = static_cast<RDOStyleFont::style>(settings.value("title_style", titleStyle).toInt());
	legendStyle   = static_cast<RDOStyleFont::style>(settings.value("legend_style", legendStyle).toInt());
	settings.endGroup();
}

void RDOStudioChartViewTheme::save( CREF(QString) groupName ) const
{
	QSettings settings;
	RDOStyleTheme::save( groupName );
	settings.beginGroup(groupName + "theme");
	//! @todo qt
	settings.setValue("axis_fg_color", axisFgColor.name());
	settings.setValue("title_fg_color", titleFGColor.name());
	settings.setValue("legend_fg_color", legendFgColor.name());
	settings.setValue("chart_bg_color", chartBgColor.name());
	settings.setValue("time_bg_color", timeBgColor.name());
	settings.setValue("title_style", titleStyle);
	settings.setValue("legend_style", legendStyle);
	//AfxGetApp()->WriteProfileInt( groupName.c_str(), "axisFgColor", axisFgColor );
	//AfxGetApp()->WriteProfileInt( groupName.c_str(), "titleFGColor", titleFGColor );
	//AfxGetApp()->WriteProfileInt( groupName.c_str(), "legendFgColor", legendFgColor );
	//AfxGetApp()->WriteProfileInt( groupName.c_str(), "chartBgColor", chartBgColor );
	//AfxGetApp()->WriteProfileInt( groupName.c_str(), "timeBgColor", timeBgColor );
	//AfxGetApp()->WriteProfileInt( groupName.c_str(), "titleStyle", titleStyle );
	//AfxGetApp()->WriteProfileInt( groupName.c_str(), "legendStyle", legendStyle );
	settings.endGroup();
}

RDOStudioChartViewTheme RDOStudioChartViewTheme::getDefaultTheme()
{
	RDOStudioChartViewTheme theme;
	return theme;
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

RDOStudioChartViewFontsTicks& RDOStudioChartViewFontsTicks::operator =( const RDOStudioChartViewFontsTicks& fonts_ticks )
{
	titleFontSize  = fonts_ticks.titleFontSize;
	legendFontSize = fonts_ticks.legendFontSize;
	tickWidth      = fonts_ticks.tickWidth;

	return *this;
}

rbool RDOStudioChartViewFontsTicks::operator ==( const RDOStudioChartViewFontsTicks& fonts_ticks ) const
{
	return titleFontSize  == fonts_ticks.titleFontSize &&
	       legendFontSize == fonts_ticks.legendFontSize &&
		   tickWidth      == fonts_ticks.tickWidth;
}

rbool RDOStudioChartViewFontsTicks::operator !=( const RDOStudioChartViewFontsTicks& fonts_ticks ) const
{
	return !(*this == fonts_ticks);
}

void RDOStudioChartViewFontsTicks::load( CREF(QString) groupName )
{
	QSettings settings;
	settings.beginGroup(groupName + "fonts_ticks");
	titleFontSize  = settings.value("title_font_size", titleFontSize).toInt();
	legendFontSize = settings.value("legend_font_size", legendFontSize).toInt();
	tickWidth      = settings.value("tick_width", tickWidth).toInt();
	settings.endGroup();
}

void RDOStudioChartViewFontsTicks::save( CREF(QString) groupName ) const
{
	QSettings settings;
	settings.beginGroup(groupName + "fonts_ticks");
	settings.setValue("title_font_size", titleFontSize);
	settings.setValue("legend_font_size", legendFontSize);
	settings.setValue("tick_width", tickWidth);
	settings.endGroup();
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewStyle
// --------------------------------------------------------------------------------
RDOStudioChartViewStyle::RDOStudioChartViewStyle() : RDOStyleWithTheme()
{
}

RDOStudioChartViewStyle::~RDOStudioChartViewStyle()
{
	if ( fonts_ticks ) { delete fonts_ticks; fonts_ticks = NULL; }
}

void RDOStudioChartViewStyle::initTheme()
{
	theme = new RDOStudioChartViewTheme;
}

void RDOStudioChartViewStyle::initFontsTicks()
{
	fonts_ticks = new RDOStudioChartViewFontsTicks;
}

RDOStudioChartViewStyle& RDOStudioChartViewStyle::operator =( const RDOStudioChartViewStyle& style )
{
	RDOStyleWithTheme::operator=( style );
	if ( theme  && style.theme )  *static_cast<RDOStudioChartViewTheme*>(theme) = *static_cast<RDOStudioChartViewTheme*>(style.theme);
	if ( fonts_ticks  && style.fonts_ticks )  *fonts_ticks = *style.fonts_ticks;
	return *this;
}

rbool RDOStudioChartViewStyle::operator ==( const RDOStudioChartViewStyle& style ) const
{
	rbool flag = RDOStyleWithTheme::operator==( style );
	if ( theme  && style.theme  && flag ) flag &= *static_cast<RDOStudioChartViewTheme*>(theme) == *static_cast<RDOStudioChartViewTheme*>(style.theme);
	if ( fonts_ticks  && style.fonts_ticks && flag ) flag &= *fonts_ticks == *style.fonts_ticks;
	return flag;
}

rbool RDOStudioChartViewStyle::operator !=( const RDOStudioChartViewStyle& style ) const
{
	return !(*this == style);
}

void RDOStudioChartViewStyle::init( CREF(QString) _groupName )
{
	RDOStyleWithTheme::init( _groupName );
	*font = rdoStyle::RDOStyleFont::getChartViewFont();
	initFontsTicks();
}

rbool RDOStudioChartViewStyle::load()
{
	if ( RDOStyleWithTheme::load() ) {
		if ( fonts_ticks ) fonts_ticks->load( groupName );
		return true;
	}
	return false;
}

rbool RDOStudioChartViewStyle::save() const
{
	if ( RDOStyleWithTheme::save() ) {
		if ( fonts_ticks ) fonts_ticks->save( groupName );
		return true;
	}
	return false;
}