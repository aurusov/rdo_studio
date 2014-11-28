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
	const rdo::runtime::LPRDORuntime runtime,
	const rdo::runtime::RDOResource::ParamList& params,
	const rdo::runtime::LPIResourceType& type,
	std::size_t resource_id,
	std::size_t type_id,
	bool trace,
	bool temporary,
	bool isNested)
{
	return rdo::Factory<rdo::runtime::RDOResource>::create(runtime, params, type, resource_id, type_id, trace, temporary, isNested);
}

rdo::runtime::LPRDOPROCResource createProcessResource(
	const rdo::runtime::LPRDORuntime runtime,
	const rdo::runtime::RDOResource::ParamList& params,
	const rdo::runtime::LPIResourceType& type,
	std::size_t resource_id,
	std::size_t type_id,
	bool trace,
	bool temporary)
{
	return rdo::Factory<rdo::runtime::RDOPROCResource>::create(runtime, params, type, resource_id, type_id, trace, temporary);
}

rdo::runtime::LPRDOPROCTransact createProcessTransact(
	const rdo::runtime::LPRDORuntime runtime,
	const rdo::runtime::RDOResource::ParamList& params,
	const rdo::runtime::LPIResourceType& type,
	std::size_t resource_id,
	std::size_t type_id,
	bool trace,
	bool temporary)
{
	return rdo::Factory<rdo::runtime::RDOPROCTransact>::create(runtime, params, type, resource_id, type_id, trace, temporary);
}

}

int rtplex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void rtperror(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
RDORTPResType::RDORTPResType(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info, bool permanent)
	: RDOParserSrcInfo(src_info)
	, RDOResourceTypeList(pParser->getRTP_id(), pParser->runtime())
	, m_number(pParser->getRTP_id()) // TODO кажется ненужным
	, m_permanent(permanent)
{
	pParser->insertRTPResType(LPRDORTPResType(this));
}

RDORTPResType::~RDORTPResType()
{}

int RDORTPResType::getNumber() const
{
	return m_number;
}

bool RDORTPResType::isPermanent() const
{
	return m_permanent;
}

bool RDORTPResType::isTemporary() const
{
	return !m_permanent;
}

const RDORTPResType::ParamList& RDORTPResType::getParams() const
{
	return m_params;
}

runtime::RDOType::TypeID RDORTPResType::typeID() const
{
	return runtime::RDOType::t_pointer;
}

LPRDORSSResource RDORTPResType::createRes(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info)
{
	return rdo::Factory<RDORSSResource>::create(pParser, src_info, this);
}

void RDORTPResType::addParam(const LPRDORTPParam& param)
{
	if (findRTPParam(param->name()))
	{
		RDOParser::s_parser()->error().error(param->src_info(), rdo::format("Параметр уже существует: %s", param->name().c_str()));
	}
	m_params.push_back(param);
}

void RDORTPResType::addParam(const std::string& /*param_name*/, rdo::runtime::RDOType::TypeID /*param_typeID*/)
{}

LPRDORTPParam RDORTPResType::findRTPParam(const std::string& paramName) const
{
	ParamList::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(paramName));
	return it != m_params.end() ? *it : LPRDORTPParam();
}

std::size_t RDORTPResType::getRTPParamNumber(const std::string& paramName) const
{
	ParamList::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(paramName));
	return it != m_params.end() ? it - m_params.begin() : UNDEFINED_PARAM;
}

void RDORTPResType::writeModelStructure(std::ostream& stream) const
{
	stream << getNumber() << " " << name() << " " << getParams().size() << std::endl;
	for (std::size_t i = 0; i < getParams().size(); i++)
	{
		stream << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure(stream);
	}
}

std::string RDORTPResType::name() const
{
	static std::string s_name;
	s_name = src_text();
	return s_name;
}

