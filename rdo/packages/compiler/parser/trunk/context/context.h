/*!
  \copyright (c) RDO-Team, 2011
  \file      context.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_H_
#define _RDOPARSER_CONTEXT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/expression.h"
#include "simulator/compiler/parser/context/stack.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Context
// --------------------------------------------------------------------------------
OBJECT(Context)
{
DECLARE_FACTORY(Context);
friend void ContextStack::push(LPContext pContext);

public:
	template <class T>
	rdo::intrusive_ptr<T> cast();

	LPContext    find  (CREF(RDOValue) value) const;
	LPExpression create(CREF(RDOValue) value);

protected:
	Context();
	virtual ~Context();

private:
	LPContextStack m_pContextStack;

	void setContextStack(CREF(LPContextStack) pContextStack);
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/context/context.inl"

#endif // _RDOPARSER_CONTEXT_H_
