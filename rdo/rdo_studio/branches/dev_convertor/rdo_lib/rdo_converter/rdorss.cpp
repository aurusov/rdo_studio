/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdorss.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdorss.h"
#include "rdo_lib/rdo_converter/rdortp.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdoparser_lexer.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

int rsslex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void rsserror(PTR(char) message)
{}

// ----------------------------------------------------------------------------
// ---------- RDORSSResource
// ----------------------------------------------------------------------------
RDORSSResource::RDORSSResource(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id)
	: RDOParserSrcInfo(src_info                                      )
	, m_pResType      (pResType                                      )
	, m_id            (id == UNDEFINED_ID ? pParser->getRSS_id() : id)
	, trace           (false                                         )
{
	ASSERT(m_pResType);
	pParser->insertRSSResource(LPRDORSSResource(this));
	m_currParam = m_pResType->getParams().begin();
}

void RDORSSResource::writeModelStructure(REF(std::ostream) stream) const
{
	stream << (getID() + 1) << _T(" ") << name() << _T(" ") << getType()->getNumber() << std::endl;
}

void RDORSSResource::addParam(CREF(RDOValue) param)
{
	if (m_currParam == getType()->getParams().end())
	{
		RDOParser::s_parser()->error().push_only(param.src_info(), _T("Слишком много параметров"));
		RDOParser::s_parser()->error().push_only(getType()->src_info(), _T("См. тип ресурса"));
		RDOParser::s_parser()->error().push_done();
	}
	try
	{
		if (param->getAsString() == _T("*"))
		{
			if (!(*m_currParam)->getDefault().defined())
			{
				RDOParser::s_parser()->error().push_only(param.src_info(), _T("Невозможно использовать '*', к.т. отсутствует значение по-умолчанию"));
				RDOParser::s_parser()->error().push_only((*m_currParam)->getParamType()->src_info(), _T("См. описание параметра"));
				RDOParser::s_parser()->error().push_done();
			}
			m_paramList.push_back(Param((*m_currParam)->getDefault()));
			m_currParam++;
		}
		else
		{
			m_paramList.push_back(Param((*m_currParam)->getParamType()->value_cast(param)));
			m_currParam++;
		}
	}
	catch(REF(RDOSyntaxException))
	{
		RDOParser::s_parser()->error().modify(rdo::format(_T("Для параметра '%s': "), (*m_currParam)->name().c_str()));
	}
}

bool RDORSSResource::defined() const
{
	return m_currParam == getType()->getParams().end();
}

rdoRuntime::LPRDOCalc RDORSSResource::createCalc() const
{
	std::vector<rdoRuntime::RDOValue> paramList;
	STL_FOR_ALL_CONST(ParamList, params(), it)
	{
		paramList.push_back(it->param().value());
	}

	rdoRuntime::LPRDOCalc calc = rdo::Factory<rdoRuntime::RDOCalcCreateNumberedResource>::create(getType()->getNumber(), getTrace(), paramList, getID(), getType()->isPermanent());
	calc->setSrcInfo(src_info());
	calc->setSrcText(_T("Создание ресурса ") + calc->src_text());
	return calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource(PTR(RDOParser) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id)
	: RDORSSResource(pParser, src_info, pResType, id)
{}

rdoRuntime::LPRDOCalc RDOPROCResource::createCalc() const
{
	std::vector<rdoRuntime::RDOValue> paramList;
	STL_FOR_ALL_CONST(ParamList, params(), it)
	{
		paramList.push_back(it->param().value());
	}

	rdoRuntime::LPRDOCalc calc = rdo::Factory<rdoRuntime::RDOCalcCreateProcessResource>::create(getType()->getNumber(), getTrace(), paramList, getID(), getType()->isPermanent());
	calc->setSrcInfo(src_info());
	calc->setSrcText(_T("Создание ресурса ") + calc->src_text());
	return calc;
}

CLOSE_RDO_CONVERTER_NAMESPACE
