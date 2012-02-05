/*!
  \copyright (c) RDO-Team, 2011
  \file      context_find_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.03.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_FIND_I_H_
#define _RDOPARSER_CONTEXT_FIND_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/expression.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSE_NAMESPACE

PREDECLARE_POINTER(Context);

// --------------------------------------------------------------------------------
// -------------------- IContextFind
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IContextFind)
{
DECLARE_FACTORY(IContextFind)
public:
	struct Result
	{
		LPContext     m_pContext;
		LPExpression  m_pExpression;
		LPRDOValue    m_pFindByValue;
		LPContext     m_pValueContext;

		Result();
		Result(CREF(Result) result);
		Result(CREF(LPContext) pContext, CREF(LPExpression) pExpression, CREF(LPRDOValue) pFindByParam, LPContext pValueContext = LPContext());
	};
	virtual Result onFindContext(CREF(LPRDOValue) pValue) const = 0;

protected:
	         IContextFind();
	virtual ~IContextFind();
};
#define DECLARE_IContextFind \
	IContextFind::Result onFindContext(CREF(LPRDOValue) pValue) const;

CLOSE_RDO_PARSE_NAMESPACE

#endif // _RDOPARSER_CONTEXT_FIND_I_H_
