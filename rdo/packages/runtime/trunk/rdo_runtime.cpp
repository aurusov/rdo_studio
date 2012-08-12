/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_runtime.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     RDORuntime implementation
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#ifdef COMPILER_GCC
	#include <float.h>
#endif // COMPILER_GCC
#include <iomanip>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch/stdpch.h"
#include "simulator/runtime/keyboard.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/result/result.h"
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/rdodptrtime.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
#include "utils/rdodebug.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORuntime
// --------------------------------------------------------------------------------
RDORuntime::RDORuntime()
	: RDOSimulatorTrace      (                   )
	, m_currActivity         (NULL               )
	, m_resultList           (NULL               )
	, m_resultListInfo       (NULL               )
	, m_pLastActiveBreakPoint(NULL               )
	, m_whyStop              (rdo::service::simulation::EC_OK)
	, m_currentTerm          (0                  )
	, m_funBreakFlag         (FBF_NONE           )
	, m_pStudioThread        (NULL               )
	, m_currFuncTop          (0                  )
{
	m_pTerminateIfCalc = NULL;
	m_pMemoryStack = rdo::Factory<RDOMemoryStack>::create();
	ASSERT(m_pMemoryStack);
}

RDORuntime::~RDORuntime()
{}

void RDORuntime::init()
{
	memory_insert(sizeof(RDORuntime));
}

void RDORuntime::deinit()
{
	onDestroy();
}

void RDORuntime::setStudioThread(PTR(RDOThread) pStudioThread)
{
	m_pStudioThread = pStudioThread;
}

rbool RDORuntime::endCondition()
{
	if (!m_pTerminateIfCalc)
	{
		return false; // forever
	}
	return fabs(m_pTerminateIfCalc->calcValue(this).getDouble()) > DBL_EPSILON;
}

void RDORuntime::setTerminateIf(CREF(LPRDOCalc) pTerminateIfCalc)
{
	ASSERT(pTerminateIfCalc);
	m_pTerminateIfCalc = pTerminateIfCalc;
}

rbool RDORuntime::breakPoints()
{
	STL_FOR_ALL_CONST(m_breakPointList, it)
	{
		if ((*it)->getCalc()->calcValue(this).getAsBool())
		{
			m_pLastActiveBreakPoint = *it;
			return true;
		}
	}
	return false;
}

