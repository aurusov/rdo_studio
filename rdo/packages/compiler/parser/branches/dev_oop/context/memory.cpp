/*!
  \copyright (c) RDO-Team, 2011
  \file      memory.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/memory.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/runtime/calc/procedural/calc_locvar.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextMemory
// --------------------------------------------------------------------------------
ContextMemory::ContextMemory()
{
	m_pLocalVariableListStack = rdo::Factory<LocalVariableListStack>::create();
	ASSERT(m_pLocalVariableListStack);
}

LPLocalVariableListStack ContextMemory::getLocalMemory()
{
	return m_pLocalVariableListStack;
}

Context::FindResult ContextMemory::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	LPLocalVariable pLocalVariable = m_pLocalVariableListStack->findLocalVariable(pValue->value().getIdentificator());
	if (pLocalVariable)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pLocalVariable->getExpression()->typeInfo(),
			rdo::Factory<rdoRuntime::RDOCalcGetLocalVariable>::create(pLocalVariable->getName()),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(ContextMemory)>(this), pExpression, pValue);
	}

	return Context::FindResult();
}

void ContextMemory::push()
{
	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);
	LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
	ASSERT(pContextMemory);

	LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
	ASSERT(pLocalVariableListStack);

	LPLocalVariableList pLocalVariableList = rdo::Factory<LocalVariableList>::create();
	ASSERT(pLocalVariableList);
	pLocalVariableListStack->push(pLocalVariableList);
}

void ContextMemory::pop()
{
	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);
	LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
	ASSERT(pContextMemory);

	LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
	ASSERT(pLocalVariableListStack);

	pLocalVariableListStack->pop();
}

// --------------------------------------------------------------------------------
// -------------------- ContextStatement
// --------------------------------------------------------------------------------
ContextStatement::ContextStatement(CREF(rdoRuntime::LPRDOCalc) pCalc)
	: m_pStatement(pCalc)
{}

rbool ContextStatement::is_for_statement()
{
	return ((m_pStatement.object_dynamic_cast<rdoRuntime::RDOCalcFor>()) ? true : false);
}

Context::FindResult ContextStatement::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);
	//Оператор for
	if(const_cast<PTR(ContextStatement)>(this)->is_for_statement())
	{
		tstring statement = pValue->value().getIdentificator();
		
		if(statement == "break")
		{
			rdoRuntime::LPRDOCalc pCalcBreak = rdo::Factory<rdoRuntime::RDOCalcFunBreak>::create();
			ASSERT(pCalcBreak);

			LPRDOType pBaseType = rdo::Factory<RDOType__void>::create();
			ASSERT(pBaseType);

			LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pBaseType, pValue->src_info());
			ASSERT(pType);

			LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalcBreak, pValue->src_info());
			ASSERT(pExpression);

			return Context::FindResult(const_cast<PTR(ContextStatement)>(this), pExpression, pValue);
		}
		else
		{
			rdoParser::g_error().error(pValue->src_info(), _T("Данный оператор используется неверно"));
		}

		rdoParser::g_error().error(pValue->src_info(), _T("Данный оператор нельзя использовать без объемлющего оператора"));
	}
	return Context::FindResult();
}

CLOSE_RDO_PARSER_NAMESPACE