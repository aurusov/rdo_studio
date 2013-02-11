/*!
  \copyright (c) RDO-Team, 2011
  \file      context.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.03.2011
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/expression.h"
#include "simulator/compiler/parser/context/stack.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

template <class T>
inline rdo::intrusive_ptr<T> Context::cast()
{
	LPContext pThis = this;
	rdo::intrusive_ptr<T> pThisResult = pThis.object_dynamic_cast<T>();
	if (pThisResult)
	{
		return pThisResult;
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->cast<T>() : rdo::intrusive_ptr<T>();
}

template <class T>
inline rdo::interface_ptr<T> Context::interface_cast()
{
	LPContext pThis = this;
	rdo::interface_ptr<T> pThisResult = pThis.interface_dynamic_cast<T>();
	if (pThisResult)
	{
		return pThisResult;
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->interface_cast<T>() : rdo::interface_ptr<T>();
}

CLOSE_RDO_PARSER_NAMESPACE
