/*!
  \copyright (c) RDO-Team, 2011
  \file      context.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/format.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_switch_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Context::Params
// --------------------------------------------------------------------------------
const std::string Context::Params::IDENTIFIER = "identifier";

std::string Context::Params::get(const std::string& name) const
{
	return get<std::string>(name);
}

std::string Context::Params::identifier() const
{
	return get(IDENTIFIER);
}

bool Context::Params::exists(const std::string& name) const
{
	return find(name) != end();
}

// --------------------------------------------------------------------------------
// -------------------- Context::SwitchContext
// --------------------------------------------------------------------------------
Context::SwitchContext::SwitchContext()
{}

Context::SwitchContext::SwitchContext(const LPContext& context)
	: context(context)
{}

Context::SwitchContext::SwitchContext(const LPContext& context, const Context::Params& params)
	: context(context)
	, params(params)
{}

Context::SwitchContext::operator bool() const
{
	return context;
}

// --------------------------------------------------------------------------------
// -------------------- Context::FindResult
// --------------------------------------------------------------------------------
Context::FindResult::FindResult()
{}

Context::FindResult::FindResult(const CreateExpression& createExpression)
	: createExpression(createExpression.function)
{}

Context::FindResult::FindResult(const SwitchContext& switchContext)
	: switchContext(switchContext)
{}

Context::FindResult::operator bool() const
{
	return createExpression || switchContext;
}

const Context::CreateExpressionFunction& Context::FindResult::getCreateExpression() const
{
	return createExpression;
}

const Context::SwitchContext& Context::FindResult::getSwitchContext() const
{
	return switchContext;
}

// --------------------------------------------------------------------------------
// -------------------- Context
// --------------------------------------------------------------------------------
const std::string Context::METHOD_GET = "get()";
const std::string Context::METHOD_SET = "set()";
const std::string Context::METHOD_TYPE_OF = "type_if()";
const std::string Context::METHOD_OPERATOR_BRACKETS = "operator()()";
const std::string Context::METHOD_OPERATOR_DOT = "operator.()";

Context::Context()
{}

Context::~Context()
{}

void Context::setContextStack(CREF(LPContextStack) pContextStack)
{
	ASSERT(pContextStack   );
	ASSERT(!m_pContextStack);
	m_pContextStack = pContextStack;
}

void Context::resetContextStack()
{
	ASSERT(m_pContextStack);
	m_pContextStack = NULL;
}

Context::FindResult Context::find(const std::string& method, const Params& params, const RDOParserSrcInfo& srcInfo) const
{
	Context::FindResult result;
	LPContext pThis(const_cast<PTR(Context)>(this));
	LPIContextFind pThisContextFind = pThis.interface_dynamic_cast<IContextFind>();
	if (pThisContextFind)
	{
		result = pThisContextFind->onFindContext(method, params, srcInfo);
		if (result.getCreateExpression() || result.getSwitchContext())
		{
			return result;
		}
	}

	LPContext pPrev;

	if (m_pContextStack)
	{
		pPrev = m_pContextStack->prev(pThis);
	}

	if (!pPrev)
	{
		const std::string error = params.exists(Params::IDENTIFIER)
			? boost::str(boost::format("Неизвестный идентификатор: %s") % params.identifier())
			: "Неизвестная ошибка";
		RDOParser::s_parser()->error().error(srcInfo, error);
	}

	return pPrev->find(method, params, srcInfo);
}

CLOSE_RDO_PARSER_NAMESPACE
