/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/context/stack.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_CONTEXT_STACK_H_
#define _RDOPARSER_CONTEXT_STACK_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/compiler/parser/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextStack
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(Context);

OBJECT(ContextStack)
{
DECLARE_FACTORY(ContextStack);
public:
	void                     push(LPContext pContext);
	template <class T> void  pop ()
	{
		ASSERT(top().object_dynamic_cast<T>());
		pop_not_safed();
	}

	LPContext top   () const;
	LPContext global() const;
	LPContext prev  (CREF(LPContext) pContext) const;

	void pop_not_safed();

private:
	ContextStack();
	virtual ~ContextStack();

	typedef std::list<LPContext> Container;

	Container m_container;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_CONTEXT_STACK_H_
