/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp.cpp
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
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_rtplex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_rtperror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
RDORTPResType::RDORTPResType(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, rbool permanent)
	: RDOParserSrcInfo(src_info            )
	, m_number        (pParser->getRTP_id())
	, m_permanent     (permanent           )
{
	pParser->insertRTPResType(LPRDORTPResType(this));
}

RDORTPResType::~RDORTPResType()
{}

void RDORTPResType::addParam(CREF(LPRDORTPParam) param)
{
	if (findRTPParam(param->name()))
	{
		Converter::s_converter()->error().error(param->src_info(), rdo::format("Параметр уже существует: %s", param->name().c_str()));
	}
	m_params.push_back(param);
}

void RDORTPResType::addParam(CREF(tstring) param_name, rdo::runtime::RDOType::TypeID param_typeID)
{
	UNUSED(param_name  );
	UNUSED(param_typeID);
	NEVER_REACH_HERE;
}

LPRDORTPParam RDORTPResType::findRTPParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(paramName));
	return it != m_params.end() ? *it : LPRDORTPParam();
}

void RDORTPResType::finish()
{}

ruint RDORTPResType::getRTPParamNumber(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(paramName));
	return it != m_params.end() ? it - m_params.begin() : UNDEFINED_PARAM;
}

void RDORTPResType::writeModelStructure(REF(std::ostream) stream) const
{
	stream << getNumber() << " " << name() << " " << getParams().size() << std::endl;
	for (ruint i = 0; i < getParams().size(); i++)
	{
		stream << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure(stream);
	}
}

/*
// --------------------------------------------------------------------------------
// -------------------- RDORTPFuzzyMembershiftFun - ф-ия принадлежности нечеткого терма
// --------------------------------------------------------------------------------
RDORTPFuzzyMembershiftFun::RDORTPFuzzyMembershiftFun(PTR(Converter) pParser):
	RDOParserObject(pParser)
{
/*	for (ruint i = 0; i < m_points.size(); i++)
	{
//		double x = m_points[i]->getX();
	}

	Items::iterator it = m_points.begin();
	while (it != m_points.end())
	{
		double x = (*it)->getX();
		it++;
	}
}
// --------------------------------------------------------------------------------
// -------------------- RDORTPFuzzyTerm - нечеткий термин
// --------------------------------------------------------------------------------
RDORTPFuzzyTerm::RDORTPFuzzyTerm(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyMembershiftFun) pMembersfift_fun):
	RDOParserObject(pParser)
{

}*/

CLOSE_RDO_CONVERTER_NAMESPACE
