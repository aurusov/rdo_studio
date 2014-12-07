// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
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
	m_pMetaLogic = rdo::Factory<RDOLogicMeta>::create();
}

RDOSimulator::~RDOSimulator()
{}

LPIBaseOperation RDOSimulator::getMustContinueOpr() const
{
	return m_pOprMustContinue;
}

void RDOSimulator::setMustContinueOpr(const LPIBaseOperation& pOperation)
{
	m_pOprMustContinue = pOperation;
}

std::size_t RDOSimulator::getSizeofSim() const
{
	return m_sizeofSim;
}

void RDOSimulator::appendBaseOperation(LPIBaseOperationContainer pLogic, const LPIBaseOperation& pBaseOperation)
{
	ASSERT(pLogic        );
	ASSERT(pBaseOperation);
	pLogic->append(pBaseOperation);
}

void RDOSimulator::appendLogic(const LPIBaseOperation& pLogic, LPIBaseOperationContainer pParent)
{
	ASSERT(pParent);
	pParent->append(pLogic);
}

bool RDOSimulator::doOperation()
{
	LPRDORuntime pRuntime(static_cast<RDORuntime*>(this));

	bool res;
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
		bool foundPlaned = false;
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
			LPIBaseOperation pMetaLogic = m_pMetaLogic.object_dynamic_cast<IBaseOperation>();
			try
			{
				res = pMetaLogic->onCheckCondition(pRuntime);
			}
			catch (const RDOUndefinedException&)
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
	LPRDORuntime pRuntime(static_cast<RDORuntime*>(this));
	m_pMetaLogic.object_dynamic_cast<IBaseOperation>()->onStart(pRuntime);
	onResetResult();
}

std::string writeActivitiesStructureRecurse(const LPIBaseOperationContainer& pLogic, std::size_t& counter)
{
	std::stringstream stream;
	IBaseOperationContainer::CIterator it = pLogic->begin();
	while (it != pLogic->end())
	{
		LPIModelStructure pModelStructure = it->object_dynamic_cast<IModelStructure>();
		if (pModelStructure && (pModelStructure.object_dynamic_cast<IRule>() || pModelStructure.object_dynamic_cast<IOperation>()))
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

	std::size_t _counter = 1;
	it = pLogic->begin();
	while (it != pLogic->end())
	{
		LPIModelStructure pModelStructure = it->object_dynamic_cast<IModelStructure>();
		if (pModelStructure && pModelStructure.object_dynamic_cast<IEvent>() && !pModelStructure.object_dynamic_cast<IOperation>())
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
		LPILogic pLogic = it->object_dynamic_cast<ILogic>();
		if (pLogic)
		{
			stream << writeActivitiesStructureRecurse(pLogic.object_dynamic_cast<IBaseOperationContainer>(), counter);
		}
		++it;
	}

	return stream.str();
}

std::string RDOSimulator::writeActivitiesStructure(std::size_t& counter)
{
	return writeActivitiesStructureRecurse(m_pMetaLogic, counter);
}

CLOSE_RDO_RUNTIME_NAMESPACE
