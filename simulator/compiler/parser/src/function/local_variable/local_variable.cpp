/*!
  \copyright (c) RDO-Team, 2011
  \file      local_variable.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/function/local_variable/local_variable.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/runtime/calc/procedural/calc_locvar.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- LocalVariable
// --------------------------------------------------------------------------------
LocalVariable::LocalVariable(const LPRDOValue& pName, const LPExpression& pExpression)
	: m_pName      (pName      )
	, m_pExpression(pExpression)
{
	ASSERT(m_pName      );
	ASSERT(m_pExpression);

	if (m_pExpression->typeInfo()->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().error(m_pExpression->typeInfo()->src_info(), "У данного типа нет значения поумолчанию");
	}
}

LocalVariable::~LocalVariable()
{}

const std::string& LocalVariable::getName() const
{
	return m_pName->value().getIdentificator();
}

const RDOParserSrcInfo& LocalVariable::getSrcInfo() const
{
	return *m_pName;
}

const LPExpression& LocalVariable::getExpression() const
{
	return m_pExpression;
}

const LPTypeInfo& LocalVariable::getTypeInfo() const
{
	return m_pExpression->typeInfo();
}

rdo::runtime::RDOValue LocalVariable::getDefaultValue() const
{
	return m_pExpression->typeInfo()->itype()->get_default();
}

namespace
{

LPExpression contextGetLocalVariable(const LPLocalVariable& pLocalVariable, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		pLocalVariable->getExpression()->typeInfo(),
		rdo::Factory<rdo::runtime::RDOCalcGetLocalVariable>::create(pLocalVariable->getName()),
		srcInfo
	);
}

LPExpression contextSetLocalVariable(const LPLocalVariable& pLocalVariable, const rdo::runtime::LPRDOCalc& rightValue, const RDOParserSrcInfo& srcInfo)
{
	return rdo::Factory<Expression>::create(
		pLocalVariable->getTypeInfo(),
		rdo::Factory<rdo::runtime::RDOCalcSetLocalVariable>::create(pLocalVariable->getName(), rightValue),
		srcInfo
	);
}

}

Context::LPFindResult LocalVariable::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	if (method == Context::METHOD_GET)
	{
		LPLocalVariable pThis(const_cast<LocalVariable*>(this));
		return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextGetLocalVariable, pThis, srcInfo)));
	}

	if (method == Context::METHOD_SET)
	{
		LPLocalVariable pThis(const_cast<LocalVariable*>(this));

		using namespace rdo::runtime;

		const LPRDOCalc localVariableValue = rdo::Factory<FindResult>::create(CreateExpression
			(boost::bind(&contextGetLocalVariable, pThis, srcInfo))
			)->getCreateExpression()()->calc();

		const LPRDOCalc rightValue = params.exists(Expression::CONTEXT_PARAM_SET_EXPRESSION)
			? params.get<LPExpression>(Expression::CONTEXT_PARAM_SET_EXPRESSION)->calc()
			: params.get<LPRDOFUNArithm>(RDOFUNArithm::CONTEXT_PARAM_SET_ARITHM)->createCalc(getTypeInfo());

		LPRDOCalc operationResult;

		switch (params.get<SetOperationType::Type>(Expression::CONTEXT_PARAM_SET_OPERATION_TYPE))
		{
		case SetOperationType::NOCHANGE:
			RDOParser::s_parser()->error().error(srcInfo, "Недопустимый тип операции: NOCHANGE");
			break;
		case SetOperationType::SET        :
			operationResult = rightValue;
			break;
		case SetOperationType::ADDITION   :
			operationResult =  rdo::Factory<RDOCalcPlus>::create(localVariableValue, rightValue);
			break;
		case SetOperationType::SUBTRACTION:
			operationResult =  rdo::Factory<RDOCalcMinus>::create(localVariableValue, rightValue);
			break;
		case SetOperationType::MULTIPLY   :
			operationResult =  rdo::Factory<RDOCalcMult>::create(localVariableValue, rightValue);
			break;
		case SetOperationType::DIVIDE     :
			operationResult =  rdo::Factory<RDOCalcDiv>::create(localVariableValue, rightValue);
			break;
		case SetOperationType::INCREMENT  :
			operationResult =  rdo::Factory<RDOCalcPlus>::create(localVariableValue, rdo::Factory<RDOCalcConst>::create(1));
			break;
		case SetOperationType::DECRIMENT  :
			operationResult =  rdo::Factory<RDOCalcMinus>::create(localVariableValue, rdo::Factory<RDOCalcConst>::create(1));
			break;
		}

		ASSERT(operationResult);
		return rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&contextSetLocalVariable, pThis, operationResult, srcInfo)));
	}

	return rdo::Factory<FindResult>::create();
}

// --------------------------------------------------------------------------------
// -------------------- LocalVariableList
// --------------------------------------------------------------------------------
LocalVariableList::LocalVariableList()
{}

LocalVariableList::~LocalVariableList()
{}

void LocalVariableList::append(const LPLocalVariable& pVariable)
{
	ASSERT(pVariable);

	LPLocalVariable pExistItem = findLocalVariable(pVariable->getName());

	if (pExistItem)
	{
		parser::g_error().push_only(pVariable->getSrcInfo(),  rdo::format("Переменная %s уже объявлена", pVariable->getName().c_str()));
		parser::g_error().push_only(pExistItem->getSrcInfo(), "См. первое описание");
		parser::g_error().push_done();
	}

	m_variableList.insert(VariableList::value_type(pVariable->getName(), pVariable));
}

LPLocalVariable LocalVariableList::findLocalVariable(const std::string& name) const
{
	VariableList::const_iterator it = m_variableList.find(name);
	return it != m_variableList.end() ? it->second : LPLocalVariable(NULL);
}

// --------------------------------------------------------------------------------
// -------------------- LocalVariableListStack
// --------------------------------------------------------------------------------
LocalVariableListStack::LocalVariableListStack()
{}

void LocalVariableListStack::push(const LPLocalVariableList& pVariableList)
{
	m_pVariableListStack.push_back(pVariableList);
}

void LocalVariableListStack::pop()
{
	ASSERT(!m_pVariableListStack.empty());
	m_pVariableListStack.pop_back();
}

LPLocalVariableList LocalVariableListStack::top() const
{
	ASSERT(!m_pVariableListStack.empty());
	return m_pVariableListStack.back();
}

LPLocalVariable LocalVariableListStack::findLocalVariable(const std::string& name) const
{
	for (const auto& stack: m_pVariableListStack)
	{
		if (stack->findLocalVariable(name))
		{
			return stack->findLocalVariable(name);
		}
	}
	return NULL;
}

CLOSE_RDO_PARSER_NAMESPACE
