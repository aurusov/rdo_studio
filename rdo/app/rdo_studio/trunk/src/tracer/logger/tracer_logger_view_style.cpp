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
	: StyleBase()
	, fontStyle()
	, borders()
{
	fontStyle = StyleFont::NONE;

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

	borders.vertBorder = 1;
	borders.horzBorder = 2;

	font = StyleFont::getTracerLogFont();
}

LogStyle::~LogStyle()
{
}

rbool LogStyle::getItemColors( int index, LogColorPair &colors ) const
{
	UNUSED(index);
	return getDefaultColor(colors);
}

rbool LogStyle::getItemColors( CREF(tstring) item, LogColorPair &colors ) const
{
	if ( item.empty() )
		return LogStyle::getDefaultColor(colors);
	int posstart = item.find_first_not_of( ' ' );
	int posend = item.find_first_of( ' ', posstart );
	tstring key = boost::algorithm::trim_copy(item.substr(posstart, posend - posstart));
	rbool res = true;
	if ( key == "ES" ) {
		colors = es;
	} else if ( key == "EB" ) {
		colors = eb;
	} else if ( key == "EF" ) {
		colors = ef;
	} else if ( key == "EI" ) {
		colors = ei;
	} else if ( key == "ER" ) {
		colors = er;
	} else if ( key == "RC" ) {
		colors = rc;
	} else if ( key == "RE" ) {
		colors = re;
	} else if ( key == "RK" ) {
		colors = rk;
	} else if ( key == "V" ) {
		colors = v;
	} else if ( key == "$Status" ) {
		colors = s;
	} else if ( key.find( "DPS" ) != tstring::npos ) {
		colors = dps;
	} else if ( key == "SB" ) {
		colors = sb;
	} else if ( key == "SO" ) {
		colors = so;
	} else if ( key == "STN" ) {
		colors = stn;
	} else if ( key == "STD" ) {
		colors = std;
	} else if ( key == "STR" ) {
		colors = str;
	} else if ( key == "SRC" ) {
		colors = src;
	} else if ( key == "SRE" ) {
		colors = sre;
	} else if ( key == "SRK" ) {
		colors = srk;
	} else if ( key == "SD" ) {
		colors = sd;
	} else if ( key == "SES" ) {
		colors = ses;
	} else if ( key == "SEN" ) {
		colors = sen;
	} else if ( key == "SEM" ) {
		colors = sem;
	} else if ( key == "SEF" ) {
		colors = sef;
	} else if ( key == "SEU" ) {
		colors = seu;
	} else {
		res = LogStyle::getDefaultColor(colors);
	}
	return res;
}

rbool LogStyle::getDefaultColor(LogColorPair &colors) const
{
	colors = defaultColor;

	return false;
}

LogStyle& LogStyle::operator =( const LogStyle& style )
{
	StyleBase::operator=( style );
	fontStyle    = style.fontStyle;
	defaultColor = style.defaultColor;
	es           = style.es;
	eb           = style.eb;
	ef           = style.ef;
	ei           = style.ei;
	er           = style.er;
	rc           = style.rc;
	re           = style.re;
	rk           = style.rk;
	v            = style.v;
	s            = style.s;
	dps          = style.dps;
	sb           = style.sb;
	so           = style.so;
	stn          = style.stn;
	std          = style.std;
	str          = style.str;
	src          = style.src;
	sre          = style.sre;
	srk          = style.srk;
	sd           = style.sd;
	ses          = style.ses;
	sen          = style.sen;
	sem          = style.sem;
	sef          = style.sef;
	seu          = style.seu;
	
	borders = style.borders;

	return *this;
}

