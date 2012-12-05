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
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- FunctionParamType
// --------------------------------------------------------------------------------
FunctionParamType::FunctionParamType(CREF(ParamList) paramList, CREF(RDOParserSrcInfo) srcInfo)
	: RDOParserSrcInfo(srcInfo  )
	, m_paramList     (paramList)
{}

FunctionParamType::~FunctionParamType()
{}

CREF(FunctionParamType::ParamList) FunctionParamType::paramList() const
{
	return m_paramList;
}

tstring FunctionParamType::name() const
{
	return src_text();
}

LPRDOType FunctionParamType::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	NEVER_REACH_HERE;
	return LPRDOType();
}

LPRDOValue FunctionParamType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	NEVER_REACH_HERE;
	return LPRDOValue();
}

rdo::runtime::LPRDOCalc FunctionParamType::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	NEVER_REACH_HERE;
	return rdo::runtime::LPRDOCalc();
}

rdo::runtime::RDOValue FunctionParamType::get_default() const
{
	NEVER_REACH_HERE;
	return rdo::runtime::RDOValue();
}

void FunctionParamType::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << "F " << std::endl;
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- FunctionType
// --------------------------------------------------------------------------------
FunctionType::FunctionType(CREF(LPTypeInfo) pReturnType, CREF(LPFunctionParamType) pParamType, CREF(RDOParserSrcInfo) srcInfo)
	: RDOParserSrcInfo(srcInfo    )
	, m_pReturnType   (pReturnType)
	, m_pParamType    (pParamType )
{
	ASSERT(m_pReturnType);
	ASSERT(m_pParamType );
}

FunctionType::~FunctionType()
{}

CREF(LPTypeInfo) FunctionType::returnType() const
{
	return m_pReturnType;
}

CREF(LPFunctionParamType) FunctionType::paramType() const
{
	return m_pParamType;
}

tstring FunctionType::name() const
{
	return src_text();
}

LPRDOType FunctionType::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	NEVER_REACH_HERE;
	return LPRDOType();
}

LPRDOValue FunctionType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	NEVER_REACH_HERE;
	return LPRDOValue();
}

rdo::runtime::LPRDOCalc FunctionType::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	NEVER_REACH_HERE;
	return rdo::runtime::LPRDOCalc();
}

rdo::runtime::RDOValue FunctionType::get_default() const
{
	NEVER_REACH_HERE;
	return rdo::runtime::RDOValue();
}

void FunctionType::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << "F " << std::endl;
	NEVER_REACH_HERE;
}

CLOSE_RDO_PARSER_NAMESPACE
