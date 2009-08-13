/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic.cpp
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// =========================================================================== PCH
#include "pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_logic.h"
#include "rdo_simulator.h"
#include "rdo_priority.h"
#include "rdo_runtime.h"
// ===============================================================================

#pragma warning(disable : 4786)  

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOOprContainer
// ----------------------------------------------------------------------------
class RDOOprContainer: public IBaseOperation, public IBaseOperationContainer
{
DEFINE_FACTORY(RDOOprContainer)
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IBaseOperationContainer)
QUERY_INTERFACE_END

private:
	List m_items;
	Item m_first;

	RDOOprContainer()
	{}
	virtual ~RDOOprContainer()
	{}

	DECLARE_IBaseOperation;
	DECLARE_IBaseOperationContainer;
};

rbool RDOOprContainer::empty() const
{
	return m_items.empty();
}

RDOOprContainer::Iterator RDOOprContainer::begin()
{
	return m_items.begin();
}

RDOOprContainer::Iterator RDOOprContainer::end()
{
	return m_items.end();
}

RDOOprContainer::CIterator RDOOprContainer::begin() const
{
	return m_items.begin();
}

RDOOprContainer::CIterator RDOOprContainer::end() const
{
	return m_items.end();
}

REF(LPIBaseOperation) RDOOprContainer::back()
{
	return m_items.back();
}

void RDOOprContainer::append(CREF(Item) item)
{
	if (item)
		m_items.push_back(item);
}

void RDOOprContainer::onStart(PTR(RDOSimulator) sim)
{
	for (Iterator it = begin(); it != end(); it++)
		(*it)->onStart(sim);
}

void RDOOprContainer::onStop(PTR(RDOSimulator) sim)
{
	for (Iterator it = begin(); it != end(); it++)
		(*it)->onStop(sim);
}

bool RDOOprContainer::onCheckCondition(PTR(RDOSimulator) sim)
{
	for (Iterator it = begin(); it != end(); it++)
	{
		if ((*it)->onCheckCondition(sim))
		{
			m_first = *it;
			return true;
		}
	}
	m_first = NULL;
	return false;
}

IBaseOperation::BOResult RDOOprContainer::onDoOperation(PTR(RDOSimulator) sim)
{
	if (!m_first)
	{
		if (!onCheckCondition(sim) || !m_first)
		{
			return IBaseOperation::BOR_cant_run;
		}
	}
	IBaseOperation::BOResult result = m_first->onDoOperation(sim);
	if (result == IBaseOperation::BOR_must_continue)
		sim->setMustContinueOpr(m_first);
	return result;
}

void RDOOprContainer::onMakePlaned(PTR(RDOSimulator) sim, PTR(void) param)
{
	for (Iterator it = begin(); it != end(); it++)
		(*it)->onMakePlaned(sim, param);
}

IBaseOperation::BOResult RDOOprContainer::onContinue(PTR(RDOSimulator) sim)
{
	for (Iterator it = begin(); it != end(); it++)
	{
		if ((*it)->onContinue(sim) == IBaseOperation::BOR_must_continue)
			return IBaseOperation::BOR_must_continue;
	}
	return IBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOLogic
// ----------------------------------------------------------------------------
RDOLogic::RDOLogic()
	: m_condition    (NULL)
	, m_lastCondition(false)
{
	m_childList = F(RDOOprContainer)::create();
	ASSERT(m_childList);
}

RDOLogic::RDOLogic(PTR(RDOSimulator) sim)
	: m_condition    (NULL)
	, m_lastCondition(false)
{
	m_childList = F(RDOOprContainer)::create();
	ASSERT(m_childList);
}

void RDOLogic::init(PTR(RDOSimulator) sim)
{
	if (sim)
		sim->appendLogic(rdo::UnknownPointer(this).query_cast<IBaseOperation>());
}

void RDOLogic::setCondition(PTR(RDOCalc) calc)
{
	m_condition = calc;
}

void RDOLogic::actionWithRDOOprContainer(PTR(RDOSimulator) sim)
{
	PTR(RDORuntime) runtime = static_cast<PTR(RDORuntime)>(sim);
	for (CIterator it = begin(); it != end(); ++it)
	{
		LPIPriority pattern = *it;
		if (pattern)
		{
			PTR(RDOCalc) prior = pattern->getPrior();
			if (prior)
			{
				RDOValue value = prior->calcValue(runtime);
				if (value < 0 || value > 1)
					runtime->error(rdo::format(_T("Приоритет активности вышел за пределы диапазона [0..1]: %s"), value.getAsString().c_str()), prior);
			}
		}
	}
	std::sort(begin(), end(), RDODPTActivityCompare(static_cast<PTR(RDORuntime)>(sim)));
}

void RDOLogic::onStart(PTR(RDOSimulator) sim)
{
	m_lastCondition = checkSelfCondition(sim);
	if (m_lastCondition)
		start(sim);
}

void RDOLogic::onStop(PTR(RDOSimulator) sim)
{
	m_lastCondition = false;
	stop(sim);
}

rbool RDOLogic::onCheckCondition(PTR(RDOSimulator) sim)
{
	rbool condition = checkSelfCondition(sim);
	if (condition != m_lastCondition)
	{
		m_lastCondition = condition;
		if (condition)
			start(sim);
		else
			stop(sim);
	}
	if (condition)
	{
		actionWithRDOOprContainer(sim);
		return m_childList.query_cast<IBaseOperation>()->onCheckCondition(sim);
	}
	return false;
}

IBaseOperation::BOResult RDOLogic::onDoOperation(PTR(RDOSimulator) sim)
{
	if (m_lastCondition)
	{
		return m_childList.query_cast<IBaseOperation>()->onDoOperation(sim);
	}
	else
	{
		return IBaseOperation::BOR_cant_run;
	}
}

void RDOLogic::onMakePlaned(PTR(RDOSimulator) sim, PTR(void) param)
{
	m_childList.query_cast<IBaseOperation>()->onMakePlaned(sim, param);
}

IBaseOperation::BOResult RDOLogic::onContinue(PTR(RDOSimulator) sim)
{
	return m_childList.query_cast<IBaseOperation>()->onContinue(sim);
}

rbool RDOLogic::checkSelfCondition(PTR(RDOSimulator) sim)
{
	return m_condition ? m_condition->calcValue(static_cast<PTR(RDORuntime)>(sim)).getAsBool() : true;
}

void RDOLogic::start(PTR(RDOSimulator) sim)
{
	m_childList.query_cast<IBaseOperation>()->onStart(sim);
}

void RDOLogic::stop(PTR(RDOSimulator) sim)
{
	m_childList.query_cast<IBaseOperation>()->onStop(sim);
}

rbool RDOLogic::empty() const
{
	return m_childList->empty();
}

RDOLogic::Iterator RDOLogic::begin()
{
	return m_childList->begin();
}

RDOLogic::Iterator RDOLogic::end()
{
	return m_childList->end();
}

RDOLogic::CIterator RDOLogic::begin() const
{
	return m_childList->begin();
}

RDOLogic::CIterator RDOLogic::end() const
{
	return m_childList->end();
}

REF(LPIBaseOperation) RDOLogic::back()
{
	return m_childList->back();
}

void RDOLogic::append(CREF(Item) item)
{
	m_childList->append(item);
}

CLOSE_RDO_RUNTIME_NAMESPACE
