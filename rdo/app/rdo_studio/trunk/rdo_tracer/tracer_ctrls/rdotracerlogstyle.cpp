#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_tracer/tracer_ctrls/rdotracerlogstyle.h"
#include "app/rdo_studio_mfc/src/application.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracerLog;

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogTheme
// --------------------------------------------------------------------------------
RDOTracerLogTheme::RDOTracerLogTheme(): RDOLogTheme()
{
	es.foregroundColor = RGB( 0x8B, 0x00, 0x00 );
	es.backgroundColor = RGB( 0xFF, 0xC0, 0xCB );
	
	eb.foregroundColor = RGB( 0x34, 0x4B, 0xA2 );
	eb.backgroundColor = RGB( 0xAA, 0xE3, 0xFB );
	
	ef.foregroundColor = RGB( 0x16, 0x02, 0x50 );
	ef.backgroundColor = RGB( 0x81, 0xB0, 0xD5 );
	
	ei.foregroundColor = RGB( 0x4F, 0x29, 0x62 );
	ei.backgroundColor = RGB( 0xD0, 0xD0, 0xFF );
	
	er.foregroundColor = RGB( 0x17, 0x32, 0x47 );
	er.backgroundColor = RGB( 0xB6, 0xCB, 0xDB );
	
	rc.foregroundColor = RGB( 0x23, 0x74, 0x42 );
	rc.backgroundColor = RGB( 0x96, 0xFF, 0x96 );
	
	re.foregroundColor = RGB( 0x43, 0x5A, 0x43 );
	re.backgroundColor = RGB( 0xB4, 0xE0, 0xB4 );
	
	rk.foregroundColor = RGB( 0x00, 0x86, 0x00 );
	rk.backgroundColor = RGB( 0xD0, 0xFF, 0xD0 );
	
	v.foregroundColor  = RGB( 0x00, 0x00, 0x00 );
	v.backgroundColor  = RGB( 0xF1, 0xFB, 0xE2 );
	
	s.foregroundColor  = RGB( 0x00, 0x00, 0xFF );
	s.backgroundColor  = RGB( 0xE7, 0xF8, 0xF8 );

	dps.foregroundColor = RGB( 0x03, 0x23, 0x8B );
	dps.backgroundColor = RGB( 0xC9, 0xCD, 0xDB );

	sb.foregroundColor = RGB( 0x5A, 0x4F, 0x37 );
	sb.backgroundColor = RGB( 0xF8, 0xD6, 0x8D );
	
	so.foregroundColor = RGB( 0x4B, 0x54, 0x0E );
	so.backgroundColor = RGB( 0xE6, 0xF1, 0x98 );

	stn.foregroundColor = RGB( 0x00, 0x54, 0x72 );
	stn.backgroundColor = RGB( 0xE8, 0xE8, 0xD7 );

	std.foregroundColor = RGB( 0x69, 0x55, 0x49 );
	std.backgroundColor = stn.backgroundColor;

	str.foregroundColor = RGB( 0x8B, 0x00, 0x00 );
	str.backgroundColor = stn.backgroundColor;
	
	src.foregroundColor = rc.foregroundColor;
	src.backgroundColor = rc.backgroundColor;

	sre.foregroundColor = re.foregroundColor;
	sre.backgroundColor = re.backgroundColor;

	srk.foregroundColor = rk.foregroundColor;
	srk.backgroundColor = rk.backgroundColor;

	sd.foregroundColor = RGB( 0x54, 0x1E, 0x09 );
	sd.backgroundColor = RGB( 0xF7, 0xCF, 0xB5 );

	ses.foregroundColor = RGB( 0x54, 0x1E, 0x09 );
	ses.backgroundColor = RGB( 0xF0, 0xDE, 0xDB );
	
	sen.foregroundColor = RGB( 0xF0, 0x4B, 0x30 );
	sen.backgroundColor = ses.backgroundColor;
	
	sem.foregroundColor = RGB( 0xF0, 0x4B, 0x30 );
	sem.backgroundColor = RGB( 0xE3, 0xF0, 0xF6 );
	
	sef.foregroundColor = sem.foregroundColor;
	sef.backgroundColor = sem.backgroundColor;
	
	seu.foregroundColor = sem.foregroundColor;
	seu.backgroundColor = sem.backgroundColor;
}

RDOTracerLogTheme::~RDOTracerLogTheme()
{
}

