/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoopr.h
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
#include "converter/smr2rdox/pch.h"
#include "converter/smr2rdox/rdoopr.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_oprlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_oprerror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOOPROperation
// --------------------------------------------------------------------------------
RDOOPROperation::RDOOPROperation(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{
	Converter::s_converter()->insertOPROperation(this);
}

// --------------------------------------------------------------------------------
// -------------------- RDOOperations
// --------------------------------------------------------------------------------
RDOOperations::RDOOperations(CREF(RDOParserSrcInfo) src_info)
	: RDOLogicActivity<rdo::runtime::RDOOperations, RDOOPROperation>(src_info)
{
	Converter::s_converter()->insertOperations(this);
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
