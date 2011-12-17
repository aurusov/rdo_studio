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
#include "simulator/compiler/parser/context/context_find_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Context
// --------------------------------------------------------------------------------
class Context: virtual public rdo::counter_reference
{
DECLARE_FACTORY(Context);
friend void ContextStack::push(LPContext pContext);

public:
	template <class T>
	rdo::intrusive_ptr<T> cast();

	LPContext    find  (CREF(LPRDOValue) pValue) const;
	LPContext    swch  (CREF(LPRDOValue) pValue) const;
	LPExpression create(CREF(LPRDOValue) pValue);

protected:
	Context();
	virtual ~Context();

private:
	LPContextStack        m_pContextStack;
	IContextFind::Result  m_findResult;

	void setContextStack(CREF(LPContextStack) pContextStack);
};
DECLARE_POINTER(Context);

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/context/context.inl"

#endif // _RDOPARSER_CONTEXT_H_
