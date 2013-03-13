/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_logger_view_style.cpp
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/algorithm/string.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/logger/tracer_logger_view_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;
using namespace rdo::gui::style;

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

void LogColorPair::load(QSettings& settings, QString regParam)
{
	regParam.append("_");
	foregroundColor = QColor(settings.value(QString(regParam + "foreground_color"), foregroundColor.name()).toString());
	backgroundColor = QColor(settings.value(QString(regParam + "background_color"), backgroundColor.name()).toString());
}

void LogColorPair::save(QSettings& settings, QString regParam) const
{
	regParam.append("_");
	settings.setValue(QString(regParam + "foreground_color"), foregroundColor.name());
	settings.setValue(QString(regParam + "background_color"), backgroundColor.name());
}

// --------------------------------------------------------------------------------
// -------------------- LogTheme
// --------------------------------------------------------------------------------
LogTheme::LogTheme()
{
	style = StyleFont::NONE;

	es.foregroundColor = QColor( 0x8B, 0x00, 0x00 );
	es.backgroundColor = QColor( 0xFF, 0xC0, 0xCB );
	
	eb.foregroundColor = QColor( 0x34, 0x4B, 0xA2 );
	eb.backgroundColor = QColor( 0xAA, 0xE3, 0xFB );
	
	ef.foregroundColor = QColor( 0x16, 0x02, 0x50 );
	ef.backgroundColor = QColor( 0x81, 0xB0, 0xD5 );
	
	ei.foregroundColor = QColor( 0x4F, 0x29, 0x62 );
	ei.backgroundColor = QColor( 0xD0, 0xD0, 0xFF );
	
	er.foregroundColor = QColor( 0x17, 0x32, 0x47 );
	er.backgroundColor = QColor( 0xB6, 0xCB, 0xDB );
	
	rc.foregroundColor = QColor( 0x23, 0x74, 0x42 );
	rc.backgroundColor = QColor( 0x96, 0xFF, 0x96 );
	
	re.foregroundColor = QColor( 0x43, 0x5A, 0x43 );
	re.backgroundColor = QColor( 0xB4, 0xE0, 0xB4 );
	
	rk.foregroundColor = QColor( 0x00, 0x86, 0x00 );
	rk.backgroundColor = QColor( 0xD0, 0xFF, 0xD0 );
	
	v.foregroundColor  = QColor( 0x00, 0x00, 0x00 );
	v.backgroundColor  = QColor( 0xF1, 0xFB, 0xE2 );
	
	s.foregroundColor  = QColor( 0x00, 0x00, 0xFF );
	s.backgroundColor  = QColor( 0xE7, 0xF8, 0xF8 );

	dps.foregroundColor = QColor( 0x03, 0x23, 0x8B );
	dps.backgroundColor = QColor( 0xC9, 0xCD, 0xDB );

	sb.foregroundColor = QColor( 0x5A, 0x4F, 0x37 );
	sb.backgroundColor = QColor( 0xF8, 0xD6, 0x8D );
	
	so.foregroundColor = QColor( 0x4B, 0x54, 0x0E );
	so.backgroundColor = QColor( 0xE6, 0xF1, 0x98 );

	stn.foregroundColor = QColor( 0x00, 0x54, 0x72 );
	stn.backgroundColor = QColor( 0xE8, 0xE8, 0xD7 );

	std.foregroundColor = QColor( 0x69, 0x55, 0x49 );
	std.backgroundColor = stn.backgroundColor;

	str.foregroundColor = QColor( 0x8B, 0x00, 0x00 );
	str.backgroundColor = stn.backgroundColor;
	
	src.foregroundColor = rc.foregroundColor;
	src.backgroundColor = rc.backgroundColor;

	sre.foregroundColor = re.foregroundColor;
	sre.backgroundColor = re.backgroundColor;

	srk.foregroundColor = rk.foregroundColor;
	srk.backgroundColor = rk.backgroundColor;

	sd.foregroundColor = QColor( 0x54, 0x1E, 0x09 );
	sd.backgroundColor = QColor( 0xF7, 0xCF, 0xB5 );

	ses.foregroundColor = QColor( 0x54, 0x1E, 0x09 );
	ses.backgroundColor = QColor( 0xF0, 0xDE, 0xDB );
	
	sen.foregroundColor = QColor( 0xF0, 0x4B, 0x30 );
	sen.backgroundColor = ses.backgroundColor;
	
	sem.foregroundColor = QColor( 0xF0, 0x4B, 0x30 );
	sem.backgroundColor = QColor( 0xE3, 0xF0, 0xF6 );
	
	sef.foregroundColor = sem.foregroundColor;
	sef.backgroundColor = sem.backgroundColor;
	
	seu.foregroundColor = sem.foregroundColor;
	seu.backgroundColor = sem.backgroundColor;
}

LogTheme::~LogTheme()
{}

