/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_simulator.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      19.04.2008
  \brief     Симулятор РДО
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/stream/rdostream.h"
#include "simulator/runtime/rdo_simulator.h"
#include "simulator/runtime/rdo_logic_dptprior.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/runtime/rdo_exception.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif // COMPILER_VISUAL_STUDIO

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSimulator
// --------------------------------------------------------------------------------
RDOSimulator::RDOSimulator()
	: RDOSimulatorBase( )
	, m_sizeofSim     (0)
{
	m_pMetaLogic = RF(RDOLogicMeta)::create();
}

RDOSimulator::~RDOSimulator()
{}

void RDOSimulator::appendLogic(CREF(LPIBaseOperation) pLogic, LPIBaseOperationContainer pParent)
{
	ASSERT(pParent);
	pParent->append(pLogic);
}

rbool RDOSimulator::doOperation()
{
	LPRDORuntime pRuntime(static_cast<PTR(RDORuntime)>(this));

	rbool res;
	if (getMustContinueOpr())
	{
		// Есть действие, которое необходимо продолжить. Используется в DPT
		IBaseOperation::BOResult result = getMustContinueOpr()->onContinue(pRuntime);
		if (result != IBaseOperation::BOR_must_continue)
		{
			setMustContinueOpr(NULL);
		}
		return result != IBaseOperation::BOR_cant_run;
	}
	else
	{
		rbool foundPlaned = false;
		// Отработаем все запланированные на данный момент события
		if (!m_checkOperation && !m_timePoints.empty())
		{
			m_checkOperation = true;
			BOPlannedMap::iterator begin = m_timePoints.begin();
			double newTime = begin->first;
			if (getCurrentTime() >= newTime)
			{
				BOPlannedList& pList = begin->second;
				if (!pList.empty())
				{
#ifdef RDOSIM_COMPATIBLE
					// Дисциплина списка текущих событий LIFO
					LPIBaseOperation pOperation = pList.back().first;
					EventFunction eventFunction = pList.back().second;
					pList.pop_back();
#else
					// Дисциплина списка текущих событий FIFO
					LPIBaseOperation pOperation = pList.front().first;
					EventFunction eventFunction = pList.front().second;
					pList.pop_front();
#endif // RDOSIM_COMPATIBLE
					if (pList.empty())
					{
						m_timePoints.erase(begin);
					}
#ifndef RDO_703_COMPATIBLE
					else
					{
						m_checkOperation = false;
					}
#endif // not RDO_703_COMPATIBLE
					eventFunction();
					foundPlaned = true;
				}
			}
		}
		res = foundPlaned;
		if (!foundPlaned)
		{
			// Не нашли запланированное событие
			// Проверить все возможные события и действия, вызвать первое, которое может быть вызвано
			LPIBaseOperation pMetaLogic = m_pMetaLogic.query_cast<IBaseOperation>();
			try
			{
				res = pMetaLogic->onCheckCondition(pRuntime);
			}
			catch (CREF(RDOUndefinedException))
			{
				res = false;
			}
			if (res)
			{
				res = pMetaLogic->onDoOperation(pRuntime) != IBaseOperation::BOR_cant_run;
			}
			if (!res)
			{
				m_checkOperation = false;
			}
		}
	}
	onCheckResult     ();
	onAfterCheckResult();
	return res;
}

void RDOSimulator::preProcess()
{
	LPRDORuntime pRuntime(static_cast<PTR(RDORuntime)>(this));
	m_pMetaLogic.query_cast<IBaseOperation>()->onStart(pRuntime);
	onResetResult();
}

tstring writeActivitiesStructureRecurse(CREF(LPIBaseOperationContainer) pLogic, REF(ruint) counter)
{
	rdo::textstream stream;
	IBaseOperationContainer::CIterator it = pLogic->begin();
	while (it != pLogic->end())
	{
		LPIModelStructure pModelStructure = *it;
		if (pModelStructure && (pModelStructure.query_cast<IRule>() || pModelStructure.query_cast<IOperation>()))
		{
			stream << counter++ << " ";
			pModelStructure->writeModelStructure(stream);
		}
		++it;
	}
#ifdef RDOSIM_COMPATIBLE
#else
	stream << std::endl;
#endif

	ruint _counter = 1;
	it = pLogic->begin();
	while (it != pLogic->end())
	{
		LPIModelStructure pModelStructure = *it;
		if (pModelStructure && pModelStructure.query_cast<IEvent>())
		{
			stream << _counter++ << " ";
			counter++;
			pModelStructure->writeModelStructure(stream);
		}
		++it;
	}

	it = pLogic->begin();
	while (it != pLogic->end())
	{
		LPILogic pLogic = *it;
		if (pLogic)
		{
			stream << writeActivitiesStructureRecurse(pLogic, counter);
		}
		++it;
	}

	return stream.str();
}

tstring RDOSimulator::writeActivitiesStructure(REF(ruint) counter)
{
	return writeActivitiesStructureRecurse(m_pMetaLogic, counter);
}

CLOSE_RDO_RUNTIME_NAMESPACE
