#ifndef RDOSTUDIOCHARTVIEWSTYLE_H
#define RDOSTUDIOCHARTVIEWSTYLE_H
#pragma once

#include "rdostudiostyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartViewTheme
// ----------------------------------------------------------------------------
class RDOStudioChartViewTheme : public rdoStyle::RDOStyleTheme
{
public:
	RDOStudioChartViewTheme();
	virtual ~RDOStudioChartViewTheme();

	RDOStudioChartViewTheme& operator =( const RDOStudioChartViewTheme& theme );
	bool operator ==( const RDOStudioChartViewTheme& theme ) const;
	bool operator !=( const RDOStudioChartViewTheme& theme ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	COLORREF axisFgColor;
	COLORREF titleFGColor;
	COLORREF legendFgColor;
	COLORREF chartBgColor;
	COLORREF timeBgColor;

	rdoStyle::RDOStyleFont::style titleStyle;
	rdoStyle::RDOStyleFont::style legendStyle;

	static RDOStudioChartViewTheme getDefaultTheme();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartViewFontsTicks
// ----------------------------------------------------------------------------
class RDOStudioChartViewFontsTicks
{
public:
	RDOStudioChartViewFontsTicks();
	virtual ~RDOStudioChartViewFontsTicks();

	RDOStudioChartViewFontsTicks& operator =( const RDOStudioChartViewFontsTicks& fonts_ticks );
	bool operator ==( const RDOStudioChartViewFontsTicks& fonts_ticks ) const;
	bool operator !=( const RDOStudioChartViewFontsTicks& fonts_ticks ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	int titleFontSize;
	int legendFontSize;
	int tickWidth;
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartViewStyle
// ----------------------------------------------------------------------------
class RDOStudioChartViewStyle: public rdoStyle::RDOStyleWithTheme
{
protected:
	virtual void initTheme();
	virtual void initFontsTicks();

public:
	RDOStudioChartViewStyle();
	virtual ~RDOStudioChartViewStyle();

	RDOStudioChartViewStyle& operator =( const RDOStudioChartViewStyle& style );
	bool operator ==( const RDOStudioChartViewStyle& style ) const;
	bool operator !=( const RDOStudioChartViewStyle& style ) const;

	virtual void init( const std::string& _regPath = "" );
	virtual bool load();
	virtual bool save() const;
	
	RDOStudioChartViewTheme* getTheme() { return static_cast<RDOStudioChartViewTheme*>(theme); }

	RDOStudioChartViewFontsTicks* fonts_ticks;
};

#endif // RDOSTUDIOCHARTVIEWSTYLE_H