LPIType RDORTPResType::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	switch (pFrom.object_dynamic_cast<rdo::runtime::RDOType>()->typeID())
	{
	case rdo::runtime::RDOType::t_pointer:
		{
			LPIType pThisRTPType(const_cast<RDORTPResType*>(this));

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

	return LPIType(NULL);
}

LPRDOValue RDORTPResType::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	LPRDORTPResType pRTPResType = pFrom->typeInfo()->itype().object_dynamic_cast<RDORTPResType>();
	if (pRTPResType)
	{
		LPIType pThisType = const_cast<RDORTPResType*>(this);

		//! Это один и тот же тип
		if (pThisType == pRTPResType.object_dynamic_cast<IType>())
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

rdo::runtime::LPRDOCalc RDORTPResType::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	return pCalc;
}

rdo::runtime::RDOValue RDORTPResType::get_default() const
{
	NEVER_REACH_HERE;
	return rdo::runtime::RDOValue();
	//return rdo::runtime::RDOValue (pResourceType,pResource);
}

namespace
{

LPExpression contextTypeOfResourceType(const LPRDORTPResType& resourceType, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(resourceType, srcInfo),
		rdo::runtime::LPRDOCalc(NULL),
		srcInfo
	);
}

}

Context::LPFindResult RDORTPResType::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	if (method == Context::METHOD_GET || method == Context::METHOD_SET)
	{
		RDOParser::s_parser()->error().error(
			srcInfo, rdo::format("Присваивание ресурсов не разрешено"));
	}

	if (method == Context::METHOD_OPERATOR_DOT)
	{
		const std::string paramName = params.identifier();

		const std::size_t parNumb = getRTPParamNumber(paramName);
		if (parNumb == RDORTPResType::UNDEFINED_PARAM)
			return rdo::Factory<FindResult>::create();

		if (!params.exists(RDORSSResource::GET_RESOURCE))
			RDOParser::s_parser()->error().error(srcInfo, rdo::format("Ожидается имя ресурса"));

		LPRDOParam pParam = findRTPParam(paramName);
		ASSERT(pParam);

		Context::Params params_;
		params_[RDORSSResource::GET_RESOURCE] = params.get<LPExpression>(RDORSSResource::GET_RESOURCE);
		params_[RDOParam::CONTEXT_PARAM_PARAM_ID] = parNumb;
		params_[Context::Params::IDENTIFIER] = paramName;

		LPRDORTPResType pParamType =
			pParam->getTypeInfo()->itype().object_dynamic_cast<RDORTPResType>();

		if (!pParamType)
			return rdo::Factory<FindResult>::create(SwitchContext(pParam, params_));

		Context::LPFindResult result = pParam->find(Context::METHOD_GET, params_, srcInfo);
		LPExpression pNestedResource = result->getCreateExpression()();

		Context::Params params__;
		params__[RDORSSResource::GET_RESOURCE] = pNestedResource;

		return rdo::Factory<FindResult>::create(SwitchContext(pParamType, params__));
	}

	if (method == Context::METHOD_TYPE_OF)
	{
		LPRDORTPResType pThis(const_cast<RDORTPResType*>(this));
		return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextTypeOfResourceType, pThis, srcInfo)));
	}

	return rdo::Factory<FindResult>::create();
}

void RDORTPResType::setSubtype(Subtype type)
{
	ASSERT(!m_subtype.is_initialized() || m_subtype.get() == type);
	m_subtype = type;
}

void RDORTPResType::setupRuntimeFactory()
{
	Subtype subtype = m_subtype.is_initialized()
		? m_subtype.get()
		: RT_SIMPLE;

	runtime::RDOResourceTypeList::Create create;
	switch (subtype)
	{
	case RT_SIMPLE:
		create = boost::bind(&createSimpleResource, _1, _2, _3, _4, _5, _6, _7, _8);
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
	setFactoryMethod(create);
}

/*
// --------------------------------------------------------------------------------
// -------------------- RDORTPFuzzyMembershiftFun - ф-ия принадлежности нечеткого терма
// --------------------------------------------------------------------------------
RDORTPFuzzyMembershiftFun::RDORTPFuzzyMembershiftFun(const LPRDOParser& pParser):
	RDOParserObject(pParser)
{
	for (std::size_t i = 0; i < m_points.size(); i++)
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
RDORTPFuzzyTerm::RDORTPFuzzyTerm(const LPRDOParser& pParser, const RDOParserSrcInfo& src_info, RDORTPFuzzyMembershiftFun* pMembersfift_fun):
	RDOParserObject(pParser)
{

}*/

CLOSE_RDO_PARSER_NAMESPACE
