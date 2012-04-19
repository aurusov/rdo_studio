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
// -------------------- ContextStatementBase
// --------------------------------------------------------------------------------
CLASS(ContextStatementBase):
	             INSTANCE_OF (Context)
{
DECLARE_FACTORY(ContextStatementBase);
protected:
	ContextStatementBase();
};
DECLARE_POINTER(ContextStatementBase);

// --------------------------------------------------------------------------------
// -------------------- ContextBreakable
// --------------------------------------------------------------------------------
CLASS(ContextBreakable):
	         INSTANCE_OF (ContextStatementBase)
{
DECLARE_FACTORY(ContextBreakable);
protected:
	ContextBreakable();
};
DECLARE_POINTER(ContextBreakable);

// --------------------------------------------------------------------------------
// -------------------- ContextReturnable
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(ContextReturnable);
CLASS(ContextReturnable):
	         INSTANCE_OF (ContextStatementBase)
{
DECLARE_FACTORY(ContextReturnable);
public:

	typedef std::vector<LPContextReturnable> ContextReturnableList;

	bool returnFlag();
	void addContext(REF(LPContextReturnable) pContext);

private:
	void compileFlags();

protected:
	ContextReturnable();

	ContextReturnableList m_contextReturnableList;
	bool                  m_returnFlag;

};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _CONTEXT_STATEMENT_H_