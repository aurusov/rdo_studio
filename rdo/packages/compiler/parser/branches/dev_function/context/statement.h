/*!
  \copyright (c) RDO-Team, 2012
  \file      statement.h
  \authors   Поподьянец Евгений (kurt.gigacore@gmail.com)
  \date      18.04.2012
  \brief     Контекст выражений
  \indent    4T
*/

#ifndef _CONTEXT_STATEMENT_H_
#define _CONTEXT_STATEMENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextBreakable
// --------------------------------------------------------------------------------
class ContextBreakable: public Context
{
DECLARE_FACTORY(ContextBreakable);
private:
	ContextBreakable();
	virtual ~ContextBreakable();
};
DECLARE_POINTER(ContextBreakable);

// --------------------------------------------------------------------------------
// -------------------- ContextReturnable
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ContextReturnable);
class ContextReturnable: public Context
{
DECLARE_FACTORY(ContextReturnable);
public:
	void addContext(REF(LPContextReturnable) pContext);

	void setReturnFlag();
	bool returnFlag   ();

private:
	ContextReturnable();
	virtual ~ContextReturnable();

	typedef std::vector<LPContextReturnable> ContextReturnableList;

	ContextReturnableList m_contextReturnableList;
	bool                  m_returnFlag;

	bool checkChildFlags();
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _CONTEXT_STATEMENT_H_