LogTheme& LogTheme::operator =( const LogTheme& theme )
{
	style        = theme.style;
	defaultColor = theme.defaultColor;
	es           = theme.es;
	eb           = theme.eb;
	ef           = theme.ef;
	ei           = theme.ei;
	er           = theme.er;
	rc           = theme.rc;
	re           = theme.re;
	rk           = theme.rk;
	v            = theme.v;
	s            = theme.s;
	dps          = theme.dps;
	sb           = theme.sb;
	so           = theme.so;
	stn          = theme.stn;
	std          = theme.std;
	str          = theme.str;
	src          = theme.src;
	sre          = theme.sre;
	srk          = theme.srk;
	sd           = theme.sd;
	ses          = theme.ses;
	sen          = theme.sen;
	sem          = theme.sem;
	sef          = theme.sef;
	seu          = theme.seu;

	return *this;
}

rbool LogTheme::operator ==( const LogTheme& theme ) const
{
	return style        == theme.style
	    && defaultColor == theme.defaultColor
	    && es           == theme.es
	    && eb           == theme.eb
	    && ef           == theme.ef
	    && ei           == theme.ei
	    && er           == theme.er
	    && rc           == theme.rc
	    && re           == theme.re
	    && rk           == theme.rk
	    && v            == theme.v
	    && s            == theme.s
	    && dps          == theme.dps
	    && sb           == theme.sb
	    && so           == theme.so
	    && stn          == theme.stn
	    && std          == theme.std
	    && str          == theme.str
	    && src          == theme.src
	    && sre          == theme.sre
	    && srk          == theme.srk
	    && sd           == theme.sd
	    && ses          == theme.ses
	    && sen          == theme.sen
	    && sem          == theme.sem
	    && sef          == theme.sef
	    && seu          == theme.seu;
}

rbool LogTheme::operator !=( const LogTheme& theme ) const
{
	return !(*this == theme);
}

void LogTheme::load(QSettings& settings)
{
	settings >> *this;
}

void LogTheme::save(QSettings& settings) const
{
	settings << *this;
}

LogTheme LogTheme::getDefaultTheme()
{
	LogTheme theme;
	return theme;
}

namespace rdo { namespace gui { namespace tracer {

QSettings& operator<< (QSettings& settings, const LogTheme& theme)
{
	settings.setValue("style", theme.style);
	theme.defaultColor.save( settings, "defaultColor" );
	theme.es.save ( settings, "es"  );
	theme.eb.save ( settings, "eb"  );
	theme.ef.save ( settings, "ef"  );
	theme.ei.save ( settings, "ei"  );
	theme.er.save ( settings, "er"  );
	theme.rc.save ( settings, "rc"  );
	theme.re.save ( settings, "re"  );
	theme.rk.save ( settings, "rk"  );
	theme.v.save  ( settings, "v"   );
	theme.s.save  ( settings, "s"   );
	theme.dps.save( settings, "dps" );
	theme.sb.save ( settings, "sb"  );
	theme.so.save ( settings, "so"  );
	theme.stn.save( settings, "stn" );
	theme.std.save( settings, "std" );
	theme.str.save( settings, "str" );
	theme.src.save( settings, "src" );
	theme.sre.save( settings, "sre" );
	theme.srk.save( settings, "srk" );
	theme.sd.save ( settings, "sd"  );
	theme.ses.save( settings, "ses" );
	theme.sen.save( settings, "sen" );
	theme.sem.save( settings, "sem" );
	theme.sef.save( settings, "sef" );
	theme.seu.save( settings, "seu" );

	return settings;
}

QSettings& operator>> (QSettings& settings, LogTheme& theme)
{
	theme.style = static_cast<StyleFont::style>(settings.value("style", theme.style).toInt());
	theme.defaultColor.load( settings, "defaultColor" );
	theme.es.load ( settings, "es"  );
	theme.eb.load ( settings, "eb"  );
	theme.ef.load ( settings, "ef"  );
	theme.ei.load ( settings, "ei"  );
	theme.er.load ( settings, "er"  );
	theme.rc.load ( settings, "rc"  );
	theme.re.load ( settings, "re"  );
	theme.rk.load ( settings, "rk"  );
	theme.v.load  ( settings, "v"   );
	theme.s.load  ( settings, "s"   );
	theme.dps.load( settings, "dps" );
	theme.sb.load ( settings, "sb"  );
	theme.so.load ( settings, "so"  );
	theme.stn.load( settings, "stn" );
	theme.std.load( settings, "std" );
	theme.str.load( settings, "str" );
	theme.src.load( settings, "src" );
	theme.sre.load( settings, "sre" );
	theme.srk.load( settings, "srk" );
	theme.sd.load ( settings, "sd"  );
	theme.ses.load( settings, "ses" );
	theme.sen.load( settings, "sen" );
	theme.sem.load( settings, "sem" );
	theme.sef.load( settings, "sef" );
	theme.seu.load( settings, "seu" );

	return settings;
}

}}} // namespace rdo::gui::tracer

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

void LogBorders::load(QSettings& settings)
{
	settings >> *this;
}

void LogBorders::save(QSettings& settings) const
{
	settings << *this;
}

