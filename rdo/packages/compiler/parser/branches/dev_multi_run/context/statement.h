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
	rbool getReturnFlag() const;
	void  setReturnFlag();

	void addChildContext();

protected:
	ContextReturnable();
	virtual ~ContextReturnable();

private:
	typedef std::vector<LPContextReturnable> ContextReturnableList;

	ContextReturnableList m_contextReturnableList;
	rbool                 m_returnFlag;

	rbool getChildFlags() const;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _CONTEXT_STATEMENT_H_