rbool LogStyle::operator ==( const LogStyle& style ) const
{
	rbool flag = StyleBase::operator==( style );
	flag &= borders == style.borders;

	return flag && fontStyle == style.fontStyle
		&& defaultColor == style.defaultColor
		&& es           == style.es
		&& eb           == style.eb
		&& ef           == style.ef
		&& ei           == style.ei
		&& er           == style.er
		&& rc           == style.rc
		&& re           == style.re
		&& rk           == style.rk
		&& v            == style.v
		&& s            == style.s
		&& dps          == style.dps
		&& sb           == style.sb
		&& so           == style.so
		&& stn          == style.stn
		&& std          == style.std
		&& str          == style.str
		&& src          == style.src
		&& sre          == style.sre
		&& srk          == style.srk
		&& sd           == style.sd
		&& ses          == style.ses
		&& sen          == style.sen
		&& sem          == style.sem
		&& sef          == style.sef
		&& seu          == style.seu;
}

rbool LogStyle::operator !=( const LogStyle& style ) const
{
	return !(*this == style);
}

LogStyle LogStyle::getDefaultStyle()
{
	LogStyle style;
	return style;
}

namespace rdo { namespace gui { namespace tracer {

QSettings& operator<< (QSettings& settings, const LogStyle& style)
{
	settings << static_cast<StyleBase>(style);

	settings.beginGroup("theme");
	settings.setValue("style", style.fontStyle);
	style.defaultColor.save( settings, "defaultColor" );
	style.es.save ( settings, "es"  );
	style.eb.save ( settings, "eb"  );
	style.ef.save ( settings, "ef"  );
	style.ei.save ( settings, "ei"  );
	style.er.save ( settings, "er"  );
	style.rc.save ( settings, "rc"  );
	style.re.save ( settings, "re"  );
	style.rk.save ( settings, "rk"  );
	style.v.save  ( settings, "v"   );
	style.s.save  ( settings, "s"   );
	style.dps.save( settings, "dps" );
	style.sb.save ( settings, "sb"  );
	style.so.save ( settings, "so"  );
	style.stn.save( settings, "stn" );
	style.std.save( settings, "std" );
	style.str.save( settings, "str" );
	style.src.save( settings, "src" );
	style.sre.save( settings, "sre" );
	style.srk.save( settings, "srk" );
	style.sd.save ( settings, "sd"  );
	style.ses.save( settings, "ses" );
	style.sen.save( settings, "sen" );
	style.sem.save( settings, "sem" );
	style.sef.save( settings, "sef" );
	style.seu.save( settings, "seu" );
	settings.endGroup();

	settings.beginGroup("borders");
	settings << style.borders;
	settings.endGroup();


	return settings;
}

QSettings& operator>> (QSettings& settings, LogStyle& style)
{
	settings >> static_cast<StyleBase&>(style);

	settings.beginGroup("theme");
	style.fontStyle = static_cast<StyleFont::style>(settings.value("style", style.fontStyle).toInt());
	style.defaultColor.load( settings, "defaultColor" );
	style.es.load ( settings, "es"  );
	style.eb.load ( settings, "eb"  );
	style.ef.load ( settings, "ef"  );
	style.ei.load ( settings, "ei"  );
	style.er.load ( settings, "er"  );
	style.rc.load ( settings, "rc"  );
	style.re.load ( settings, "re"  );
	style.rk.load ( settings, "rk"  );
	style.v.load  ( settings, "v"   );
	style.s.load  ( settings, "s"   );
	style.dps.load( settings, "dps" );
	style.sb.load ( settings, "sb"  );
	style.so.load ( settings, "so"  );
	style.stn.load( settings, "stn" );
	style.std.load( settings, "std" );
	style.str.load( settings, "str" );
	style.src.load( settings, "src" );
	style.sre.load( settings, "sre" );
	style.srk.load( settings, "srk" );
	style.sd.load ( settings, "sd"  );
	style.ses.load( settings, "ses" );
	style.sen.load( settings, "sen" );
	style.sem.load( settings, "sem" );
	style.sef.load( settings, "sef" );
	style.seu.load( settings, "seu" );
	settings.endGroup();

	settings.beginGroup("borders");
	settings >> style.borders;
	settings.endGroup();

	return settings;
}

}}} // namespace rdo::gui::tracer