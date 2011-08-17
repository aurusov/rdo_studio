/*
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void frmerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic)
	: RDOParserSrcInfo(src_info)
{
	m_pFrame = rdo::Factory<rdoRuntime::RDOFRMFrame>::create(src_info, pLogic ? pLogic->getCalc() : NULL);
	ASSERT(m_pFrame)
	RDOParser::s_parser()->runtime()->addRuntimeFrame(m_pFrame);
	RDOParser::s_parser()->insertFRMFrame(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

LPContext RDOFRMFrame::onFindContext(CREF(RDOValue) value) const
{
	//! Код из RDOFUNArithm::init(CREF(RDOValue) resName, CREF(RDOValue) parName)
	//! Зачем он нужен - непонятно
	//! Его задача - сгенерить исключение в рантайме, почему это не сделать в парсере ?

	//if (RDOParser::s_parser()->getLastFRMFrame() && RDOParser::s_parser()->getLastFRMFrame()->frame()->getLastShow() && RDOParser::s_parser()->getLastFRMFrame()->frame()->getLastShow()->isShowIf())
	//{
	//	m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetUnknowResParam>::create(resName->getIdentificator(), parName->getIdentificator());
	//	m_pCalc->setSrcInfo(src_info());
	//	return;
	//}

	return NULL;
}

void RDOFRMFrame::end()
{
	RDOParser::s_parser()->contextStack()->pop();
}

CLOSE_RDO_PARSER_NAMESPACE
