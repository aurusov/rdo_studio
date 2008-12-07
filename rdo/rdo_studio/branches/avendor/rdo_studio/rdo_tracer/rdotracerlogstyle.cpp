#include "stdafx.h"
#include "rdotracerlogstyle.h"
#include "../rdostudioapp.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerLogStyle
// ----------------------------------------------------------------------------
RDOTracerLogStyle::RDOTracerLogStyle() : RDOLogStyle()
{
	vertBorder = 1;
	horzBorder = 2;

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

RDOTracerLogStyle::~RDOTracerLogStyle()
{
}

bool RDOTracerLogStyle::getItemColors( const CString& item, COLORREF& textColor, COLORREF& backColor ) const
{
	CString key = item.SpanExcluding( " " );
	key.TrimLeft();
	key.TrimRight();
	bool res = true;
	if ( key == _T("ES") ) {
		textColor = es.foregroundColor;
		backColor = es.backgroundColor;
	} else if ( key == _T("EB") ) {
		textColor = eb.foregroundColor;
		backColor = eb.backgroundColor;
	} else if ( key == _T("EF") ) {
		textColor = ef.foregroundColor;
		backColor = ef.backgroundColor;
	} else if ( key == _T("EI") ) {
		textColor = ei.foregroundColor;
		backColor = ei.backgroundColor;
	} else if ( key == _T("ER") ) {
		textColor = er.foregroundColor;
		backColor = er.backgroundColor;
	} else if ( key == _T("RC") ) {
		textColor = rc.foregroundColor;
		backColor = rc.backgroundColor;
	} else if ( key == _T("RE") ) {
		textColor = re.foregroundColor;
		backColor = re.backgroundColor;
	} else if ( key == _T("RK") ) {
		textColor = rk.foregroundColor;
		backColor = rk.backgroundColor;
	} else if ( key == _T("V") ) {
		textColor = v.foregroundColor;
		backColor = v.backgroundColor;
	} else if ( key == _T("$Status") ) {
		textColor = s.foregroundColor;
		backColor = s.backgroundColor;
	} else if ( key.Find( "DPS", 0) != -1 ) {
		textColor = dps.foregroundColor;
		backColor = dps.backgroundColor;
	} else if ( key == _T("SB") ) {
		textColor = sb.foregroundColor;
		backColor = sb.backgroundColor;
	} else if ( key == _T("SO") ) {
		textColor = so.foregroundColor;
		backColor = so.backgroundColor;
	} else if ( key == _T("STN") ) {
		textColor = stn.foregroundColor;
		backColor = stn.backgroundColor;
	} else if ( key == _T("STD") ) {
		textColor = std.foregroundColor;
		backColor = std.backgroundColor;
	} else if ( key == _T("STR") ) {
		textColor = str.foregroundColor;
		backColor = str.backgroundColor;
	} else if ( key == _T("SRK") ) {
		textColor = srk.foregroundColor;
		backColor = srk.backgroundColor;
	} else if ( key == _T("SRC") ) {
		textColor = src.foregroundColor;
		backColor = src.backgroundColor;
	} else if ( key == _T("SRE") ) {
		textColor = sre.foregroundColor;
		backColor = sre.backgroundColor;
	} else if ( key == _T("SRK") ) {
		textColor = srk.foregroundColor;
		backColor = srk.backgroundColor;
	} else if ( key == _T("SD") ) {
		textColor = sd.foregroundColor;
		backColor = sd.backgroundColor;
	} else if ( key == _T("SES") ) {
		textColor = ses.foregroundColor;
		backColor = ses.backgroundColor;
	} else if ( key == _T("SEN") ) {
		textColor = sen.foregroundColor;
		backColor = sen.backgroundColor;
	} else if ( key == _T("SEM") ) {
		textColor = sem.foregroundColor;
		backColor = sem.backgroundColor;
	} else if ( key == _T("SEF") ) {
		textColor = sef.foregroundColor;
		backColor = sef.backgroundColor;
	} else if ( key == _T("SEU") ) {
		textColor = seu.foregroundColor;
		backColor = seu.backgroundColor;
	} else {
		res = false;
	}
	return res;
}

RDOTracerLogStyle& RDOTracerLogStyle::operator =( const RDOTracerLogStyle& style )
{
	*((RDOLogStyle*)this) = style;
	es.foregroundColor = style.es.foregroundColor;
	es.backgroundColor = style.es.backgroundColor;
	
	eb.foregroundColor = style.eb.foregroundColor;
	eb.backgroundColor = style.eb.backgroundColor;
	
	ef.foregroundColor = style.ef.foregroundColor;
	ef.backgroundColor = style.ef.backgroundColor;
	
	ei.foregroundColor = style.ei.foregroundColor;
	ei.backgroundColor = style.ei.backgroundColor;
	
	er.foregroundColor = style.er.foregroundColor;
	er.backgroundColor = style.er.backgroundColor;
	
	rc.foregroundColor = style.rc.foregroundColor;
	rc.backgroundColor = style.rc.backgroundColor;
	
	re.foregroundColor = style.re.foregroundColor;
	re.backgroundColor = style.re.backgroundColor;
	
	rk.foregroundColor = style.rk.foregroundColor;
	rk.backgroundColor = style.rk.backgroundColor;
	
	v.foregroundColor  = style.v.foregroundColor;
	v.backgroundColor  = style.v.backgroundColor;
	
	s.foregroundColor  = style.s.foregroundColor;
	s.backgroundColor  = style.s.backgroundColor;

	dps.foregroundColor = style.dps.foregroundColor;
	dps.backgroundColor = style.dps.backgroundColor;

	sb.foregroundColor = style.sb.foregroundColor;
	sb.backgroundColor = style.sb.backgroundColor;
	
	so.foregroundColor = style.so.foregroundColor;
	so.backgroundColor = style.so.backgroundColor;

	stn.foregroundColor = style.stn.foregroundColor;
	stn.backgroundColor = style.stn.backgroundColor;

	std.foregroundColor = style.std.foregroundColor;
	std.backgroundColor = style.std.backgroundColor;

	str.foregroundColor = style.str.foregroundColor;
	str.backgroundColor = style.str.backgroundColor;
	
	src.foregroundColor = style.src.foregroundColor;
	src.backgroundColor = style.src.backgroundColor;

	sre.foregroundColor = style.sre.foregroundColor;
	sre.backgroundColor = style.sre.backgroundColor;

	srk.foregroundColor = style.srk.foregroundColor;
	srk.backgroundColor = style.srk.backgroundColor;

	sd.foregroundColor = style.sd.foregroundColor;
	sd.backgroundColor = style.sd.backgroundColor;

	ses.foregroundColor = style.ses.foregroundColor;
	ses.backgroundColor = style.ses.backgroundColor;
	
	sen.foregroundColor = style.sen.foregroundColor;
	sen.backgroundColor = style.sen.backgroundColor;
	
	sem.foregroundColor = style.sem.foregroundColor;
	sem.backgroundColor = style.sem.backgroundColor;
	
	sef.foregroundColor = style.sef.foregroundColor;
	sef.backgroundColor = style.sef.backgroundColor;
	
	seu.foregroundColor = style.seu.foregroundColor;
	seu.backgroundColor = style.seu.backgroundColor;

	return *this;
}

bool RDOTracerLogStyle::operator ==( const RDOTracerLogStyle& style ) const
{
	return  *((RDOLogStyle*)this) == style &&
			es.foregroundColor == style.es.foregroundColor &&
			es.backgroundColor == style.es.backgroundColor &&
			
			eb.foregroundColor == style.eb.foregroundColor &&
			eb.backgroundColor == style.eb.backgroundColor &&
			
			ef.foregroundColor == style.ef.foregroundColor &&
			ef.backgroundColor == style.ef.backgroundColor &&
			
			ei.foregroundColor == style.ei.foregroundColor &&
			ei.backgroundColor == style.ei.backgroundColor &&
			
			er.foregroundColor == style.er.foregroundColor &&
			er.backgroundColor == style.er.backgroundColor &&
			
			rc.foregroundColor == style.rc.foregroundColor &&
			rc.backgroundColor == style.rc.backgroundColor &&
			
			re.foregroundColor == style.re.foregroundColor &&
			re.backgroundColor == style.re.backgroundColor &&
			
			rk.foregroundColor == style.rk.foregroundColor &&
			rk.backgroundColor == style.rk.backgroundColor &&
			
			v.foregroundColor  == style.v.foregroundColor &&
			v.backgroundColor  == style.v.backgroundColor &&
			
			s.foregroundColor  == style.s.foregroundColor &&
			s.backgroundColor  == style.s.backgroundColor &&

			dps.foregroundColor == style.dps.foregroundColor &&
			dps.backgroundColor == style.dps.backgroundColor &&

			sb.foregroundColor == style.sb.foregroundColor &&
			sb.backgroundColor == style.sb.backgroundColor &&
			
			so.foregroundColor == style.so.foregroundColor &&
			so.backgroundColor == style.so.backgroundColor &&

			stn.foregroundColor == style.stn.foregroundColor &&
			stn.backgroundColor == style.stn.backgroundColor &&

			std.foregroundColor == style.std.foregroundColor &&
			std.backgroundColor == style.std.backgroundColor &&

			str.foregroundColor == style.str.foregroundColor &&
			str.backgroundColor == style.str.backgroundColor &&
			
			src.foregroundColor == style.src.foregroundColor &&
			src.backgroundColor == style.src.backgroundColor &&

			sre.foregroundColor == style.sre.foregroundColor &&
			sre.backgroundColor == style.sre.backgroundColor &&

			srk.foregroundColor == style.srk.foregroundColor &&
			srk.backgroundColor == style.srk.backgroundColor &&

			sd.foregroundColor == style.sd.foregroundColor &&
			sd.backgroundColor == style.sd.backgroundColor &&

			ses.foregroundColor == style.ses.foregroundColor &&
			ses.backgroundColor == style.ses.backgroundColor &&
			
			sen.foregroundColor == style.sen.foregroundColor &&
			sen.backgroundColor == style.sen.backgroundColor &&
			
			sem.foregroundColor == style.sem.foregroundColor &&
			sem.backgroundColor == style.sem.backgroundColor &&
			
			sef.foregroundColor == style.sef.foregroundColor &&
			sef.backgroundColor == style.sef.backgroundColor &&
			
			seu.foregroundColor == style.seu.foregroundColor &&
			seu.backgroundColor == style.seu.backgroundColor;
}

bool RDOTracerLogStyle::operator !=( const RDOTracerLogStyle& style ) const
{
	return !(*this == style);
}

RDOTracerLogStyle RDOTracerLogStyle::getDefaultStyle()
{
	RDOTracerLogStyle style;
	return style;
}

void RDOTracerLogStyle::load()
{
	RDOLogStyle::load();
	
	CWinApp* app = AfxGetApp();

	es.foregroundColor  = app->GetProfileInt( "logStyle", "es_foregroundColor", es.foregroundColor );
	es.backgroundColor  = app->GetProfileInt( "logStyle", "es_backgroundColor", es.backgroundColor );
	eb.foregroundColor  = app->GetProfileInt( "logStyle", "eb_foregroundColor", eb.foregroundColor );
	eb.backgroundColor  = app->GetProfileInt( "logStyle", "eb_backgroundColor", eb.backgroundColor );
	ef.foregroundColor  = app->GetProfileInt( "logStyle", "ef_foregroundColor", ef.foregroundColor );
	ef.backgroundColor  = app->GetProfileInt( "logStyle", "ef_backgroundColor", ef.backgroundColor );
	eb.foregroundColor  = app->GetProfileInt( "logStyle", "eb_foregroundColor", eb.foregroundColor );
	eb.backgroundColor  = app->GetProfileInt( "logStyle", "eb_backgroundColor", eb.backgroundColor );
	ei.foregroundColor  = app->GetProfileInt( "logStyle", "ei_foregroundColor", ei.foregroundColor );
	ei.backgroundColor  = app->GetProfileInt( "logStyle", "ei_backgroundColor", ei.backgroundColor );
	er.foregroundColor  = app->GetProfileInt( "logStyle", "er_foregroundColor", er.foregroundColor );
	er.backgroundColor  = app->GetProfileInt( "logStyle", "er_backgroundColor", er.backgroundColor );
	rc.foregroundColor  = app->GetProfileInt( "logStyle", "rc_foregroundColor", rc.foregroundColor );
	rc.backgroundColor  = app->GetProfileInt( "logStyle", "rc_backgroundColor", rc.backgroundColor );
	re.foregroundColor  = app->GetProfileInt( "logStyle", "re_foregroundColor", re.foregroundColor );
	re.backgroundColor  = app->GetProfileInt( "logStyle", "re_backgroundColor", re.backgroundColor );
	rk.foregroundColor  = app->GetProfileInt( "logStyle", "rk_foregroundColor", rk.foregroundColor );
	rk.backgroundColor  = app->GetProfileInt( "logStyle", "rk_backgroundColor", rk.backgroundColor );
	v.foregroundColor   = app->GetProfileInt( "logStyle", "v_foregroundColor", v.foregroundColor );
	v.backgroundColor   = app->GetProfileInt( "logStyle", "v_backgroundColor", v.backgroundColor );
	s.foregroundColor   = app->GetProfileInt( "logStyle", "s_foregroundColor", s.foregroundColor );
	s.backgroundColor   = app->GetProfileInt( "logStyle", "s_backgroundColor", s.backgroundColor );
	dps.foregroundColor = app->GetProfileInt( "logStyle", "dps_foregroundColor", dps.foregroundColor );
	dps.backgroundColor = app->GetProfileInt( "logStyle", "dps_backgroundColor", dps.backgroundColor );
	sb.foregroundColor  = app->GetProfileInt( "logStyle", "sb_foregroundColor", sb.foregroundColor );
	sb.backgroundColor  = app->GetProfileInt( "logStyle", "sb_backgroundColor", sb.backgroundColor );
	so.foregroundColor  = app->GetProfileInt( "logStyle", "so_foregroundColor", so.foregroundColor );
	so.backgroundColor  = app->GetProfileInt( "logStyle", "so_backgroundColor", so.backgroundColor );
	stn.foregroundColor = app->GetProfileInt( "logStyle", "stn_foregroundColor", stn.foregroundColor );
	stn.backgroundColor = app->GetProfileInt( "logStyle", "stn_backgroundColor", stn.backgroundColor );
	std.foregroundColor = app->GetProfileInt( "logStyle", "std_foregroundColor", std.foregroundColor );
	std.backgroundColor = app->GetProfileInt( "logStyle", "std_backgroundColor", std.backgroundColor );
	str.foregroundColor = app->GetProfileInt( "logStyle", "str_foregroundColor", str.foregroundColor );
	str.backgroundColor = app->GetProfileInt( "logStyle", "str_backgroundColor", str.backgroundColor );
	src.foregroundColor = app->GetProfileInt( "logStyle", "src_foregroundColor", src.foregroundColor );
	src.backgroundColor = app->GetProfileInt( "logStyle", "src_backgroundColor", src.backgroundColor );
	sre.foregroundColor = app->GetProfileInt( "logStyle", "sre_foregroundColor", sre.foregroundColor );
	sre.backgroundColor = app->GetProfileInt( "logStyle", "sre_backgroundColor", sre.backgroundColor );
	srk.foregroundColor = app->GetProfileInt( "logStyle", "srk_foregroundColor", srk.foregroundColor );
	srk.backgroundColor = app->GetProfileInt( "logStyle", "srk_backgroundColor", srk.backgroundColor );
	sd.foregroundColor  = app->GetProfileInt( "logStyle", "sd_foregroundColor", sd.foregroundColor );
	sd.backgroundColor  = app->GetProfileInt( "logStyle", "sd_backgroundColor", sd.backgroundColor );
	ses.foregroundColor = app->GetProfileInt( "logStyle", "ses_foregroundColor", ses.foregroundColor );
	ses.backgroundColor = app->GetProfileInt( "logStyle", "ses_backgroundColor", ses.backgroundColor );
	sen.foregroundColor = app->GetProfileInt( "logStyle", "sen_foregroundColor", sen.foregroundColor );
	sen.backgroundColor = app->GetProfileInt( "logStyle", "sen_backgroundColor", sen.backgroundColor );
	sem.foregroundColor = app->GetProfileInt( "logStyle", "sem_foregroundColor", sem.foregroundColor );
	sem.backgroundColor = app->GetProfileInt( "logStyle", "sem_backgroundColor", sem.backgroundColor );
	sef.foregroundColor = app->GetProfileInt( "logStyle", "sef_foregroundColor", sef.foregroundColor );
	sef.backgroundColor = app->GetProfileInt( "logStyle", "sef_backgroundColor", sef.backgroundColor );
	seu.foregroundColor = app->GetProfileInt( "logStyle", "seu_foregroundColor", seu.foregroundColor );
	seu.backgroundColor = app->GetProfileInt( "logStyle", "seu_backgroundColor", seu.backgroundColor );
}

void RDOTracerLogStyle::save() const
{
	RDOLogStyle::save();
	
	CWinApp* app = AfxGetApp();

	app->WriteProfileInt( "logStyle", "es_foregroundColor", es.foregroundColor );
	app->WriteProfileInt( "logStyle", "es_backgroundColor", es.backgroundColor );
	app->WriteProfileInt( "logStyle", "eb_foregroundColor", eb.foregroundColor );
	app->WriteProfileInt( "logStyle", "eb_backgroundColor", eb.backgroundColor );
	app->WriteProfileInt( "logStyle", "ef_foregroundColor", ef.foregroundColor );
	app->WriteProfileInt( "logStyle", "ef_backgroundColor", ef.backgroundColor );
	app->WriteProfileInt( "logStyle", "eb_foregroundColor", eb.foregroundColor );
	app->WriteProfileInt( "logStyle", "eb_backgroundColor", eb.backgroundColor );
	app->WriteProfileInt( "logStyle", "ei_foregroundColor", ei.foregroundColor );
	app->WriteProfileInt( "logStyle", "ei_backgroundColor", ei.backgroundColor );
	app->WriteProfileInt( "logStyle", "er_foregroundColor", er.foregroundColor );
	app->WriteProfileInt( "logStyle", "er_backgroundColor", er.backgroundColor );
	app->WriteProfileInt( "logStyle", "rc_foregroundColor", rc.foregroundColor );
	app->WriteProfileInt( "logStyle", "rc_backgroundColor", rc.backgroundColor );
	app->WriteProfileInt( "logStyle", "re_foregroundColor", re.foregroundColor );
	app->WriteProfileInt( "logStyle", "re_backgroundColor", re.backgroundColor );
	app->WriteProfileInt( "logStyle", "rk_foregroundColor", rk.foregroundColor );
	app->WriteProfileInt( "logStyle", "rk_backgroundColor", rk.backgroundColor );
	app->WriteProfileInt( "logStyle", "v_foregroundColor", v.foregroundColor );
	app->WriteProfileInt( "logStyle", "v_backgroundColor", v.backgroundColor );
	app->WriteProfileInt( "logStyle", "s_foregroundColor", s.foregroundColor );
	app->WriteProfileInt( "logStyle", "s_backgroundColor", s.backgroundColor );
	app->WriteProfileInt( "logStyle", "dps_foregroundColor", dps.foregroundColor );
	app->WriteProfileInt( "logStyle", "dps_backgroundColor", dps.backgroundColor );
	app->WriteProfileInt( "logStyle", "sb_foregroundColor", sb.foregroundColor );
	app->WriteProfileInt( "logStyle", "sb_backgroundColor", sb.backgroundColor );
	app->WriteProfileInt( "logStyle", "so_foregroundColor", so.foregroundColor );
	app->WriteProfileInt( "logStyle", "so_backgroundColor", so.backgroundColor );
	app->WriteProfileInt( "logStyle", "stn_foregroundColor", stn.foregroundColor );
	app->WriteProfileInt( "logStyle", "stn_backgroundColor", stn.backgroundColor );
	app->WriteProfileInt( "logStyle", "std_foregroundColor", std.foregroundColor );
	app->WriteProfileInt( "logStyle", "std_backgroundColor", std.backgroundColor );
	app->WriteProfileInt( "logStyle", "str_foregroundColor", str.foregroundColor );
	app->WriteProfileInt( "logStyle", "str_backgroundColor", str.backgroundColor );
	app->WriteProfileInt( "logStyle", "src_foregroundColor", src.foregroundColor );
	app->WriteProfileInt( "logStyle", "src_backgroundColor", src.backgroundColor );
	app->WriteProfileInt( "logStyle", "sre_foregroundColor", sre.foregroundColor );
	app->WriteProfileInt( "logStyle", "sre_backgroundColor", sre.backgroundColor );
	app->WriteProfileInt( "logStyle", "srk_foregroundColor", srk.foregroundColor );
	app->WriteProfileInt( "logStyle", "srk_backgroundColor", srk.backgroundColor );
	app->WriteProfileInt( "logStyle", "sd_foregroundColor", sd.foregroundColor );
	app->WriteProfileInt( "logStyle", "sd_backgroundColor", sd.backgroundColor );
	app->WriteProfileInt( "logStyle", "ses_foregroundColor", ses.foregroundColor );
	app->WriteProfileInt( "logStyle", "ses_backgroundColor", ses.backgroundColor );
	app->WriteProfileInt( "logStyle", "sen_foregroundColor", sen.foregroundColor );
	app->WriteProfileInt( "logStyle", "sen_backgroundColor", sen.backgroundColor );
	app->WriteProfileInt( "logStyle", "sem_foregroundColor", sem.foregroundColor );
	app->WriteProfileInt( "logStyle", "sem_backgroundColor", sem.backgroundColor );
	app->WriteProfileInt( "logStyle", "sef_foregroundColor", sef.foregroundColor );
	app->WriteProfileInt( "logStyle", "sef_backgroundColor", sef.backgroundColor );
	app->WriteProfileInt( "logStyle", "seu_foregroundColor", seu.foregroundColor );
	app->WriteProfileInt( "logStyle", "seu_backgroundColor", seu.backgroundColor );
}