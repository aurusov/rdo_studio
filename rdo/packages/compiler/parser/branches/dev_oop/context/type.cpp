/*!
  \copyright (c) RDO-Team, 2011
  \file      type.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.10.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/type.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TypeContext
// --------------------------------------------------------------------------------
TypeContext::TypeContext(CREF(LPTypeInfo) pType)
	: m_pType(pType)
{}

CREF(LPTypeInfo) TypeContext::getTypeInfo() const
{
	return m_pType;
}

CLOSE_RDO_PARSER_NAMESPACE
