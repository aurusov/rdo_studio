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
#include <boost/bind.hpp>
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
// -------------------- RDOFRMCommandList
// --------------------------------------------------------------------------------
RDOFRMCommandList::RDOFRMCommandList()
{
	RDOParser::s_parser()->insertFRMCommandList(this);
}

LPExpression RDOFRMCommandList::generateExpression(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(RDOParserSrcInfo) srcInfo)
{
	ASSERT(pCalc);

	LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(srcInfo);
	ASSERT(pType);

	LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, srcInfo);
	ASSERT(pExpression);

	return pExpression;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMSprite
// --------------------------------------------------------------------------------
RDOFRMSprite::RDOFRMSprite(CREF(RDOParserSrcInfo) src_info)
	: RDOFRMCommandList()
	, Function(rdo::Factory<TypeInfo>::delegate<RDOType__void>(src_info), src_info)
{
	m_pSprite = rdo::Factory<rdo::runtime::RDOFRMSprite>::create(this->src_info());
	ASSERT(m_pSprite)
	RDOParser::s_parser()->insertFRMSprite(this);

	setCall(m_pSprite);

	pushContext();
}

void RDOFRMSprite::end()
{
	popContext();
}

Context::FindResult RDOFRMSprite::onFindContext(CREF(LPRDOValue) pValue) const
{
	return Function::onFindContext(pValue);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOParserSrcInfo) srcInfo)
	: RDOFRMCommandList()
{
	m_pFrame = rdo::Factory<rdo::runtime::RDOFRMFrame>::create(srcInfo);
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
	ASSERT(pValue);

	//! Код из RDOFUNArithm::init(CREF(RDOValue) resName, CREF(RDOValue) parName)
	//! Зачем он нужен - непонятно
	//! Его задача - сгенерить исключение в рантайме, почему это не сделать в парсере ?

	//if (RDOParser::s_parser()->getLastFRMFrame() && RDOParser::s_parser()->getLastFRMFrame()->frame()->getLastShow() && RDOParser::s_parser()->getLastFRMFrame()->frame()->getLastShow()->isShowIf())
	//{
	//	m_pCalc = rdo::Factory<rdo::runtime::RDOCalcGetUnknowResParam>::create(resName->getIdentificator(), parName->getIdentificator());
	//	m_pCalc->setSrcInfo(src_info());
	//	return;
	//}

	tstring name = pValue->value().getIdentificator();
	LPRDOFRMSprite pSprite = RDOParser::s_parser()->findFRMSprite(name);
	if (pSprite)
	{
		return Context::FindResult(const_cast<PTR(RDOFRMFrame)>(this), pSprite->expression(), pValue);
	}

	return Context::FindResult();
}

void RDOFRMFrame::end()
{
	ContextMemory::pop();
	RDOParser::s_parser()->contextStack()->pop();
	RDOParser::s_parser()->contextStack()->pop();
}

CLOSE_RDO_PARSER_NAMESPACE