RDOTracerLogTheme& RDOTracerLogTheme::operator =( const RDOTracerLogTheme& theme )
{
	RDOLogTheme::operator=( theme );
	
	es  = theme.es;
	eb  = theme.eb;
	ef  = theme.ef;
	ei  = theme.ei;
	er  = theme.er;
	rc  = theme.rc;
	re  = theme.re;
	rk  = theme.rk;
	v   = theme.v;
	s   = theme.s;
	dps = theme.dps;
	sb  = theme.sb;
	so  = theme.so;
	stn = theme.stn;
	std = theme.std;
	str = theme.str;
	src = theme.src;
	sre = theme.sre;
	srk = theme.srk;
	sd  = theme.sd;
	ses = theme.ses;
	sen = theme.sen;
	sem = theme.sem;
	sef = theme.sef;
	seu = theme.seu;

	return *this;
}

bool RDOTracerLogTheme::operator ==( const RDOTracerLogTheme& theme ) const
{
	bool flag = RDOLogTheme::operator==( theme );
	if ( flag )	flag &= es  == theme.es;
	if ( flag )	flag &= eb  == theme.eb;
	if ( flag )	flag &= ef  == theme.ef;
	if ( flag )	flag &= ei  == theme.ei;
	if ( flag )	flag &= er  == theme.er;
	if ( flag )	flag &= rc  == theme.rc;
	if ( flag )	flag &= re  == theme.re;
	if ( flag )	flag &= rk  == theme.rk;
	if ( flag )	flag &= v   == theme.v;
	if ( flag )	flag &= s   == theme.s;
	if ( flag )	flag &= dps == theme.dps;
	if ( flag )	flag &= sb  == theme.sb;
	if ( flag )	flag &= so  == theme.so;
	if ( flag )	flag &= stn == theme.stn;
	if ( flag )	flag &= std == theme.std;
	if ( flag )	flag &= str == theme.str;
	if ( flag )	flag &= src == theme.src;
	if ( flag )	flag &= sre == theme.sre;
	if ( flag )	flag &= srk == theme.srk;
	if ( flag )	flag &= sd  == theme.sd;
	if ( flag )	flag &= ses == theme.ses;
	if ( flag )	flag &= sen == theme.sen;
	if ( flag )	flag &= sem == theme.sem;
	if ( flag )	flag &= sef == theme.sef;
	if ( flag )	flag &= seu == theme.seu;
	return flag;
}

bool RDOTracerLogTheme::operator !=( const RDOTracerLogTheme& theme ) const
{
	return !(*this == theme);
}

void RDOTracerLogTheme::load( std::string regPath )
{
	RDOLogTheme::load( regPath );

	regPath += "theme";
	es.load( regPath, "es" );
	eb.load( regPath, "eb" );
	ef.load( regPath, "ef" );
	ei.load( regPath, "ei" );
	er.load( regPath, "er" );
	rc.load( regPath, "rc" );
	re.load( regPath, "re" );
	rk.load( regPath, "rk" );
	v.load( regPath, "v" );
	s.load( regPath, "s" );
	dps.load( regPath, "dps" );
	sb.load( regPath, "sb" );
	so.load( regPath, "so" );
	stn.load( regPath, "stn" );
	std.load( regPath, "std" );
	str.load( regPath, "str" );
	src.load( regPath, "src" );
	sre.load( regPath, "sre" );
	srk.load( regPath, "srk" );
	sd.load( regPath, "sd" );
	ses.load( regPath, "ses" );
	sen.load( regPath, "sen" );
	sem.load( regPath, "sem" );
	sef.load( regPath, "sef" );
	seu.load( regPath, "seu" );
}

void RDOTracerLogTheme::save( std::string regPath ) const
{
	RDOLogTheme::save( regPath );

	regPath += "theme";
	es.save( regPath, "es" );
	eb.save( regPath, "eb" );
	ef.save( regPath, "ef" );
	ei.save( regPath, "ei" );
	er.save( regPath, "er" );
	rc.save( regPath, "rc" );
	re.save( regPath, "re" );
	rk.save( regPath, "rk" );
	v.save( regPath, "v" );
	s.save( regPath, "s" );
	dps.save( regPath, "dps" );
	sb.save( regPath, "sb" );
	so.save( regPath, "so" );
	stn.save( regPath, "stn" );
	std.save( regPath, "std" );
	str.save( regPath, "str" );
	src.save( regPath, "src" );
	sre.save( regPath, "sre" );
	srk.save( regPath, "srk" );
	sd.save( regPath, "sd" );
	ses.save( regPath, "ses" );
	sen.save( regPath, "sen" );
	sem.save( regPath, "sem" );
	sef.save( regPath, "sef" );
	seu.save( regPath, "seu" );
}

