/*!
  \copyright (c) RDO-Team, 2012
  \file      advance.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор ADVANCE
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
inline RDOPROCAdvance::RDOPROCAdvance(LPIPROCProcess process, CREF(LPRDOCalc) _pDelayCalc)
	: RDOPROCBlock(process    )
	, pDelayCalc  (_pDelayCalc)
{
	m_transCountProc = 0;
}

inline RDOPROCAdvance::LeaveTr::LeaveTr(CREF(LPRDOPROCTransact) _transact, double _timeLeave)
	: transact (_transact )
	, timeLeave(_timeLeave)
{}

CLOSE_RDO_RUNTIME_NAMESPACE
