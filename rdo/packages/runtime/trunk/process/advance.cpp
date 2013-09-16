/*!
  \copyright (c) RDO-Team, 2012
  \file      advance.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор ADVANCE
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
#include "utils/src/common/rdomacros.h"
#include "simulator/runtime/process/advance.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
rbool RDOPROCAdvance::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	if (!m_transacts.empty())
	{
		return true;
	} 
	else
	{
		if (!leave_list.empty())
		{
			double tnow = pRuntime->getCurrentTime();
			std::list< LeaveTr >::iterator it = leave_list.begin();
			while (it != leave_list.end())
			{
				if (tnow >= it->timeLeave)
				{
					return true;
				}
				++it;
			}
		}
	}
	return false;
}

IBaseOperation::BOResult RDOPROCAdvance::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	if (!m_transacts.empty())
	{
		//		TRACE1("%7.1f ADVANCE BEGIN\n", pRuntime->getCurrentTime());
		double timeLeave = pDelayCalc->calcValue(pRuntime).getDouble() + pRuntime->getCurrentTime();
		leave_list.push_back(LeaveTr(m_transacts.front(), timeLeave));
		m_transacts.erase(m_transacts.begin());
		pRuntime->addTimePoint(timeLeave, m_process, this);

		if (m_pStatistics)
			m_pStatistics->setTransCount(m_transacts.size());

		return IBaseOperation::BOR_done;
	} 
	else if (!leave_list.empty())
	{
		double tnow = pRuntime->getCurrentTime();
		std::list<LeaveTr>::iterator it = leave_list.begin();
		while (it != leave_list.end())
		{
			if (tnow >= it->timeLeave)
			{
				//				TRACE1("%7.1f ADVANCE END\n", it->timeLeave);
				it->transact->next();
				leave_list.erase(it);
				return IBaseOperation::BOR_planned_and_run;
			}
			++it;
		}
	}
	return IBaseOperation::BOR_cant_run;
}

void RDOPROCAdvance::onStart(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCAdvance::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCAdvance::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	UNUSED(pRuntime);
	UNUSED(pParam  );
}

IBaseOperation::BOResult RDOPROCAdvance::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

void RDOPROCAdvance::setStatistics(CREF(rdo::runtime::LPIInternalStatistics) pStatistics)
{
	m_pStatistics = pStatistics;
}

CLOSE_RDO_RUNTIME_NAMESPACE
