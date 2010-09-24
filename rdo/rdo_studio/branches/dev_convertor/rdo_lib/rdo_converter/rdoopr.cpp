/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdoopr.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/pch.h"
#include "rdo_lib/rdo_converter/rdoopr.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdoparser_lexer.h"
//#include "rdo_lib/rdo_converter/rdo_logic_opr.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

int oprlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void oprerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOOPROperation
// ----------------------------------------------------------------------------
RDOOPROperation::RDOOPROperation(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{
	Converter::s_converter()->insertOPROperation(this);
}

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
RDOOperations::RDOOperations(CREF(RDOParserSrcInfo) src_info)
	: RDOLogicActivity<rdoRuntime::RDOOperations, RDOOPROperation>(src_info)
{
	//m_pRuntimeLogic = F(rdoRuntime::RDOOperations)::create(Converter::s_converter()->runtime());
	//ASSERT(m_pRuntimeLogic);
	//m_pRuntimeLogic->init(Converter::s_converter()->runtime());
	//Converter::s_converter()->runtime()->insertOperations(this);
}

CLOSE_RDO_CONVERTER_NAMESPACE
