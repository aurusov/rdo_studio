/*!
  \copyright (c) RDO-Team, 2012
  \file      terminate.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор TERMINATE
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
inline RDOPROCTerminate::RDOPROCTerminate(LPIPROCProcess process, ruint _term)
	: RDOPROCBlock(process)
	, term        (_term  )
{}

inline int RDOPROCTerminate::getTerm()
{
	return term;
}

CLOSE_RDO_RUNTIME_NAMESPACE
