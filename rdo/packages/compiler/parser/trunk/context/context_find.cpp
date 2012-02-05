/*!
  \copyright (c) RDO-Team, 2011
  \file      context_find.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.12.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/context_find_i.h"
#include "simulator/compiler/parser/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSE_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextFind::Result
// --------------------------------------------------------------------------------
IContextFind::Result::Result()
{}

IContextFind::Result::Result(CREF(Result) result)
	: m_pContext     (result.m_pContext     )
	, m_pExpression  (result.m_pExpression  )
	, m_pFindByValue (result.m_pFindByValue )
	, m_pValueContext(result.m_pValueContext)
{}

IContextFind::Result::Result(CREF(LPContext) pContext, CREF(LPExpression) pExpression, CREF(LPRDOValue) pFindByParam, LPContext pValueContext)
	: m_pContext     (pContext     )
	, m_pExpression  (pExpression  )
	, m_pFindByValue (pFindByParam )
	, m_pValueContext(pValueContext)
{}

// --------------------------------------------------------------------------------
// -------------------- IContextFind::Result
// --------------------------------------------------------------------------------
IContextFind::IContextFind()
{}

IContextFind::~IContextFind()
{}

CLOSE_RDO_PARSE_NAMESPACE