void RDORuntime::insertBreakPoint(CREF(tstring) name, CREF(LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_breakPointList.push_back(rdo::Factory<BreakPoint>::create(name, pCalc));
}

LPRDOCalc RDORuntime::findBreakPoint(CREF(tstring) name)
{
	STL_FOR_ALL_CONST(m_breakPointList, it)
	{
		if ((*it)->getName() == name)
		{
			return (*it)->getCalc();
		}
	}
	return NULL;
}

tstring RDORuntime::getLastBreakPointName() const
{
	return m_pLastActiveBreakPoint ? m_pLastActiveBreakPoint->getName() + _T(": ") + m_pLastActiveBreakPoint->getCalc()->srcInfo().src_text() : _T("");
}

void RDORuntime::setConstValue(ruint constID, CREF(RDOValue) constValue)
{
	if (m_constantList.size() <= constID)
	{
		m_constantList.resize(constID + 1);
	}
	m_constantList[constID] = constValue;
}

RDOValue RDORuntime::getConstValue(ruint constID) const
{
	ASSERT(constID < m_constantList.size());
	return m_constantList[constID];
}

#ifdef _DEBUG
rbool RDORuntime::checkState()
{
	if (m_state.empty())
	{
		ValueList res;
		res.push_back(1);
		res.push_back(4);
		m_state.push_back(res);
		res[0] = 2;
		res[1] = 1;
		m_state.push_back(res);
		res[0] = 3;
		res[1] = 3;
		m_state.push_back(res);
		res[0] = 4;
		res[1] = 2;
		m_state.push_back(res);
		res[0] = 5;
		res[1] = 6;
		m_state.push_back(res);
		res.clear();
		res.push_back(5);
		m_state.push_back(res);
	}
	if (m_state.size() != m_resourceListByID.size()) return false;
	for (ruint i = 0; i < m_state.size(); ++i)
	{
		if (m_state[i].size() != m_resourceListByID[i]->paramsCount()) return false;
		for (ruint j = 0; j < m_resourceListByID[i]->paramsCount(); ++j)
		{
			if (m_state[i][j] != m_resourceListByID[i]->getParam(j)) return false;
		}
	}
	return true;
}

void RDORuntime::showResources(int node) const
{
	TRACE1(_T("------------- %d:\n"), node);
	int index = 0;
	ResourceListByID::const_iterator it = m_resourceListByID.begin();
	while (it != m_resourceListByID.end())
	{
		if (*it)
		{
			TRACE1(_T("%d. "), index);
			for (ruint i = 0; i < (*it)->paramsCount(); ++i)
			{
				TRACE1(_T("%s "), (*it)->getParam(i).getAsString().c_str());
			}
			TRACE(_T("\n"));
		}
		else
		{
			TRACE1(_T("%d. NULL\n"), index);
		}
		++index;
		++it;
	}
}
#endif

void RDORuntime::onEraseRes(ruint resourceID, CREF(LPRDOEraseResRelCalc) pCalc)
{
	LPRDOResource res = m_resourceListByID.at(resourceID);
	if (!res)
	{
		error().push(rdo::format(_T("Временный ресурс уже удален. Возможно, он удален ранее в этом же образце. Имя релевантного ресурса: %s"), pCalc ? pCalc->getName().c_str() : "неизвестное имя"), pCalc->srcInfo());
	}
	if (!res->canFree())
	{
		error().push(_T("Невозможно удалить ресурс, т.к. он еще используется"), pCalc->srcInfo());
	}
	else
	{
		LPIResultWatchValueList::iterator it = m_resultWatchValueList.begin();
		while (it != m_resultWatchValueList.end())
		{
			(*it)->checkResourceErased(res);
			++it;
		}
		m_resourceListByID.at(resourceID) = NULL;
		// Диструктор ресурса вызывается в std::list::erase, который вызывается из std::list::remove
		m_resourceListByTime.remove(res);
		notify().fireMessage(Notify::RO_BEFOREDELETE, (void*)res->getTraceID());
		onResourceErase(res);
	}
}

void RDORuntime::insertNewResource(CREF(LPRDOResource) pResource)
{
	ASSERT(pResource);
	if (pResource->getTraceID() >= m_resourceListByID.size())
	{
		m_resourceListByID.resize(pResource->getTraceID() + 1, NULL);
		m_resourceListByID.at(pResource->getTraceID()) = pResource;
	}
	else
	{
		if (m_resourceListByID.at(pResource->getTraceID()) == LPRDOResource(NULL))
		{
			m_resourceListByID.at(pResource->getTraceID()) = pResource;
		}
		else
		{
			error().push(rdo::service::simulation::RDOSyntaxMessage(
				_T("Внутренняя ошибка: insertNewResource"),
				rdoModelObjects::PAT,
				0,
				0
			));
		}
	}
	m_resourceListByTime.push_back(pResource);
}

void RDORuntime::addRuntimeEvent(LPIBaseOperationContainer pLogic, CREF(LPIEvent) pEvent)
{
	ASSERT(pLogic);
	ASSERT(pEvent);
	appendBaseOperation(pLogic, pEvent);
}

void RDORuntime::addRuntimeRule(LPIBaseOperationContainer pLogic, CREF(LPIRule) pRule)
{
	ASSERT(pLogic);
	ASSERT(pRule );
	appendBaseOperation(pLogic, pRule);
}

void RDORuntime::addRuntimeOperation(LPIBaseOperationContainer pLogic, CREF(LPIOperation) pOperation)
{
	ASSERT(pLogic    );
	ASSERT(pOperation);
	appendBaseOperation(pLogic, pOperation);
}

void RDORuntime::addRuntimeResult(CREF(LPIResult) pResult)
{
	m_resultAllList.push_back(pResult);
	LPIResultTrace resultTrace = pResult;
	LPITrace       trace       = pResult;
	if (resultTrace && trace && trace->traceable())
	{
		m_resultTraceList.push_back(resultTrace);
	}
	if (pResult.query_cast<IResultWatchValue>())
	{
		m_resultWatchValueList.push_back(pResult);
	}
}

void RDORuntime::addRuntimeFrame(CREF(LPRDOFRMFrame) pFrame)
{ 
	m_frameList.push_back(pFrame);
}

LPRDOFRMFrame RDORuntime::lastFrame() const
{
	return !m_frameList.empty() ? m_frameList.front() : LPRDOFRMFrame(NULL);
}

rbool RDORuntime::isKeyDown() const
{
	return m_hotKey.isKeyDown();
}

void RDORuntime::rdoInit(RDOTrace* tracer, RDOResults* customResults, RDOResults* customResultsInfo, CREF(LPIThreadProxy) pThreadProxy)
{
	ASSERT(pThreadProxy);

	m_tracer         = tracer;
	m_resultList     = customResults;
	m_resultListInfo = customResultsInfo;
	m_currFuncTop    = 0;
	m_pThreadProxy   = pThreadProxy;
	parent_type::rdoInit();
}

void RDORuntime::onInit()
{
	STL_FOR_ALL(m_initCalcList, calcIt)
		(*calcIt)->calcValue(this);
}

void RDORuntime::onDestroy()
{
	/// @todo Дима, почему у первого ресурса счетчик на 1 больше, чем у других ?
	m_resourceListByTime.clear();
	m_resourceListByID.clear();

	if (m_resultList)
	{
		delete m_resultList;
		m_resultList = NULL;
	}
	if (m_resultListInfo)
	{
		delete m_resultListInfo;
		m_resultListInfo = NULL;
	}
}

RDOValue RDORuntime::getFuncArgument(ruint paramID) const
{
	ASSERT(m_currFuncTop + paramID < m_funcStack.size());
	return m_funcStack[m_currFuncTop + paramID];
}

LPRDORuntime RDORuntime::clone() const
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	ASSERT(pRuntime);
	pRuntime->m_sizeofSim = sizeof(RDORuntime);

	LPRDORuntime pThis(const_cast<PTR(RDORuntime)>(this));
	pRuntime->copyFrom(pThis);

	return pRuntime;
}

