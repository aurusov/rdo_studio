/*
 * copyright: (c) RDO-Team, 2010
 * filename : stack.h
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_STACK_H_
#define _RDOPARSER_CONTEXT_STACK_H_

// ====================================================================== INCLUDES
#include <list>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/context.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextStack
// ----------------------------------------------------------------------------
OBJECT(ContextStack)
{
public:
	ContextStack();
	virtual ~ContextStack();

	void      push  (CREF(LPContext) pContext);
	void      pop   ();

	LPContext top   () const;
	LPContext global() const;

private:
	typedef std::list<LPContext> Container;

	Container m_container;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_STACK_H_