namespace rdo { namespace gui { namespace tracer {

QSettings& operator<< (QSettings& settings, const LogBorders& border)
{
	settings.setValue("vert_border", border.vertBorder);
	settings.setValue("horz_border", border.horzBorder);

	return settings;
}

QSettings& operator>> (QSettings& settings, LogBorders& border)
{
	border.vertBorder = settings.value("vert_border", border.vertBorder).toInt();
	border.horzBorder = settings.value("horz_border", border.horzBorder).toInt();

	return settings;
}

}}} // namespace rdo::gui::tracer

// --------------------------------------------------------------------------------
// -------------------- LogStyle
// --------------------------------------------------------------------------------
LogStyle::LogStyle()
	: Style()
	, theme   (NULL)
	, borders (NULL)
{
	theme = new LogTheme();
	borders = new LogBorders;
	borders->vertBorder = 1;
	borders->horzBorder = 2;
}

LogStyle::~LogStyle()
{
	if ( theme )   { delete theme;   theme = NULL; };
	if ( borders ) { delete borders; borders = NULL; };
}

rbool LogStyle::getItemColors( int index, LogColorPair* &colors ) const
{
	UNUSED(index);
	return getDefaultColor(colors);
}

rbool LogStyle::getItemColors( CREF(tstring) item, LogColorPair* &colors ) const
{
	if ( item.empty() )
		return LogStyle::getDefaultColor(colors);
	int posstart = item.find_first_not_of( ' ' );
	int posend = item.find_first_of( ' ', posstart );
	tstring key = boost::algorithm::trim_copy(item.substr(posstart, posend - posstart));
	rbool res = true;
	if ( theme ) {
		if ( key == "ES" ) {
			colors = &theme->es;
		} else if ( key == "EB" ) {
			colors = &theme->eb;
		} else if ( key == "EF" ) {
			colors = &theme->ef;
		} else if ( key == "EI" ) {
			colors = &theme->ei;
		} else if ( key == "ER" ) {
			colors = &theme->er;
		} else if ( key == "RC" ) {
			colors = &theme->rc;
		} else if ( key == "RE" ) {
			colors = &theme->re;
		} else if ( key == "RK" ) {
			colors = &theme->rk;
		} else if ( key == "V" ) {
			colors = &theme->v;
		} else if ( key == "$Status" ) {
			colors = &theme->s;
		} else if ( key.find( "DPS" ) != tstring::npos ) {
			colors = &theme->dps;
		} else if ( key == "SB" ) {
			colors = &theme->sb;
		} else if ( key == "SO" ) {
			colors = &theme->so;
		} else if ( key == "STN" ) {
			colors = &theme->stn;
		} else if ( key == "STD" ) {
			colors = &theme->std;
		} else if ( key == "STR" ) {
			colors = &theme->str;
		} else if ( key == "SRC" ) {
			colors = &theme->src;
		} else if ( key == "SRE" ) {
			colors = &theme->sre;
		} else if ( key == "SRK" ) {
			colors = &theme->srk;
		} else if ( key == "SD" ) {
			colors = &theme->sd;
		} else if ( key == "SES" ) {
			colors = &theme->ses;
		} else if ( key == "SEN" ) {
			colors = &theme->sen;
		} else if ( key == "SEM" ) {
			colors = &theme->sem;
		} else if ( key == "SEF" ) {
			colors = &theme->sef;
		} else if ( key == "SEU" ) {
			colors = &theme->seu;
		} else {
			res = LogStyle::getDefaultColor(colors);
		}
	} else {
		res = LogStyle::getDefaultColor(colors);
	}
	return res;
}

rbool LogStyle::getDefaultColor(LogColorPair* &colors) const
{
	if (theme)
	{
		colors = &theme->defaultColor;
	}
	return false;
}

LogStyle& LogStyle::operator =( const LogStyle& style )
{
	Style::operator=( style );
	if ( theme   && style.theme )   *theme   = *style.theme;
	if ( borders && style.borders ) *borders = *style.borders;

	return *this;
}

rbool LogStyle::operator ==( const LogStyle& style ) const
{
	rbool flag = Style::operator==( style );
	if ( theme   && style.theme   && flag ) flag &= *theme   == *style.theme;
	if ( borders && style.borders && flag ) flag &= *borders == *style.borders;
	return flag;
}

rbool LogStyle::operator !=( const LogStyle& style ) const
{
	return !(*this == style);
}

void LogStyle::init( CREF(QString) _groupName )
{
	Style::init( _groupName );
	*font = StyleFont::getTracerLogFont();
}

rbool LogStyle::load()
{
	if (Style::load()) {
		QSettings settings;
		settings.beginGroup(groupName + "theme");
		if (theme)   theme->load(settings);
		settings.endGroup();
		settings.beginGroup(groupName + "borders");
		if (borders) borders->load(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

rbool LogStyle::save() const
{
	if (Style::save()) {
		QSettings settings;
		settings.beginGroup(groupName + "theme");
		if (theme)   theme->save(settings);
		settings.endGroup();
		settings.beginGroup(groupName + "borders");
		if (borders) borders->save(settings);
		settings.endGroup();
		return true;
	}
	return false;
}
