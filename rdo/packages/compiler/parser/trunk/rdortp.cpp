/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Типы ресурсов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

namespace
{

rdo::runtime::LPRDOResource createSimpleResource(
	const rdo::runtime::LPRDORuntime            runtime,
	const rdo::runtime::RDOResource::ParamList& params,
	const rdo::runtime::LPIResourceType&        type,
	ruint resource_id,
	ruint type_id,
	rbool trace,
	rbool temporary)
{
	return rdo::Factory<rdo::runtime::RDOResource>::create(runtime, params, type, resource_id, type_id, trace, temporary);
}

rdo::runtime::LPRDOPROCResource createProcessResource(
	const rdo::runtime::LPRDORuntime            runtime,
	const rdo::runtime::RDOResource::ParamList& params,
	const rdo::runtime::LPIResourceType&        type,
	ruint resource_id,
	ruint type_id,
	rbool trace,
	rbool temporary)
{
	return rdo::Factory<rdo::runtime::RDOPROCResource>::create(runtime, params, type, resource_id, type_id, trace, temporary);
}

rdo::runtime::LPRDOPROCTransact createProcessTransact(
	const rdo::runtime::LPRDORuntime            runtime,
	const rdo::runtime::RDOResource::ParamList& params,
	const rdo::runtime::LPIResourceType&        type,
	ruint resource_id,
	ruint type_id,
	rbool trace,
	rbool temporary)
{
	return rdo::Factory<rdo::runtime::RDOPROCTransact>::create(runtime, params, type, resource_id, type_id, trace, temporary);
}

}

int rtplex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void rtperror(const char* message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
RDORTPResType::RDORTPResType(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, rbool permanent, Subtype subtype)
	: RDOParserSrcInfo(src_info            )
	, m_pRuntime      (pParser->runtime()  )
	, m_number        (pParser->getRTP_id())
	, m_permanent     (permanent           )
	, m_subtype       (subtype             )
{
	pParser->insertRTPResType(LPRDORTPResType(this));
}

RDORTPResType::~RDORTPResType()
{}

runtime::RDOType::TypeID RDORTPResType::typeID() const
{
	return runtime::RDOType::t_pointer;
}

LPRDORSSResource RDORTPResType::createRes(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info)
{
	return rdo::Factory<RDORSSResource>::create(pParser, src_info, this);
}

void RDORTPResType::addParam(CREF(LPRDORTPParam) param)
{
	if (findRTPParam(param->name()))
	{
		RDOParser::s_parser()->error().error(param->src_info(), rdo::format("Параметр уже существует: %s", param->name().c_str()));
	}
	m_params.push_back(param);
}

void RDORTPResType::addParam(CREF(tstring) param_name, rdo::runtime::RDOType::TypeID param_typeID)
{
	UNUSED(param_name  );
	UNUSED(param_typeID);
}

LPRDORTPParam RDORTPResType::findRTPParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(paramName));
	return it != m_params.end() ? *it : LPRDORTPParam();
}

ruint RDORTPResType::getRTPParamNumber(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(paramName));
	return it != m_params.end() ? it - m_params.begin() : UNDEFINED_PARAM;
}

void RDORTPResType::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << getNumber() << " " << name() << " " << getParams().size() << std::endl;
	for (ruint i = 0; i < getParams().size(); i++)
	{
		stream << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure(stream);
	}
}

tstring RDORTPResType::name() const
{
	static tstring s_name;
	s_name = src_text();
	return s_name;
}

LPRDOType RDORTPResType::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(from_src_info);

	switch (pFrom->typeID())
	{
	case rdo::runtime::RDOType::t_pointer:
		{	
			LPRDOType pThisRTPType(const_cast<PTR(RDORTPResType)>(this));

			//! Это один и тот же тип
			if (pThisRTPType == pFrom)
				return pThisRTPType;

			//! Типы разные, сгенерим ошибку
			parser::g_error().push_only(src_info,    "Несоответствие типов ресурсов");
			parser::g_error().push_only(to_src_info, to_src_info.src_text());
			parser::g_error().push_done();
			break;
		}
	default:
		{
			parser::g_error().push_only(src_info,    rdo::format("Ожидается тип ресурса, найдено: %s", from_src_info.src_text().c_str()));
			parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			parser::g_error().push_done();
			break;
		}
	}

	return LPRDOType(NULL);
}

