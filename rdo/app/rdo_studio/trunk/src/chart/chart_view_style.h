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
class RDOStudioChartViewTheme : public rdoStyle::RDOStyleTheme
{
public:
	RDOStudioChartViewTheme();
	virtual ~RDOStudioChartViewTheme();

	RDOStudioChartViewTheme& operator =( const RDOStudioChartViewTheme& theme );
	rbool operator ==( const RDOStudioChartViewTheme& theme ) const;
	rbool operator !=( const RDOStudioChartViewTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	COLORREF axisFgColor;
	COLORREF titleFGColor;
	COLORREF legendFgColor;
	COLORREF chartBgColor;
	COLORREF timeBgColor;

	rdoStyle::RDOStyleFont::style titleStyle;
	rdoStyle::RDOStyleFont::style legendStyle;

	static RDOStudioChartViewTheme getDefaultTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewFontsTicks
// --------------------------------------------------------------------------------
class RDOStudioChartViewFontsTicks
{
public:
	RDOStudioChartViewFontsTicks();
	virtual ~RDOStudioChartViewFontsTicks();

	RDOStudioChartViewFontsTicks& operator =( const RDOStudioChartViewFontsTicks& fonts_ticks );
	rbool operator ==( const RDOStudioChartViewFontsTicks& fonts_ticks ) const;
	rbool operator !=( const RDOStudioChartViewFontsTicks& fonts_ticks ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	int titleFontSize;
	int legendFontSize;
	int tickWidth;
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChartViewStyle
// --------------------------------------------------------------------------------
class RDOStudioChartViewStyle: public rdoStyle::RDOStyleWithTheme
{
protected:
	virtual void initTheme();
	virtual void initFontsTicks();

public:
	RDOStudioChartViewStyle();
	virtual ~RDOStudioChartViewStyle();

	RDOStudioChartViewStyle& operator =( const RDOStudioChartViewStyle& style );
	rbool operator ==( const RDOStudioChartViewStyle& style ) const;
	rbool operator !=( const RDOStudioChartViewStyle& style ) const;

	virtual void  init( CREF(tstring) _regPath = "" );
	virtual rbool load();
	virtual rbool save() const;
	
	RDOStudioChartViewTheme* getTheme() { return static_cast<RDOStudioChartViewTheme*>(theme); }

	RDOStudioChartViewFontsTicks* fonts_ticks;
};

#endif // _RDO_STUDIO_CHART_VIEW_STYLE_H_
