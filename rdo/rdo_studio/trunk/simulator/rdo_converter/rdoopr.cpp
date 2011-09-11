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
#include "rdo_lib/rdo_converter/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_converter/pch.h"
#include "rdo_lib/rdo_converter/rdoopr.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_oprlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_oprerror(PTR(char) mes)
{}

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
	: RDOLogicActivity<rdoRuntime::RDOOperations, RDOOPROperation>(src_info)
{
	Converter::s_converter()->insertOperations(this);
}

CLOSE_RDO_CONVERTER_NAMESPACE
