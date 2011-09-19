/*!
  \copyright (c) RDO-Team, 2011
  \file      stack.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.06.2010
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_CONTEXT_STACK_H_
#define _CONVERTOR_RDOCONVERTER_CONTEXT_STACK_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/context/context.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ContextStack
// --------------------------------------------------------------------------------
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

#endif // _CONVERTOR_RDOCONVERTER_CONTEXT_STACK_H_
