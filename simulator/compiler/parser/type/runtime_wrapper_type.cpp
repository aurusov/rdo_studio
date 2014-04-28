/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/type/runtime_wrapper_type.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.12.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/runtime_wrapper_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RuntimeWrapperType::RuntimeWrapperType()
	: RDOType(t_pointer)
{}

RuntimeWrapperType::RuntimeWrapperType(const rdo::runtime::LPRDOType& pType)
	: RDOType(pType->typeID())
	, m_pType(pType)
{
	ASSERT(m_pType);
}

RuntimeWrapperType::~RuntimeWrapperType()
{}

const rdo::runtime::LPRDOType& RuntimeWrapperType::type() const
{
	return m_pType;
}

rdo::runtime::RDOType::TypeID RuntimeWrapperType::typeID() const
{
	return m_pType->typeID();
}

rdo::runtime::LPRDOCalc RuntimeWrapperType::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPRDOType& /*pType*/) const
{
	return pCalc;
}

CLOSE_RDO_PARSER_NAMESPACE
