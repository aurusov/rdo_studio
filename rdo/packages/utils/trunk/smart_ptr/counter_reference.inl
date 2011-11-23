/*!
  \copyright (c) RDO-Team, 2011
  \file      counter_reference.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.06.2010
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

FORCE_INLINE counter_reference::counter_reference()
	: m_intrusive_counter(0)
{}

CLOSE_RDO_NAMESPACE
