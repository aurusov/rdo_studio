/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl_style.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_style.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;
using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- LogColorPair
// --------------------------------------------------------------------------------
LogColorPair::LogColorPair()
{
	foregroundColor = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );
}

LogColorPair::~LogColorPair()
{
}

LogColorPair& LogColorPair::operator =( const LogColorPair& colors )
{
	foregroundColor = colors.foregroundColor;
	backgroundColor = colors.backgroundColor;

	return *this;
}

rbool LogColorPair::operator ==( const LogColorPair& colors ) const
{
	return foregroundColor == colors.foregroundColor &&
	       backgroundColor == colors.backgroundColor;
}

rbool LogColorPair::operator !=( const LogColorPair& colors ) const
{
	return !(*this == colors);
}

void LogColorPair::load( tstring regPath, tstring regParam )
{
	regParam += "_%s";
	//! todo qt
	//foregroundColor = QColor(QString::fromStdString((LPCTSTR)AfxGetApp()->GetProfileString( regPath.c_str(), rdo::format( regParam.c_str(), "foregroundColor" ).c_str(), foregroundColor.name().toStdString().c_str() )));
	//backgroundColor = QColor(QString::fromStdString((LPCTSTR)AfxGetApp()->GetProfileString( regPath.c_str(), rdo::format( regParam.c_str(), "backgroundColor" ).c_str(), backgroundColor.name().toStdString().c_str() )));
}

void LogColorPair::save( tstring regPath, tstring regParam ) const
{
	regParam += "_%s";
	AfxGetApp()->WriteProfileString( regPath.c_str(), rdo::format( regParam.c_str(), "foregroundColor" ).c_str(), foregroundColor.name().toStdString().c_str() );
	AfxGetApp()->WriteProfileString( regPath.c_str(), rdo::format( regParam.c_str(), "backgroundColor" ).c_str(), backgroundColor.name().toStdString().c_str() );
}

// --------------------------------------------------------------------------------
// -------------------- LogTheme
// --------------------------------------------------------------------------------
LogTheme::LogTheme()
{
	style = RDOStyleFont::NONE;
}

LogTheme::~LogTheme()
{
}

LogTheme& LogTheme::operator =( const LogTheme& theme )
{
	defaultColor = theme.defaultColor;

	style = theme.style;

	return *this;
}

rbool LogTheme::operator ==( const LogTheme& theme ) const
{
	return defaultColor == theme.defaultColor &&

	       style == theme.style;
}

rbool LogTheme::operator !=( const LogTheme& theme ) const
{
	return !(*this == theme);
}

void LogTheme::load( tstring regPath )
{
	regPath += "theme";
	defaultColor.load( regPath, "defaultColor" );
	style = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "style", style ));
}

void LogTheme::save( tstring regPath ) const
{
	regPath += "theme";
	defaultColor.save( regPath, "defaultColor" );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "style", style );
}

// --------------------------------------------------------------------------------
// -------------------- LogBorders
// --------------------------------------------------------------------------------
LogBorders::LogBorders()
{
	vertBorder = 0;
	horzBorder = 0;
}

LogBorders::~LogBorders()
{
}

LogBorders& LogBorders::operator =( const LogBorders& borders )
{
	vertBorder = borders.vertBorder;
	horzBorder = borders.horzBorder;

	return *this;
}

rbool LogBorders::operator ==( const LogBorders& borders ) const
{
	return vertBorder == borders.vertBorder &&
	       horzBorder == borders.horzBorder;
}

rbool LogBorders::operator !=( const LogBorders& borders ) const
{
	return !(*this == borders);
}

void LogBorders::load( tstring regPath )
{
	regPath += "borders";
	vertBorder = AfxGetApp()->GetProfileInt( regPath.c_str(), "vertBorder", vertBorder );
	horzBorder = AfxGetApp()->GetProfileInt( regPath.c_str(), "horzBorder", horzBorder );
}

void LogBorders::save( tstring regPath ) const
{
	regPath += "borders";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "vertBorder", vertBorder );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "horzBorder", horzBorder );
}

// --------------------------------------------------------------------------------
// -------------------- LogStyle
// --------------------------------------------------------------------------------
LogStyle::LogStyle() :
	RDOStyle(),
	theme( NULL ),
	borders( NULL )
{
}

LogStyle::~LogStyle()
{
	if ( theme )   { delete theme;   theme = NULL; };
	if ( borders ) { delete borders; borders = NULL; };
}

void LogStyle::initTheme()
{
	theme = new LogTheme;
}

void LogStyle::initBorders()
{
	borders = new LogBorders;
}

rbool LogStyle::getItemColors( int index, LogColorPair* &colors ) const
{
	UNUSED(index);

	if ( theme ) {
		colors = &theme->defaultColor;
	}
	return false;
}

rbool LogStyle::getItemColors( CREF(tstring) item, LogColorPair* &colors ) const
{
	UNUSED(item);

	if ( theme ) {
		colors = &theme->defaultColor;
	}
	return false;
}

LogStyle& LogStyle::operator =( const LogStyle& style )
{
	RDOStyle::operator=( style );
	if ( theme   && style.theme )   *theme   = *style.theme;
	if ( borders && style.borders ) *borders = *style.borders;

	return *this;
}

rbool LogStyle::operator ==( const LogStyle& style ) const
{
	rbool flag = RDOStyle::operator==( style );
	if ( theme   && style.theme   && flag ) flag &= *theme   == *style.theme;
	if ( borders && style.borders && flag ) flag &= *borders == *style.borders;
	return flag;
}

rbool LogStyle::operator !=( const LogStyle& style ) const
{
	return !(*this == style);
}

/*LogStyle LogStyle::getDefaultStyle()
{
	LogStyle style;
	return style;
}*/

void LogStyle::init( CREF(tstring) _regPath )
{
	RDOStyle::init( _regPath );
	initTheme();
	initBorders();
}

rbool LogStyle::load()
{
	if ( RDOStyle::load() ) {
		if ( theme )   theme->load( regPath );
		if ( borders ) borders->load( regPath );
		return true;
	}
	return false;
}

rbool LogStyle::save() const
{
	if ( RDOStyle::save() ) {
		if ( theme )   theme->save( regPath );
		if ( borders ) borders->save( regPath );
		return true;
	}
	return false;
}
