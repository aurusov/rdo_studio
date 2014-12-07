#pragma once

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
	bool getReturnFlag() const;
	void setReturnFlag();

	void addChildContext();

protected:
	ContextReturnable();
	virtual ~ContextReturnable();

private:
	typedef std::vector<LPContextReturnable> ContextReturnableList;

	ContextReturnableList m_contextReturnableList;
	bool m_returnFlag;

	bool getChildFlags() const;
};

CLOSE_RDO_PARSER_NAMESPACE
