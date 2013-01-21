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
{}

RuntimeWrapperType::RuntimeWrapperType(CREF(rdo::runtime::LPRDOType) pType)
	: m_pType(pType)
{
	ASSERT(m_pType);
	if (typeID() == rdo::runtime::RDOType::t_unknow)
	{
		int i = 1;
		UNUSED(i);
	}
}

RuntimeWrapperType::~RuntimeWrapperType()
{}

CREF(rdo::runtime::LPRDOType) RuntimeWrapperType::type() const
{
	return m_pType;
}

rdo::runtime::RDOType::TypeID RuntimeWrapperType::typeID() const
{
	return m_pType->typeID();
}

rdo::runtime::LPRDOCalc RuntimeWrapperType::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	UNUSED(pType);
	return pCalc;
}

void RuntimeWrapperType::serializeInDB(REF(IDB) db) const
{
	NEVER_REACH_HERE;
}

CLOSE_RDO_PARSER_NAMESPACE
