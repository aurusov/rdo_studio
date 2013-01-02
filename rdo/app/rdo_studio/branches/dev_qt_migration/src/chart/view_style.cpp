/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/chart/view_style.cpp
  \author    Захаров Павел
  \date      28.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/view_style.h"
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

void RDOStudioChartViewTheme::load( tstring regPath )
{
	RDOStyleTheme::load( regPath );
	regPath += "theme";

	axisFgColor   = AfxGetApp()->GetProfileInt( regPath.c_str(), "axisFgColor", axisFgColor );
	titleFGColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "titleFGColor", titleFGColor );
	legendFgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "legendFgColor", legendFgColor );
	chartBgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "chartBgColor", chartBgColor );
	timeBgColor   = AfxGetApp()->GetProfileInt( regPath.c_str(), "timeBgColor", timeBgColor );
	titleStyle    = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "titleStyle", titleStyle ));
	legendStyle   = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "legendStyle", legendStyle ));
}

void RDOStudioChartViewTheme::save( tstring regPath ) const
{
	RDOStyleTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "axisFgColor", axisFgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "titleFGColor", titleFGColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "legendFgColor", legendFgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "chartBgColor", chartBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "timeBgColor", timeBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "titleStyle", titleStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "legendStyle", legendStyle );
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

void RDOStudioChartViewFontsTicks::load( tstring regPath )
{
	regPath += "fonts_ticks";
	titleFontSize  = AfxGetApp()->GetProfileInt( regPath.c_str(), "titleFontSize", titleFontSize );
	legendFontSize = AfxGetApp()->GetProfileInt( regPath.c_str(), "legendFontSize", legendFontSize );
	tickWidth      = AfxGetApp()->GetProfileInt( regPath.c_str(), "tickWidth", tickWidth );
}

void RDOStudioChartViewFontsTicks::save( tstring regPath ) const
{
	regPath += "fonts_ticks";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "titleFontSize", titleFontSize );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "legendFontSize", legendFontSize );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "tickWidth", tickWidth );
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

void RDOStudioChartViewStyle::init( CREF(tstring) _regPath )
{
	RDOStyleWithTheme::init( _regPath );
	*font = rdoStyle::RDOStyleFont::getChartViewFont();
	initFontsTicks();
}

rbool RDOStudioChartViewStyle::load()
{
	if ( RDOStyleWithTheme::load() ) {
		if ( fonts_ticks ) fonts_ticks->load( regPath );
		return true;
	}
	return false;
}

rbool RDOStudioChartViewStyle::save() const
{
	if ( RDOStyleWithTheme::save() ) {
		if ( fonts_ticks ) fonts_ticks->save( regPath );
		return true;
	}
	return false;
}