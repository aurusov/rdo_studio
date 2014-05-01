/*!
  \copyright (c) RDO-Team, 2012
  \file      function_type.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.12.2012
  \brief     Тип функции
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/function_type.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- FunctionParamType
// --------------------------------------------------------------------------------
FunctionParamType::FunctionParamType(const ParamList& paramList, const RDOParserSrcInfo& srcInfo)
	: RDOType(t_unknow)
	, RDOParserSrcInfo(srcInfo)
	, m_paramList(paramList)
{
	if (m_paramList.empty())
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(srcInfo);
		ASSERT(pType);
		m_paramList.push_back(pType);
	}
}

FunctionParamType::~FunctionParamType()
{}

const FunctionParamType::ParamList& FunctionParamType::paramList() const
{
	return m_paramList;
}

const rdo::runtime::LPRDOType& FunctionParamType::type() const
{
	NEVER_REACH_HERE;
	static rdo::runtime::LPRDOType s_Type;
	return s_Type;
}

rdo::runtime::RDOType::TypeID FunctionParamType::typeID() const
{
	NEVER_REACH_HERE;
	return rdo::runtime::RDOType::t_unknow;
}

std::string FunctionParamType::name() const
{
	return src_text();
}

LPIType FunctionParamType::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	LPFunctionParamType pFromParamType = pFrom.object_dynamic_cast<FunctionParamType>();
	if (!pFromParamType)
	{
		parser::g_error().push_only(src_info,    rdo::format("Ожидаеются параметры вызова, найдено: %s", from_src_info.src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}

	if (paramList().size() != pFromParamType->paramList().size())
	{
		RDOParser::s_parser()->error().error(
			src_info,
			rdo::format("Неверное количество параметров: %s", from_src_info.src_text().c_str())
		);
	}

	ParamList::const_iterator paramFromIt = pFromParamType->paramList().begin();
	for (const LPTypeInfo& pParamType: paramList())
	{
		pParamType->type_cast(*paramFromIt, src_info);
		++paramFromIt;
	}

	return const_cast<FunctionParamType*>(this);
}

LPRDOValue FunctionParamType::value_cast(const LPRDOValue& /*pFrom*/, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& /*src_info*/) const
{
	NEVER_REACH_HERE;
	return LPRDOValue();
}

rdo::runtime::LPRDOCalc FunctionParamType::calc_cast(const rdo::runtime::LPRDOCalc& /*pCalc*/, const LPIType& /*pType*/) const
{
	NEVER_REACH_HERE;
	return rdo::runtime::LPRDOCalc();
}

rdo::runtime::RDOValue FunctionParamType::get_default() const
{
	NEVER_REACH_HERE;
	return rdo::runtime::RDOValue();
}

void FunctionParamType::writeModelStructure(std::ostream& stream) const
{
	stream << "F " << std::endl;
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- FunctionType
// --------------------------------------------------------------------------------
FunctionType::FunctionType(const LPTypeInfo& pReturnType, const LPFunctionParamType& pParamType, const RDOParserSrcInfo& srcInfo)
	: RDOType(pReturnType->typeID())
	, RDOParserSrcInfo(srcInfo    )
	, m_pReturnType   (pReturnType)
	, m_pParamType    (pParamType )
{
	ASSERT(m_pReturnType);
	ASSERT(m_pParamType );
}

FunctionType::~FunctionType()
{}

const rdo::runtime::LPRDOType& FunctionType::type() const
{
	return m_pReturnType->type();
}

rdo::runtime::RDOType::TypeID FunctionType::typeID() const
{
	return m_pReturnType->typeID();
}

const LPTypeInfo& FunctionType::returnType() const
{
	return m_pReturnType;
}

const LPFunctionParamType& FunctionType::paramType() const
{
	return m_pParamType;
}

std::string FunctionType::name() const
{
	return src_text();
}

LPIType FunctionType::type_cast(const LPIType& /*pFrom*/, const RDOParserSrcInfo& /*from_src_info*/, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& /*src_info*/) const
{
	NEVER_REACH_HERE;
	return LPIType();
}

LPRDOValue FunctionType::value_cast(const LPRDOValue& /*pFrom*/, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& /*src_info*/) const
{
	NEVER_REACH_HERE;
	return LPRDOValue();
}

rdo::runtime::LPRDOCalc FunctionType::calc_cast(const rdo::runtime::LPRDOCalc& /*pCalc*/, const LPIType& /*pType*/) const
{
	NEVER_REACH_HERE;
	return rdo::runtime::LPRDOCalc();
}

rdo::runtime::RDOValue FunctionType::get_default() const
{
	NEVER_REACH_HERE;
	return rdo::runtime::RDOValue();
}

void FunctionType::writeModelStructure(std::ostream& stream) const
{
	stream << "F " << std::endl;
	NEVER_REACH_HERE;
}

CLOSE_RDO_PARSER_NAMESPACE
