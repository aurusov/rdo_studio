/*
  \copyright (c) RDO-Team, 2011
  \file      context.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
 */

#ifndef _RDOPARSER_CONTEXT_H_
#define _RDOPARSER_CONTEXT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/expression.h"
#include "rdo_lib/rdo_parser/context/stack.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Context
// ----------------------------------------------------------------------------
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

#include "rdo_lib/rdo_parser/context/context.inl"

#endif //! _RDOPARSER_CONTEXT_H_
