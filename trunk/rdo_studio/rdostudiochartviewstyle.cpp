#include "stdafx.h"
#include "rdostudiochartviewstyle.h"

using namespace rdoStyle;
using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOLogTheme
// ----------------------------------------------------------------------------
RDOStudioChartViewTheme::RDOStudioChartViewTheme(): RDOStyleTheme()
{
	axisFgColor   = RGB( 0xFF, 0x00, 0x00 );
	titleFGColor  = RGB( 0x00, 0xFF, 0x00 );
	legendFgColor = RGB( 0x00, 0x00, 0xFF );
	chartBgColor  = RGB( 0xAA, 0xAA, 0xAA );
	timeBgColor   = RGB( 0xE7, 0xF8, 0xF8 );

	titleStyle  = RDOStyleFont::BOLD;
	legendStyle = RDOStyleFont::ITALIC;
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

bool RDOStudioChartViewTheme::operator ==( const RDOStudioChartViewTheme& theme ) const
{
	bool flag = RDOStyleTheme::operator==( theme );
	
	if ( flag ) flag &= axisFgColor   == theme.axisFgColor &&
	                    titleFGColor  == theme.titleFGColor &&
	                    legendFgColor == theme.legendFgColor &&
	                    chartBgColor  == theme.chartBgColor &&
	                    timeBgColor   == theme.timeBgColor &&

	                    titleStyle  == theme.titleStyle &&
	                    legendStyle == theme.legendStyle;
	return flag;
}

bool RDOStudioChartViewTheme::operator !=( const RDOStudioChartViewTheme& theme ) const
{
	return !(*this == theme);
}

void RDOStudioChartViewTheme::load( string regPath )
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

void RDOStudioChartViewTheme::save( string regPath ) const
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

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartViewStyle
// ----------------------------------------------------------------------------
RDOStudioChartViewStyle::RDOStudioChartViewStyle() : RDOStyleWithTheme()
{
}

RDOStudioChartViewStyle::~RDOStudioChartViewStyle()
{
}

void RDOStudioChartViewStyle::initTheme()
{
	theme = new RDOStudioChartViewTheme;
}

RDOStudioChartViewStyle& RDOStudioChartViewStyle::operator =( const RDOStudioChartViewStyle& style )
{
	RDOStyleWithTheme::operator=( style );
	if ( theme  && style.theme )  *static_cast<RDOStudioChartViewTheme*>(theme) = *static_cast<RDOStudioChartViewTheme*>(style.theme);
	return *this;
}

bool RDOStudioChartViewStyle::operator ==( const RDOStudioChartViewStyle& style ) const
{
	bool flag = RDOStyleWithTheme::operator==( style );
	if ( theme  && style.theme  && flag ) flag &= *static_cast<RDOStudioChartViewTheme*>(theme) == *static_cast<RDOStudioChartViewTheme*>(style.theme);
	return flag;
}

bool RDOStudioChartViewStyle::operator !=( const RDOStudioChartViewStyle& style ) const
{
	return !(*this == style);
}

void RDOStudioChartViewStyle::init( const std::string& _regPath )
{
	RDOStyleWithTheme::init( _regPath );
	*font = rdoStyle::RDOStyleFont::getChartViewFont();
}