LPRDOValue RDORTPResType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDORTPResType pRTPResType = pFrom->typeInfo()->type().object_dynamic_cast<RDORTPResType>();
	if (pRTPResType)
	{
		LPRDOType pThisType = const_cast<PTR(RDORTPResType)>(this);

		//! Это один и тот же тип
		if (pThisType == pRTPResType.object_parent_cast<RDOType>())
			return pFrom;

		//! Типы разные, сгенерим ошибку
		parser::g_error().push_only(src_info,    "Несоответствие типов ресурсов");
		parser::g_error().push_only(to_src_info,  rdo::format(  "Ожидается: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_only(src_info,  rdo::format(  "Пришел: %s", pFrom->src_text().c_str()));
		parser::g_error().push_only(to_src_info, to_src_info.src_text());
		parser::g_error().push_done();
	}
	parser::g_error().push_only(src_info,    rdo::format("Ожидается ресурс, найдено: %s", pFrom->src_text().c_str()));
	parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
	parser::g_error().push_done();

	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDORTPResType::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RuntimeWrapperType::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDORTPResType::get_default() const
{
	NEVER_REACH_HERE;
	return rdo::runtime::RDOValue();
	//return rdo::runtime::RDOValue (pResourceType,pResource);
}

Context::FindResult RDORTPResType::onSwitchContext(CREF(LPExpression) pSwitchExpression, CREF(LPRDOValue) pValue) const
{
	ASSERT(pSwitchExpression);
	ASSERT(pValue           );

	if (!pSwitchExpression->calc())
	{
		RDOParser::s_parser()->error().error(
			pSwitchExpression->src_info(),
			rdo::format("Недопустимое использование типа ресурса: %s", src_text().c_str())
		);
	}

	ruint parNumb = getRTPParamNumber(pValue->value().getIdentificator());
	if (parNumb == RDORTPResType::UNDEFINED_PARAM)
	{
		RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format("Неизвестный параметр ресурса: %s", pValue->value().getIdentificator().c_str()));
	}

	LPRDORTPParam pParam = findRTPParam(pValue->value().getIdentificator());
	ASSERT(pParam);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pParam->getTypeInfo(),
		rdo::Factory<rdo::runtime::RDOCalcGetResourceParam>::create(pSwitchExpression->calc(), parNumb),
		pValue->src_info()
	);
	ASSERT(pExpression);

	return Context::FindResult(const_cast<PTR(RDORTPResType)>(this), pExpression, pValue);
}

void RDORTPResType::end()
{
	m_pRuntimeResType = rdo::Factory<rdo::runtime::RDOResourceTypeList>::create(m_number, m_pRuntime).interface_cast<rdo::runtime::IResourceType>();
	m_pType = m_pRuntimeResType;
	ASSERT(m_pType);
}

void RDORTPResType::setupRuntimeFactory()
{
	runtime::RDOResourceTypeList::Create create;
	switch (m_subtype)
	{
	case RT_SIMPLE:
		create = boost::bind(&createSimpleResource, _1, _2, _3, _4, _5, _6, _7);
		break;
	case RT_PROCESS_RESOURCE:
		create = boost::bind(&createProcessResource, _1, _2, _3, _4, _5, _6, _7);
		break;
	case RT_PROCESS_TRANSACT:
		create = boost::bind(&createProcessTransact, _1, _2, _3, _4, _5, _6, _7);
		break;
	default:
		NEVER_REACH_HERE;
	}
	rdo::runtime::LPRDOResourceTypeList type(m_pRuntimeResType);
	ASSERT(type);
	type->setFactoryMethod(create);
}

/*
// --------------------------------------------------------------------------------
// -------------------- RDORTPFuzzyMembershiftFun - ф-ия принадлежности нечеткого терма
// --------------------------------------------------------------------------------
RDORTPFuzzyMembershiftFun::RDORTPFuzzyMembershiftFun(CREF(LPRDOParser) pParser):
	RDOParserObject(pParser)
{
	for (ruint i = 0; i < m_points.size(); i++)
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
RDORTPFuzzyTerm::RDORTPFuzzyTerm(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyMembershiftFun) pMembersfift_fun):
	RDOParserObject(pParser)
{

}*/

CLOSE_RDO_PARSER_NAMESPACE
