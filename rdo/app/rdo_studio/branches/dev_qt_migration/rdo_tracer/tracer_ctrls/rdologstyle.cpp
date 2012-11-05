/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologstyle.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdologstyle.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracerLog;
using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- RDOLogColorPair
// --------------------------------------------------------------------------------
RDOLogColorPair::RDOLogColorPair()
{
	foregroundColor = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );
}

RDOLogColorPair::~RDOLogColorPair()
{
}

RDOLogColorPair& RDOLogColorPair::operator =( const RDOLogColorPair& colors )
{
	foregroundColor = colors.foregroundColor;
	backgroundColor = colors.backgroundColor;

	return *this;
}

rbool RDOLogColorPair::operator ==( const RDOLogColorPair& colors ) const
{
	return foregroundColor == colors.foregroundColor &&
	       backgroundColor == colors.backgroundColor;
}

rbool RDOLogColorPair::operator !=( const RDOLogColorPair& colors ) const
{
	return !(*this == colors);
}

void RDOLogColorPair::load( tstring regPath, tstring regParam )
{
	regParam += "_%s";
	//! todo qt
	//foregroundColor = QColor(QString::fromStdString((LPCTSTR)AfxGetApp()->GetProfileString( regPath.c_str(), rdo::format( regParam.c_str(), "foregroundColor" ).c_str(), foregroundColor.name().toStdString().c_str() )));
	//backgroundColor = QColor(QString::fromStdString((LPCTSTR)AfxGetApp()->GetProfileString( regPath.c_str(), rdo::format( regParam.c_str(), "backgroundColor" ).c_str(), backgroundColor.name().toStdString().c_str() )));
}

void RDOLogColorPair::save( tstring regPath, tstring regParam ) const
{
	regParam += "_%s";
	AfxGetApp()->WriteProfileString( regPath.c_str(), rdo::format( regParam.c_str(), "foregroundColor" ).c_str(), foregroundColor.name().toStdString().c_str() );
	AfxGetApp()->WriteProfileString( regPath.c_str(), rdo::format( regParam.c_str(), "backgroundColor" ).c_str(), backgroundColor.name().toStdString().c_str() );
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogTheme
// --------------------------------------------------------------------------------
RDOLogTheme::RDOLogTheme()
{
	style = RDOStyleFont::NONE;
}

RDOLogTheme::~RDOLogTheme()
{
}

RDOLogTheme& RDOLogTheme::operator =( const RDOLogTheme& theme )
{
	defaultColor = theme.defaultColor;

	style = theme.style;

	return *this;
}

rbool RDOLogTheme::operator ==( const RDOLogTheme& theme ) const
{
	return defaultColor == theme.defaultColor &&

	       style == theme.style;
}

rbool RDOLogTheme::operator !=( const RDOLogTheme& theme ) const
{
	return !(*this == theme);
}

void RDOLogTheme::load( tstring regPath )
{
	regPath += "theme";
	defaultColor.load( regPath, "defaultColor" );
	style = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "style", style ));
}

void RDOLogTheme::save( tstring regPath ) const
{
	regPath += "theme";
	defaultColor.save( regPath, "defaultColor" );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "style", style );
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogBorders
// --------------------------------------------------------------------------------
RDOLogBorders::RDOLogBorders()
{
	vertBorder = 0;
	horzBorder = 0;
}

RDOLogBorders::~RDOLogBorders()
{
}

RDOLogBorders& RDOLogBorders::operator =( const RDOLogBorders& borders )
{
	vertBorder = borders.vertBorder;
	horzBorder = borders.horzBorder;

	return *this;
}

rbool RDOLogBorders::operator ==( const RDOLogBorders& borders ) const
{
	return vertBorder == borders.vertBorder &&
	       horzBorder == borders.horzBorder;
}

rbool RDOLogBorders::operator !=( const RDOLogBorders& borders ) const
{
	return !(*this == borders);
}

void RDOLogBorders::load( tstring regPath )
{
	regPath += "borders";
	vertBorder = AfxGetApp()->GetProfileInt( regPath.c_str(), "vertBorder", vertBorder );
	horzBorder = AfxGetApp()->GetProfileInt( regPath.c_str(), "horzBorder", horzBorder );
}

void RDOLogBorders::save( tstring regPath ) const
{
	regPath += "borders";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "vertBorder", vertBorder );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "horzBorder", horzBorder );
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogStyle
// --------------------------------------------------------------------------------
RDOLogStyle::RDOLogStyle() :
	RDOStyle(),
	theme( NULL ),
	borders( NULL )
{
}

RDOLogStyle::~RDOLogStyle()
{
	if ( theme )   { delete theme;   theme = NULL; };
	if ( borders ) { delete borders; borders = NULL; };
}

void RDOLogStyle::initTheme()
{
	theme = new RDOLogTheme;
}

void RDOLogStyle::initBorders()
{
	borders = new RDOLogBorders;
}

rbool RDOLogStyle::getItemColors( const int index, RDOLogColorPair* &colors ) const
{
	UNUSED(index);

	if ( theme ) {
		colors = &theme->defaultColor;
	}
	return false;
}

rbool RDOLogStyle::getItemColors( CREF(tstring) item, RDOLogColorPair* &colors ) const
{
	UNUSED(item);

	if ( theme ) {
		colors = &theme->defaultColor;
	}
	return false;
}

RDOLogStyle& RDOLogStyle::operator =( const RDOLogStyle& style )
{
	RDOStyle::operator=( style );
	if ( theme   && style.theme )   *theme   = *style.theme;
	if ( borders && style.borders ) *borders = *style.borders;

	return *this;
}

rbool RDOLogStyle::operator ==( const RDOLogStyle& style ) const
{
	rbool flag = RDOStyle::operator==( style );
	if ( theme   && style.theme   && flag ) flag &= *theme   == *style.theme;
	if ( borders && style.borders && flag ) flag &= *borders == *style.borders;
	return flag;
}

rbool RDOLogStyle::operator !=( const RDOLogStyle& style ) const
{
	return !(*this == style);
}

/*RDOLogStyle RDOLogStyle::getDefaultStyle()
{
	RDOLogStyle style;
	return style;
}*/

void RDOLogStyle::init( CREF(tstring) _regPath )
{
	RDOStyle::init( _regPath );
	initTheme();
	initBorders();
}

rbool RDOLogStyle::load()
{
	if ( RDOStyle::load() ) {
		if ( theme )   theme->load( regPath );
		if ( borders ) borders->load( regPath );
		return true;
	}
	return false;
}

rbool RDOLogStyle::save() const
{
	if ( RDOStyle::save() ) {
		if ( theme )   theme->save( regPath );
		if ( borders ) borders->save( regPath );
		return true;
	}
	return false;
}
