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
#include "utils/namespace.h"
#include "utils/rdomacros.h"
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
inline REF(RDOResults) RDORuntime::getResults()
{
	return *results;
}

inline REF(RDOResults) RDORuntime::getResultsInfo()
{
	return *results_info;
}

inline double RDORuntime::getTimeNow()
{
	return getCurrentTime();
}

inline double RDORuntime::getSeconds()
{
	/// @todo использовать явный cast()
	return (double)(time(NULL) - physic_time);
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

inline CREF(RDORuntime::LPIPokazList) RDORuntime::getPokaz() const
{
	return m_pokazAllList;
}

inline void RDORuntime::addInitCalc(CREF(LPRDOCalc) initCalc)
{
	initCalcs.push_back(initCalc);
}

inline RDOValue RDORuntime::getResParamVal(ruint resID, ruint paramID) const
{
	LPRDOResource pResource = getResourceByID(resID);
	ASSERT(pResource);
	return pResource->getParam(paramID);
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

inline LPRDOResource RDORuntime::getGroupFuncRes()
{
	return groupFuncStack.back();
}

inline void RDORuntime::pushFuncArgument(RDOValue arg)
{
	funcStack.push_back(arg);
}

inline void RDORuntime::pushGroupFunc(CREF(LPRDOResource) pResource)
{
	groupFuncStack.push_back(pResource);
}

inline void RDORuntime::popFuncArgument()
{
	funcStack.pop_back();
}

inline void RDORuntime::popGroupFunc()
{
	groupFuncStack.pop_back();
}

inline void RDORuntime::pushFuncTop()
{
	funcStack.push_back(RDOValue(currFuncTop));
}

inline void RDORuntime::resetFuncTop(int numArg)
{
	currFuncTop = funcStack.size() - numArg;
}

inline void RDORuntime::popFuncTop()
{
	currFuncTop = funcStack.back().getInt();
	funcStack.pop_back();
}

inline LPRDOResource RDORuntime::getResourceByID(const int num) const
{
	return num >= 0 ? allResourcesByID.at(num) : LPRDOResource(NULL);
}

inline void RDORuntime::setPatternParameter(unsigned int parNumb, RDOValue val)
{
	if (patternParameters.size() <= parNumb)
	{
		patternParameters.resize(parNumb + 1);
	}
	patternParameters.at(parNumb) = val;
}

inline RDOValue RDORuntime::getPatternParameter(int parNumb) 
{
	ASSERT((ruint)parNumb < patternParameters.size());
	return patternParameters[parNumb];
}

inline void RDORuntime::onNothingMoreToDo()
{
	whyStop = rdoSimulator::EC_NoMoreEvents;
}

inline void RDORuntime::onEndCondition()
{
	whyStop = rdoSimulator::EC_OK;
}

inline void RDORuntime::onRuntimeError()
{
	whyStop = rdoSimulator::EC_RunTimeError;
}

inline void RDORuntime::onUserBreak()
{
	whyStop = rdoSimulator::EC_UserBreak;
}

inline RDORuntime::ResCIterator RDORuntime::res_begin() const
{
	return allResourcesByTime.begin();
}

inline RDORuntime::ResCIterator RDORuntime::res_end() const
{
	return allResourcesByTime.end();
}

inline CREF(LPIThreadProxy) RDORuntime::getThreadProxy() const
{
	return m_pThreadProxy;
}

inline RDORuntime::ResList RDORuntime::getResourcesBeforeSim() const
{
	ResList list;
	ResCIterator it = allResourcesByTime.begin();
	while (it != allResourcesByTime.end())
	{
		list.push_back(*it);
		++it;
	}
	return list;
}

inline void RDORuntime::preProcess()
{
	Parent::preProcess();
	physic_time = time(NULL);
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

CLOSE_RDO_RUNTIME_NAMESPACE
