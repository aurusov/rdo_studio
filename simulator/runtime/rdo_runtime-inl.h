/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_runtime-inl.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      06.08.2011
  \brief     RDORuntime implementation
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResults
// --------------------------------------------------------------------------------
template<class T>
inline RDOResults& RDOResults::operator<< (const T& value)
{
	getOStream() << value;
	return *this;
}

CLOSE_RDO_RUNTIME_NAMESPACE
