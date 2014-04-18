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
#include "simulator/runtime/namespace.h"
#include "simulator/runtime/rdo_enum.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOResults
// --------------------------------------------------------------------------------
inline RDOResults::RDOResults()
{}

inline RDOResults::~RDOResults()
{}

inline void RDOResults::width(std::size_t w)
{
	getOStream().width(w);
}

template<class T> 
inline RDOResults& RDOResults::operator<< (const T& value)
{
	getOStream() << value;
	return *this;
}

// --------------------------------------------------------------------------------
// -------------------- RDORuntime
// --------------------------------------------------------------------------------
inline Notify& RDORuntime::notify()
{
	return m_notify;
}

inline Error& RDORuntime::error()
{
	ASSERT(m_pError);
	return *m_pError;
}

inline RDOHotKey& RDORuntime::hotkey()
{
	return m_hotKey;
}

inline RDOResults& RDORuntime::getResults()
{
	return *m_resultList;
}

inline RDOResults& RDORuntime::getResultsInfo()
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

inline std::size_t RDORuntime::getCurrentTerm() const
{
	return m_currentTerm;
}

inline void RDORuntime::setCurrentTerm(std::size_t value)
{
	m_currentTerm = value;
}

inline LPIActivity& RDORuntime::getCurrentActivity()
{
	return m_currActivity;
}

inline void RDORuntime::setCurrentActivity(const LPIActivity& activity)
{
	m_currActivity = activity;
}

inline const RDORuntime::LPIResultList& RDORuntime::getResult() const
{
	return m_resultAllList;
}

inline void RDORuntime::addInitCalc(const LPRDOCalc& initCalc)
{
	m_initCalcList.push_back(initCalc);
}

inline RDOValue& RDORuntime::getResParamValRaw(std::size_t resID, std::size_t paramID)
{
	LPRDOResource pResource = getResourceByID(resID);
	ASSERT(pResource);
	return pResource->getParamRaw(paramID);
}

inline void RDORuntime::setResParamVal(std::size_t resID, std::size_t paramID, const RDOValue& value)
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

inline void RDORuntime::pushGroupFunc(const LPRDOResource& pResource)
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

inline LPRDOResource RDORuntime::getResourceByID(std::size_t resourceID) const
{
	return resourceID != std::size_t(~0) && resourceID < m_resourceListByID.size()
		? m_resourceListByID[resourceID]
		: LPRDOResource(NULL);
}

inline void RDORuntime::setPatternParameter(std::size_t paramID, const RDOValue& paramValue)
{
	if (m_patternParameterList.size() <= paramID)
	{
		m_patternParameterList.resize(paramID + 1);
	}
	m_patternParameterList[paramID] = paramValue;
}

inline RDOValue RDORuntime::getPatternParameter(std::size_t paramID) const
{
	ASSERT(paramID < m_patternParameterList.size());
	return m_patternParameterList[paramID];
}

inline void RDORuntime::onNothingMoreToDo()
{
	m_whyStop = rdo::simulation::report::EC_NoMoreEvents;
}

inline void RDORuntime::onEndCondition()
{
	m_whyStop = rdo::simulation::report::EC_OK;
}

inline void RDORuntime::onRuntimeError()
{
	m_whyStop = rdo::simulation::report::EC_RunTimeError;
}

inline void RDORuntime::onUserBreak()
{
	m_whyStop = rdo::simulation::report::EC_UserBreak;
}

inline void RDORuntime::addResType(const LPRDOResourceTypeList& pResType)
{
	ASSERT(pResType);
	ASSERT(m_resourceTypeList.size() == pResType->getTraceID() - 1);
	m_resourceTypeList.push_back(pResType);
}

inline const LPRDOResourceTypeList& RDORuntime::getResType(std::size_t number) const
{
	ASSERT(number > 0);
	ASSERT(number - 1 < m_resourceTypeList.size());
	return m_resourceTypeList[number - 1];
}

inline const LPIThreadProxy& RDORuntime::getThreadProxy() const
{
	return m_pThreadProxy;
}

inline RDORuntime::ResList RDORuntime::getResourcesBeforeSim() const
{
	ResList list;
	for (std::size_t i = 0; i < m_resourceTypeList.size(); i++)
	{
		ResCIterator it, end;
		it  = m_resourceTypeList[i]->res_begin();
		end = m_resourceTypeList[i]->res_end  ();
		while (it != end)
		{
			list.push_back(*it);
			++it;
		}
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
inline RDORuntime::BreakPoint::BreakPoint(const std::string& name, const LPRDOCalc& pCalc)
	: m_name (name )
	, m_pCalc(pCalc)
{}

inline const std::string& RDORuntime::BreakPoint::getName() const
{
	return m_name;
}

inline const LPRDOCalc& RDORuntime::BreakPoint::getCalc() const
{
	return m_pCalc;
}

CLOSE_RDO_RUNTIME_NAMESPACE
