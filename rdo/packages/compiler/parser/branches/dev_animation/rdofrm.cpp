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
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdofrm.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void frmerror(PTR(char) message)
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
	ASSERT(m_pFrame)
	RDOParser::s_parser()->runtime()->addRuntimeFrame(m_pFrame);
	RDOParser::s_parser()->insertFRMFrame(this);
	RDOParser::s_parser()->contextStack()->push(this);

	m_pContextMemory = rdo::Factory<ContextMemory>::create();
	ASSERT(m_pContextMemory);
	RDOParser::s_parser()->contextStack()->push(m_pContextMemory);

	ContextMemory::push();
}

Context::FindResult RDOFRMFrame::onFindContext(CREF(LPRDOValue) pValue) const
{
	UNUSED(pValue);

	//! Код из RDOFUNArithm::init(CREF(RDOValue) resName, CREF(RDOValue) parName)
	//! Зачем он нужен - непонятно
	//! Его задача - сгенерить исключение в рантайме, почему это не сделать в парсере ?

	//if (RDOParser::s_parser()->getLastFRMFrame() && RDOParser::s_parser()->getLastFRMFrame()->frame()->getLastShow() && RDOParser::s_parser()->getLastFRMFrame()->frame()->getLastShow()->isShowIf())
	//{
	//	m_pCalc = rdo::Factory<rdo::runtime::RDOCalcGetUnknowResParam>::create(resName->getIdentificator(), parName->getIdentificator());
	//	m_pCalc->setSrcInfo(src_info());
	//	return;
	//}

	return Context::FindResult();
}

void RDOFRMFrame::end()
{
	RDOParser::s_parser()->contextStack()->pop();
	ContextMemory::pop();
}

CLOSE_RDO_PARSER_NAMESPACE
