/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic.inl
 * author   : Урусов Андрей
 * date     : 03.10.09
 * bref     : 
 * indent   : 4T
 */

// =========================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdotrace.h"
#include "rdo_simulator.h"
#include "rdo_priority.h"
#include "rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

#pragma warning(disable : 4786)  

#define STL_FOR_ALL(TYPE, CONTAINER, IT) for (TYPE::iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)
#define LOGIC_SERIAL_BYPASS()            STL_FOR_ALL(ChildList, m_childList, it)

/*
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
*/

// ----------------------------------------------------------------------------
// ---------- OrderFIFO
// ----------------------------------------------------------------------------
inline LPIBaseOperation OrderFIFO::sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container)
{
	STL_FOR_ALL(BaseOperationList, container, it)
	{
		if ((*it)->onCheckCondition(sim))
		{
			return *it;
		}
	}
	return NULL;
}

// ----------------------------------------------------------------------------
// ---------- RDOLogic
// ----------------------------------------------------------------------------
template <class Order>
inline RDOLogic<Order>::RDOLogic()
	: m_condition    (NULL )
	, m_lastCondition(false)
	, m_first        (NULL )
{}

template <class Order>
inline RDOLogic<Order>::~RDOLogic()
{}

template <class Order>
inline void RDOLogic<Order>::init(PTR(RDOSimulator) sim)
{
	if (sim)
		sim->appendLogic(rdo::UnknownPointer(this).query_cast<IBaseOperation>());
}

template <class Order>
inline void RDOLogic<Order>::setCondition(PTR(RDOCalc) calc)
{
	m_condition = calc;
}

template <class Order>
inline void RDOLogic<Order>::onStart(PTR(RDOSimulator) sim)
{
	m_lastCondition = checkSelfCondition(sim);
	if (m_lastCondition)
		start(sim);
}

template <class Order>
inline void RDOLogic<Order>::onStop(PTR(RDOSimulator) sim)
{
	m_lastCondition = false;
	stop(sim);
}

template <class Order>
inline rbool RDOLogic<Order>::onCheckCondition(PTR(RDOSimulator) sim)
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

	if (!condition)
		return false;

	m_first = Order::sort(sim, m_childList);
	return m_first ? true : false;
}

template <class Order>
inline IBaseOperation::BOResult RDOLogic<Order>::onDoOperation(PTR(RDOSimulator) sim)
{
	if (m_lastCondition)
	{
		if (!m_first)
			return IBaseOperation::BOR_cant_run;

		IBaseOperation::BOResult result = m_first->onDoOperation(sim);
		if (result == IBaseOperation::BOR_must_continue)
			sim->setMustContinueOpr(m_first);

		return result;
	}
	else
	{
		return IBaseOperation::BOR_cant_run;
	}
}

template <class Order>
inline void RDOLogic<Order>::onMakePlaned(PTR(RDOSimulator) sim, PTR(void) param)
{
	LOGIC_SERIAL_BYPASS()
		(*it)->onMakePlaned(sim, param);
}

template <class Order>
inline IBaseOperation::BOResult RDOLogic<Order>::onContinue(PTR(RDOSimulator) sim)
{
	LOGIC_SERIAL_BYPASS()
	{
		if ((*it)->onContinue(sim) == IBaseOperation::BOR_must_continue)
			return IBaseOperation::BOR_must_continue;
	}
	return IBaseOperation::BOR_cant_run;
}

template <class Order>
inline rbool RDOLogic<Order>::checkSelfCondition(PTR(RDOSimulator) sim)
{
	return m_condition ? m_condition->calcValue(static_cast<PTR(RDORuntime)>(sim)).getAsBool() : true;
}

template <class Order>
inline void RDOLogic<Order>::start(PTR(RDOSimulator) sim)
{
	LOGIC_SERIAL_BYPASS()
		(*it)->onStart(sim);
}

template <class Order>
inline void RDOLogic<Order>::stop(PTR(RDOSimulator) sim)
{
	LOGIC_SERIAL_BYPASS()
		(*it)->onStop(sim);
}

template <class Order>
inline rbool RDOLogic<Order>::empty() const
{
	return m_childList.empty();
}

template <class Order>
inline typename RDOLogic<Order>::Iterator RDOLogic<Order>::begin()
{
	return m_childList.begin();
}

template <class Order>
inline typename RDOLogic<Order>::Iterator RDOLogic<Order>::end()
{
	return m_childList.end();
}

template <class Order>
inline typename RDOLogic<Order>::CIterator RDOLogic<Order>::begin() const
{
	return m_childList.begin();
}

template <class Order>
inline typename RDOLogic<Order>::CIterator RDOLogic<Order>::end() const
{
	return m_childList.end();
}

template <class Order>
inline REF(LPIBaseOperation) RDOLogic<Order>::back()
{
	return m_childList.back();
}

template <class Order>
inline void RDOLogic<Order>::append(CREF(Item) item)
{
	m_childList.push_back(item);
}

template <class Order>
inline void RDOLogic<Order>::clear()
{
	m_childList.clear();
}

CLOSE_RDO_RUNTIME_NAMESPACE