RDOTracerLogTheme RDOTracerLogTheme::getDefaultTheme()
{
	RDOTracerLogTheme theme;
	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- RDOTracerLogStyle
// --------------------------------------------------------------------------------
RDOTracerLogStyle::RDOTracerLogStyle() : RDOLogStyle()
{
}

RDOTracerLogStyle::~RDOTracerLogStyle()
{
}

void RDOTracerLogStyle::initTheme()
{
	theme = new RDOTracerLogTheme;
}

void RDOTracerLogStyle::initBorders()
{
	RDOLogStyle::initBorders();
	borders->vertBorder = 1;
	borders->horzBorder = 2;
}

bool RDOTracerLogStyle::getItemColors( const std::string& item, RDOLogColorPair* &colors ) const
{
	if ( item.empty() )
		return RDOLogStyle::getItemColors( "", colors );
	int posstart = item.find_first_not_of( ' ' );
	int posend = item.find_first_of( ' ', posstart );
	std::string key = item.substr( posstart, posend - posstart );
	rdo::trim( key );
	bool res = true;
	if ( theme ) {
		RDOTracerLogTheme* _theme = static_cast<RDOTracerLogTheme*>(theme);
		if ( key == "ES" ) {
			colors = &_theme->es;
		} else if ( key == "EB" ) {
			colors = &_theme->eb;
		} else if ( key == "EF" ) {
			colors = &_theme->ef;
		} else if ( key == "EI" ) {
			colors = &_theme->ei;
		} else if ( key == "ER" ) {
			colors = &_theme->er;
		} else if ( key == "RC" ) {
			colors = &_theme->rc;
		} else if ( key == "RE" ) {
			colors = &_theme->re;
		} else if ( key == "RK" ) {
			colors = &_theme->rk;
		} else if ( key == "V" ) {
			colors = &_theme->v;
		} else if ( key == "$Status" ) {
			colors = &_theme->s;
		} else if ( key.find( "DPS" ) != std::string::npos ) {
			colors = &_theme->dps;
		} else if ( key == "SB" ) {
			colors = &_theme->sb;
		} else if ( key == "SO" ) {
			colors = &_theme->so;
		} else if ( key == "STN" ) {
			colors = &_theme->stn;
		} else if ( key == "STD" ) {
			colors = &_theme->std;
		} else if ( key == "STR" ) {
			colors = &_theme->str;
		} else if ( key == "SRC" ) {
			colors = &_theme->src;
		} else if ( key == "SRE" ) {
			colors = &_theme->sre;
		} else if ( key == "SRK" ) {
			colors = &_theme->srk;
		} else if ( key == "SD" ) {
			colors = &_theme->sd;
		} else if ( key == "SES" ) {
			colors = &_theme->ses;
		} else if ( key == "SEN" ) {
			colors = &_theme->sen;
		} else if ( key == "SEM" ) {
			colors = &_theme->sem;
		} else if ( key == "SEF" ) {
			colors = &_theme->sef;
		} else if ( key == "SEU" ) {
			colors = &_theme->seu;
		} else {
			res = RDOLogStyle::getItemColors( "", colors );
		}
	} else {
		res = RDOLogStyle::getItemColors( "", colors );
	}
	return res;
}

RDOTracerLogStyle& RDOTracerLogStyle::operator =( const RDOTracerLogStyle& style )
{
	RDOLogStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOTracerLogTheme*>(theme) = *static_cast<RDOTracerLogTheme*>(style.theme);

	return *this;
}

bool RDOTracerLogStyle::operator ==( const RDOTracerLogStyle& style ) const
{
	bool flag = RDOLogStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOTracerLogTheme*>(theme) == *static_cast<RDOTracerLogTheme*>(style.theme);
	return flag;
}

bool RDOTracerLogStyle::operator !=( const RDOTracerLogStyle& style ) const
{
	return !(*this == style);
}

void RDOTracerLogStyle::init( const std::string& _regPath )
{
	RDOLogStyle::init( _regPath );
	*font = rdoStyle::RDOStyleFont::getTracerLogFont();
}
