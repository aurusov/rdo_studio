/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorss.cpp
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
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/calc/resource/calc_create_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int rsslex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void rsserror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
RDORSSResource::RDORSSResource(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, ruint id)
	: RDOParserSrcInfo(src_info                                      )
	, m_pResType      (pResType                                      )
	, m_id            (id == UNDEFINED_ID ? pParser->getRSS_id() : id)
	, trace           (false                                         )
{
	ASSERT(m_pResType);
	pParser->insertRSSResource(LPRDORSSResource(this));
	m_currParam = m_pResType->getParams().begin();
	RDOParser::s_parser()->contextStack()->push(this);
}

RDORSSResource::~RDORSSResource()
{}

void RDORSSResource::end()
{
	RDOParser::s_parser()->contextStack()->pop();
}

Context::FindResult RDORSSResource::onSwitchContext(CREF(LPExpression) pSwitchExpression, CREF(LPRDOValue) pValue) const
{
	ASSERT(pSwitchExpression);
	ASSERT(pValue           );

	ruint parNumb = getType()->getRTPParamNumber(pValue->value().getIdentificator());
	if (parNumb == RDORTPResType::UNDEFINED_PARAM)
	{
		RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("Неизвестный параметр ресурса: %s"), pValue->value().getIdentificator().c_str()));
	}

	LPRDORTPParam pParam = getType()->findRTPParam(pValue->value().getIdentificator());
	ASSERT(pParam);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pParam->getTypeInfo(),
		rdo::Factory<rdoRuntime::RDOCalcGetResourceParam>::create(pSwitchExpression->calc(), parNumb),
		pValue->src_info()
	);
	ASSERT(pExpression);

	return Context::FindResult(const_cast<PTR(RDORSSResource)>(this), pExpression, pValue);
}

void RDORSSResource::writeModelStructure(REF(std::ostream) stream) const
{
	stream << (getID() + 1) << _T(" ") << name() << _T(" ") << getType()->getNumber() << std::endl;
}

void RDORSSResource::addParam(CREF(LPRDOValue) pParam)
{
	ASSERT(pParam);

	if (m_currParam == getType()->getParams().end())
	{
		RDOParser::s_parser()->error().push_only(pParam->src_info(), _T("Слишком много параметров"));
		RDOParser::s_parser()->error().push_only(getType()->src_info(), _T("См. тип ресурса"));
		RDOParser::s_parser()->error().push_done();
	}
	try
	{
		if (pParam->value().getAsString() == _T("*"))
		{
			if (!(*m_currParam)->getDefault()->defined())
			{
				RDOParser::s_parser()->error().push_only(pParam->src_info(), _T("Невозможно использовать '*', к.т. отсутствует значение по-умолчанию"));
				/// @todo src_info() без параметра RDOParserSrcInfo()
				RDOParser::s_parser()->error().push_only((*m_currParam)->getTypeInfo()->src_info(RDOParserSrcInfo()), _T("См. описание параметра"));
				RDOParser::s_parser()->error().push_done();
			}
			m_paramList.push_back(Param((*m_currParam)->getDefault()));
			m_currParam++;
		}
		else if (pParam->value().getAsString() == _T("#"))
		{
			if (!(*m_currParam)->getDefault()->defined())
			{
				RDOParser::s_parser()->error().push_only(pParam->src_info(), _T("Невозможно использовать '#', к.т. отсутствует значение по-умолчанию"));
				/// @todo src_info() без параметра RDOParserSrcInfo()
				RDOParser::s_parser()->error().push_only((*m_currParam)->getTypeInfo()->src_info(RDOParserSrcInfo()), _T("См. описание параметра"));
				RDOParser::s_parser()->error().push_done();
			}
			pParam->value().setUndefined(0);
			m_paramList.push_back(Param((*m_currParam)->getTypeInfo()->value_cast(pParam)));
			m_currParam++;
		}
		else
		{
			m_paramList.push_back(Param((*m_currParam)->getTypeInfo()->value_cast(pParam)));
			m_currParam++;
		}
	}
	catch(REF(RDOSyntaxException))
	{
		RDOParser::s_parser()->error().modify(rdo::format(_T("Для параметра '%s': "), (*m_currParam)->name().c_str()));
	}
}

rbool RDORSSResource::defined() const
{
	return m_currParam == getType()->getParams().end();
}

rdoRuntime::LPRDOCalc RDORSSResource::createCalc() const
{
	std::vector<rdoRuntime::RDOValue> paramList;
	STL_FOR_ALL_CONST(params(), it)
	{
		paramList.push_back(it->param()->value());
	}

	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcCreateResource>::create(
		getType()->getRuntimeResType(),
		paramList,
		getTrace(),
		getType()->isPermanent()
	);
	ASSERT(pCalc);
	rdoRuntime::RDOSrcInfo srcInfo(src_info());
	srcInfo.setSrcText(_T("Создание ресурса ") + src_text());
	pCalc->setSrcInfo(srcInfo);
	return pCalc;
}

CLOSE_RDO_PARSER_NAMESPACE
