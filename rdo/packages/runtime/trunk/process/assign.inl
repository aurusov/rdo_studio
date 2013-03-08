/*!
  \copyright (c) RDO-Team, 2012
  \file      assign.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор ASSIGN
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
inline RDOPROCAssign::RDOPROCAssign(LPIPROCProcess pProcess, CREF(LPRDOCalc) pCalc)
	: RDOPROCBlock(pProcess)
	, pAssignCalc (pCalc   )
{}

CLOSE_RDO_RUNTIME_NAMESPACE
