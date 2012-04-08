/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdofrm.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_frmerror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic)
	: RDOParserSrcInfo(src_info)
{
	m_pFrame = rdo::Factory<rdo::runtime::RDOFRMFrame>::create(src_info, pLogic ? pLogic->getCalc() : rdo::runtime::LPRDOCalc(NULL));
	Converter::s_converter()->insertFRMFrame(this);
}

CLOSE_RDO_CONVERTER_NAMESPACE