void RDORuntime::copyFrom(CREF(LPRDORuntime) pOther)
{
	ASSERT(pOther);

	ruint size = pOther->m_resourceListByID.size();
	for (ruint i = 0; i < size; ++i)
	{
		if (pOther->m_resourceListByID.at(i) == LPRDOResource(NULL))
		{
			m_resourceListByID.push_back(NULL);
		}
		else
		{
			// вставка ресурса в контейнер m_resourceListByID нового RDORuntime произойдет в его конструкторе
			pOther->m_resourceListByID.at(i)->clone(this);
			m_sizeofSim += sizeof(RDOResource) + sizeof(void*) * 2;
		}
	}
	m_constantList      = pOther->m_constantList;
	m_patternParameterList = pOther->m_patternParameterList;
	m_resultList           = pOther->m_resultList;
	m_pThreadProxy    = pOther->m_pThreadProxy;
	setCurrentTime(pOther->getCurrentTime());

	parent_type::copyFrom(pOther.object_parent_cast<parent_type>());
}

rbool RDORuntime::equal(CREF(LPRDORuntime) pOther) const
{
	if (pOther->m_resourceListByID.size() != m_resourceListByID.size()) return false;

	ruint size = m_resourceListByID.size();
	for (ruint i = 0; i < size; ++i)
	{
		if (m_resourceListByID.at(i) == LPRDOResource(NULL) && pOther->m_resourceListByID.at(i) != LPRDOResource(NULL)) return false;
		if (m_resourceListByID.at(i) != LPRDOResource(NULL) && pOther->m_resourceListByID.at(i) == LPRDOResource(NULL)) return false;
		if (m_resourceListByID.at(i) == LPRDOResource(NULL) && pOther->m_resourceListByID.at(i) == LPRDOResource(NULL)) continue;
		if (pOther->m_resourceListByID.at(i) != m_resourceListByID.at(i)) return false;
	}
	return true;
}

void RDORuntime::onResetResult()
{
	LPIResultList::iterator it = m_resultAllList.begin();
	while (it != m_resultAllList.end())
	{
		(*it)->resetResult(this);
		++it;
	}
}

void RDORuntime::onCheckResult()
{
	LPIResultList::iterator it = m_resultAllList.begin();
	while (it != m_resultAllList.end())
	{
		(*it)->checkResult(this);
		++it;
	}
}

void RDORuntime::onAfterCheckResult()
{
	LPIResultTraceList::iterator it = m_resultTraceList.begin();
	while (it != m_resultTraceList.end())
	{
		(*it)->traceResult();
		++it;
	}
}

void RDORuntime::onPutToTreeNode()
{
	// when create TreeNode with new RDOSimulator,
	// make all resources permanent, to avoid trace their
	// erase when delete TreeNode
	for (ruint i = 0; i < m_resourceListByID.size(); ++i)
	{
		if (m_resourceListByID[i])
		{
			m_resourceListByID[i]->makeTemporary(false);
		}
	}
}

void RDORuntime::writeExitCode()
{
	tstring status;
	switch (m_whyStop)
	{
	case rdo::service::simulation::EC_OK          : status = _T("NORMAL_TERMINATION"); break;
	case rdo::service::simulation::EC_NoMoreEvents: status = _T("NO_MORE_EVENTS");     break;
	case rdo::service::simulation::EC_RunTimeError: status = _T("RUN_TIME_ERROR");     break;
	case rdo::service::simulation::EC_UserBreak   : status = _T("USER_BREAK");         break;
	default: NEVER_REACH_HERE;
	}

	getTracer()->writeStatus(this, status);
}

void RDORuntime::postProcess()
{
	getTracer()->startWriting();
	LPIResultList::iterator it = m_resultAllList.begin();
	while (it != m_resultAllList.end())
	{
		try
		{
			(*it)->calcStat(this, getResults().getOStream());
		}
		catch (REF(RDORuntimeException))
		{}
		++it;
	}

	try
	{
		parent_type::postProcess();
		writeExitCode();
		getTracer()->stopWriting();
	}
	catch (REF(RDORuntimeException) e)
	{
		writeExitCode();
		getTracer()->stopWriting();
		throw e;
	}
}

LPRDOMemoryStack RDORuntime::getMemoryStack()
{
	return m_pMemoryStack;
}

void RDORuntime::setFunBreakFlag(CREF(FunBreakFlag) flag)
{
	m_funBreakFlag = flag;
}

CREF(RDORuntime::FunBreakFlag) RDORuntime::getFunBreakFlag() const
{
	return m_funBreakFlag;
}

CLOSE_RDO_RUNTIME_NAMESPACE
