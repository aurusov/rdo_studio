/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_runtime.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      06.08.2011
  \brief     RDORuntime implementation
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResults
// --------------------------------------------------------------------------------
inline RDOResults::RDOResults()
{}

inline RDOResults::~RDOResults()
{}

inline void RDOResults::width(ruint w)
{
	getOStream().width(w);
}

template<class T> 
inline REF(RDOResults) RDOResults::operator<< (CREF(T) value)
{
	getOStream() << value;
	return *this;
}

// --------------------------------------------------------------------------------
// -------------------- RDORuntime
// --------------------------------------------------------------------------------
inline REF(Notify) RDORuntime::notify()
{
	return m_notify;
}

inline REF(Error) RDORuntime::error()
{
	return m_error;
}

inline REF(RDOHotKey) RDORuntime::hotkey()
{
	return m_hotKey;
}

inline REF(RDORuntime::Stack) RDORuntime::stack()
{
	return m_stack;
}

inline REF(RDOResults) RDORuntime::getResults()
{
	return *m_resultList;
}

inline REF(RDOResults) RDORuntime::getResultsInfo()
{
	return *m_resultListInfo;
}

inline double RDORuntime::getTimeNow()
{
	return getCurrentTime();
}

inline double RDORuntime::getSeconds()
{
	/// @todo использовать явный cast()
	return (double)(time(NULL) - m_physicTime);
}

inline ruint RDORuntime::getCurrentTerm() const
{
	return m_currentTerm;
}

inline void RDORuntime::setCurrentTerm(ruint value)
{
	m_currentTerm = value;
}

inline REF(LPIActivity) RDORuntime::getCurrentActivity()
{
	return m_currActivity;
}

inline void RDORuntime::setCurrentActivity(CREF(LPIActivity) activity)
{
	m_currActivity = activity;
}

inline CREF(RDORuntime::LPIResultList) RDORuntime::getResult() const
{
	return m_resultAllList;
}

inline void RDORuntime::addInitCalc(CREF(LPRDOCalc) initCalc)
{
	m_initCalcList.push_back(initCalc);
}

inline REF(RDOValue) RDORuntime::getResParamValRaw(ruint resID, ruint paramID)
{
	LPRDOResource pResource = getResourceByID(resID);
	ASSERT(pResource);
	return pResource->getParamRaw(paramID);
}

inline void RDORuntime::setResParamVal(ruint resID, ruint paramID, CREF(RDOValue) value)
{
	LPRDOResource pResource = getResourceByID(resID);
	ASSERT(pResource);
	pResource->setParam(paramID, value);
}

inline LPRDOResource RDORuntime::getGroupFuncRes() const
{
	return m_groupFuncStack.back();
}

inline void RDORuntime::pushFuncArgument(RDOValue arg)
{
	m_funcStack.push_back(arg);
}

inline void RDORuntime::pushGroupFunc(CREF(LPRDOResource) pResource)
{
	m_groupFuncStack.push_back(pResource);
}

inline void RDORuntime::popFuncArgument()
{
	m_funcStack.pop_back();
}

inline void RDORuntime::popGroupFunc()
{
	m_groupFuncStack.pop_back();
}

inline void RDORuntime::pushFuncTop()
{
	m_funcStack.push_back(RDOValue(m_currFuncTop));
}

inline void RDORuntime::resetFuncTop(int numArg)
{
	m_currFuncTop = m_funcStack.size() - numArg;
}

inline void RDORuntime::popFuncTop()
{
	m_currFuncTop = m_funcStack.back().getInt();
	m_funcStack.pop_back();
}

inline LPRDOResource RDORuntime::getResourceByID(ruint resourceID) const
{
	return resourceID != ~0 ? m_resourceListByID[resourceID] : LPRDOResource(NULL);
}

inline void RDORuntime::setPatternParameter(ruint paramID, CREF(RDOValue) paramValue)
{
	if (m_patternParameterList.size() <= paramID)
	{
		m_patternParameterList.resize(paramID + 1);
	}
	m_patternParameterList[paramID] = paramValue;
}

inline RDOValue RDORuntime::getPatternParameter(ruint paramID) const
{
	ASSERT(paramID < m_patternParameterList.size());
	return m_patternParameterList[paramID];
}

inline void RDORuntime::onNothingMoreToDo()
{
	m_whyStop = rdo::service::simulation::EC_NoMoreEvents;
}

inline void RDORuntime::onEndCondition()
{
	m_whyStop = rdo::service::simulation::EC_OK;
}

inline void RDORuntime::onRuntimeError()
{
	m_whyStop = rdo::service::simulation::EC_RunTimeError;
}

inline void RDORuntime::onUserBreak()
{
	m_whyStop = rdo::service::simulation::EC_UserBreak;
}

inline RDORuntime::ResCIterator RDORuntime::res_begin() const
{
	return m_resourceListByTime.begin();
}

inline RDORuntime::ResCIterator RDORuntime::res_end() const
{
	return m_resourceListByTime.end();
}

inline CREF(LPIThreadProxy) RDORuntime::getThreadProxy() const
{
	return m_pThreadProxy;
}

inline RDORuntime::ResList RDORuntime::getResourcesBeforeSim() const
{
	ResList list;
	ResCIterator it = m_resourceListByTime.begin();
	while (it != m_resourceListByTime.end())
	{
		list.push_back(*it);
		++it;
	}
	return list;
}

inline void RDORuntime::preProcess()
{
	parent_type::preProcess();
	m_physicTime = time(NULL);
}

// --------------------------------------------------------------------------------
// -------------------- RDORuntime::BreakPoint
// --------------------------------------------------------------------------------
inline RDORuntime::BreakPoint::BreakPoint(CREF(tstring) name, CREF(LPRDOCalc) pCalc)
	: m_name (name )
	, m_pCalc(pCalc)
{}

inline CREF(tstring) RDORuntime::BreakPoint::getName() const
{
	return m_name;
}

inline CREF(LPRDOCalc) RDORuntime::BreakPoint::getCalc() const
{
	return m_pCalc;
}

// --------------------------------------------------------------------------------
// -------------------- RDORuntime::Stack
// --------------------------------------------------------------------------------
inline RDORuntime::Stack::Stack()
{
	m_container.reserve(1000);
}

inline void RDORuntime::Stack::push(CREF(RDOValue) value)
{
	m_container.push_back(value);
}

inline void RDORuntime::Stack::push(CREF(RDOValue) value, ruint removeCount)
{
	m_container.resize(m_container.size() - removeCount);
	push(value);
}

inline RDOValue RDORuntime::Stack::pop()
{
	ASSERT(!m_container.empty());
	RDOValue result = m_container.back();
	m_container.pop_back();
	return result;
}

inline CREF(RDOValue) RDORuntime::Stack::top(ruint index) const
{
	return m_container[m_container.size() - 1 - index];
}

CLOSE_RDO_RUNTIME_NAMESPACE
