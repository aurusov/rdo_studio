/*
 * copyright: (c) RDO-Team, 2010
 * filename : stack.h
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCONVERTER_CONTEXT_STACK_H_
#define _RDOCONVERTER_CONTEXT_STACK_H_

// ====================================================================== INCLUDES
#include <list>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/context/context.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

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

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _RDOCONVERTER_CONTEXT_STACK_H_
