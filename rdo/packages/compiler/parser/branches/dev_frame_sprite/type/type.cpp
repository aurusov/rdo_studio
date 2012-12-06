/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/type/type.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.12.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOType::RDOType()
{}

RDOType::RDOType(CREF(rdo::runtime::LPRDOType) pType)
	: m_pType(pType)
{
	ASSERT(m_pType);
	if (typeID() == rdo::runtime::RDOType::t_unknow)
	{
		int i = 1;
		UNUSED(i);
	}
}

RDOType::~RDOType()
{}

CREF(rdo::runtime::LPRDOType) RDOType::type() const
{
	return m_pType;
}

rdo::runtime::RDOType::TypeID RDOType::typeID() const
{
	return m_pType->typeID();
}

rdo::runtime::LPRDOCalc RDOType::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	UNUSED(pType);
	return pCalc;
}

CLOSE_RDO_PARSER_NAMESPACE
