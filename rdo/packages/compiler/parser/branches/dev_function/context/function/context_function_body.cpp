/*!
  \copyright (c) RDO-Team, 2012
  \file      context_function_body.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      08.12.2012
  \brief     Контекст тела функции
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/function/context_function_body.h"
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextFunctionBody
// --------------------------------------------------------------------------------
ContextFunctionBody::ContextFunctionBody(const OnBody& onBody)
	: m_onBody(onBody)
{}

ContextFunctionBody::~ContextFunctionBody()
{}

void ContextFunctionBody::pushContext()
{
	LPContextMemory pContextMemory = rdo::Factory<ContextMemory>::create();
	ASSERT(pContextMemory);
	RDOParser::s_parser()->contextStack()->push(pContextMemory);

	ContextMemory::push();
}

void ContextFunctionBody::popContext()
{
	ContextReturnable::resetChildContext();
	ContextMemory::pop();
	RDOParser::s_parser()->contextStack()->pop<ContextMemory>();
}

void ContextFunctionBody::setBody(CREF(rdo::runtime::LPRDOCalc) pBody)
{
	m_onBody(pBody);
}

CLOSE_RDO_PARSER_NAMESPACE
