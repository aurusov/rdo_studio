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
inline RDOPROCTerminate::RDOPROCTerminate(CREF(LPIPROCProcess) pProcess, CREF(LPRDOCalc) pCalc)
	: RDOPROCBlock             (pProcess)
	, m_terminatedTransactCount(0       )
	, m_pTermCalc              (pCalc   )
{}

CLOSE_RDO_RUNTIME_NAMESPACE
