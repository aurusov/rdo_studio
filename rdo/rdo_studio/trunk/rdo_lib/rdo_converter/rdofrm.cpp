/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdofrm.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdofrm.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdoparser_lexer.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_frmerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic)
	: RDOParserSrcInfo(src_info)
{
	m_pFrame = rdo::Factory<rdoRuntime::RDOFRMFrame>::create(src_info, pLogic ? pLogic->getCalc() : NULL);
	Converter::s_converter()->insertFRMFrame(this);
}

CLOSE_RDO_CONVERTER_NAMESPACE
