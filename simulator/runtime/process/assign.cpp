/*!
  \copyright (c) RDO-Team, 2012
  \file      assign.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор ASSIGN
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/process/assign.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
bool RDOPROCAssign::onCheckCondition(const LPRDORuntime& /*pRuntime*/)
{
	return !m_transacts.empty();
}

IBaseOperation::BOResult RDOPROCAssign::onDoOperation(const LPRDORuntime& pRuntime)
{
	pAssignCalc->calcValue(pRuntime);
	TRACE1("%7.1f ASSIGN\n", pRuntime->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCAssign::onStart(const LPRDORuntime& /*pRuntime*/)
{}

void RDOPROCAssign::onStop(const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::BOResult RDOPROCAssign::onContinue(const LPRDORuntime& /*pRuntime*/)
{
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
