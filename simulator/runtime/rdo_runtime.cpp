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
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#ifdef COMPILER_GCC
	#include <float.h>
#endif // COMPILER_GCC
#include <iomanip>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"
#include "utils/src/animation/rdoanimation.h"
#include "simulator/runtime/pch/stdpch.h"
#include "simulator/runtime/keyboard.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdo_exception.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/result/result.h"
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/rdodptrtime.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
// --------------------------------------------------------------------------------

typedef rdo::simulation::report::FileMessage RDOSyntaxMessage;

#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORuntime
// --------------------------------------------------------------------------------
RDORuntime::RDORuntime(Error* pError)
	: RDOSimulatorTrace()
	, m_whyStop        (rdo::simulation::report::EC_OK)
	, m_funBreakFlag   (FBF_NONE)
	, m_pStudioThread  (NULL    )
	, m_pError         (pError  )
	, m_currFuncTop    (0       )
	, m_resultList     (NULL    )
	, m_resultListInfo (NULL    )
	, m_currentTerm    (0       )
	, m_pPreparingFrame(NULL    )
{
	ASSERT(m_pError);
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

void RDORuntime::setStudioThread(RDOThread* pStudioThread)
{
	m_pStudioThread = pStudioThread;
}

bool RDORuntime::endCondition()
{
	if (!m_pTerminateIfCalc)
	{
		return false; // forever
	}
	return fabs(m_pTerminateIfCalc->calcValue(this).getDouble()) > DBL_EPSILON;
}

void RDORuntime::setTerminateIf(const LPRDOCalc& pTerminateIfCalc)
{
	ASSERT(pTerminateIfCalc);
	m_pTerminateIfCalc = pTerminateIfCalc;
}

bool RDORuntime::breakPoints()
{
	for (const auto& breakPoint: m_breakPointList)
	{
		if (breakPoint->getCalc()->calcValue(this).getAsBool())
		{
			m_pLastActiveBreakPoint = breakPoint;
			return true;
		}
	}
	return false;
}

void RDORuntime::insertBreakPoint(const std::string& name, const LPRDOCalc& pCalc)
{
	ASSERT(pCalc);
	m_breakPointList.push_back(rdo::Factory<BreakPoint>::create(name, pCalc));
}

LPRDOCalc RDORuntime::findBreakPoint(const std::string& name)
{
	for (const auto& breakPoint: m_breakPointList)
	{
		if (breakPoint->getName() == name)
		{
			return breakPoint->getCalc();
		}
	}
	return NULL;
}

std::string RDORuntime::getLastBreakPointName() const
{
	return m_pLastActiveBreakPoint ? m_pLastActiveBreakPoint->getName() + ": " + m_pLastActiveBreakPoint->getCalc()->srcInfo().src_text() : "";
}

void RDORuntime::setConstValue(std::size_t constID, const RDOValue& constValue)
{
	if (m_constantList.size() <= constID)
	{
		m_constantList.resize(constID + 1);
	}
	m_constantList[constID] = constValue;
}

RDOValue RDORuntime::getConstValue(std::size_t constID) const
{
	ASSERT(constID < m_constantList.size());
	return m_constantList[constID];
}

#ifdef _DEBUG
bool RDORuntime::checkState()
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
	for (std::size_t i = 0; i < m_state.size(); ++i)
	{
		if (m_state[i].size() != m_resourceListByID[i]->paramsCount()) return false;
		for (std::size_t j = 0; j < m_resourceListByID[i]->paramsCount(); ++j)
		{
			if (m_state[i][j] != m_resourceListByID[i]->getParam(j)) return false;
		}
	}
	return true;
}

void RDORuntime::showResources(int node) const
{
	TRACE1("------------- %d:\n", node);
	int index = 0;
	ResourceListByID::const_iterator it = m_resourceListByID.begin();
	while (it != m_resourceListByID.end())
	{
		if (*it)
		{
			TRACE1("%d. ", index);
			for (std::size_t i = 0; i < (*it)->paramsCount(); ++i)
			{
				TRACE1("%s ", (*it)->getParam(i).getAsString().c_str());
			}
			TRACE("\n");
		}
		else
		{
			TRACE1("%d. NULL\n", index);
		}
		++index;
		++it;
	}
}
#endif

void RDORuntime::onEraseRes(std::size_t resourceID, const LPRDOEraseResRelCalc& pCalc)
{
	LPRDOResource res = m_resourceListByID.at(resourceID);
	if (!res)
	{
		error().push(rdo::format("Временный ресурс уже удален. Возможно, он удален ранее в этом же образце. Имя релевантного ресурса: %s", pCalc ? pCalc->getName().c_str() : "неизвестное имя"), pCalc->srcInfo());
	}
	if (!res->canFree())
	{
		error().push("Невозможно удалить ресурс, т.к. он еще используется", pCalc->srcInfo());
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

		LPIResourceType type(res->getResType());
		// Деструктор ресурса вызывается в std::list::erase, который вызывается из std::list::remove
		type->eraseRes(res);
		notify().fireMessage(Notify::RO_BEFOREDELETE, reinterpret_cast<void*>(res->getTraceID()));
		onResourceErase(res);
	}
}

void RDORuntime::insertNewResource(const LPRDOResource& pResource)
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
			error().push(RDOSyntaxMessage(
				"Внутренняя ошибка: insertNewResource",
				rdoModelObjects::PAT,
				0,
				0
			));
		}
	}
#ifdef RDO_LIMIT_RES
	if (m_resourceListByID.size() >= 200)
	{
		error().push(RDOSyntaxMessage(
			"Сработало лицензионное ограничение на количество ресурсов. Обратитесь за приобритением полной версии",
			rdoModelObjects::PAT,
			0,
			0
		));
	}
#endif
}

void RDORuntime::addRuntimeEvent(LPIBaseOperationContainer pLogic, const LPIEvent& pEvent)
{
	ASSERT(pLogic);
	ASSERT(pEvent);
	appendBaseOperation(pLogic, pEvent);
}

void RDORuntime::addRuntimeRule(LPIBaseOperationContainer pLogic, const LPIRule& pRule)
{
	ASSERT(pLogic);
	ASSERT(pRule );
	appendBaseOperation(pLogic, pRule);
}

void RDORuntime::addRuntimeOperation(LPIBaseOperationContainer pLogic, const LPIOperation& pOperation)
{
	ASSERT(pLogic    );
	ASSERT(pOperation);
	appendBaseOperation(pLogic, pOperation);
}

void RDORuntime::addRuntimeResult(const LPIResult& pResult)
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

void RDORuntime::addRuntimeFrame(const LPRDOFRMFrame& pFrame)
{ 
	m_frameList.push_back(pFrame);
}

LPRDOFRMFrame RDORuntime::lastFrame() const
{
	return !m_frameList.empty() ? m_frameList.front() : LPRDOFRMFrame(NULL);
}

bool RDORuntime::isKeyDown() const
{
	return m_hotKey.isKeyDown();
}

void RDORuntime::rdoInit(RDOTrace* tracer, RDOResults* customResults, RDOResults* customResultsInfo, const LPIThreadProxy& pThreadProxy)
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
	for (const auto& calc: m_initCalcList)
		calc->calcValue(this);
}

void RDORuntime::onDestroy()
{
	/// @todo Дима, почему у первого ресурса счетчик на 1 больше, чем у других ?
	m_resourceListByID.clear();
	m_resourceTypeList.clear();

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

RDOValue RDORuntime::getFuncArgument(std::size_t paramID) const
{
	ASSERT(m_currFuncTop + paramID < m_funcStack.size());
	return m_funcStack[m_currFuncTop + paramID];
}

LPRDORuntime RDORuntime::clone() const
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create(m_pError);
	ASSERT(pRuntime);
	pRuntime->m_sizeofSim = sizeof(RDORuntime);

	LPRDORuntime pThis(const_cast<RDORuntime*>(this));
	pRuntime->copyFrom(pThis);

	return pRuntime;
}

void RDORuntime::copyFrom(const LPRDORuntime& pOther)
{
	ASSERT(pOther);
	ASSERT(m_resourceTypeList.empty());

	LPRDORuntime pThis(const_cast<RDORuntime*>(this));

	BOOST_FOREACH(const LPRDOResourceTypeList& pRTP, pOther->m_resourceTypeList)
	{
		pRTP->clone(pThis);
	}

	m_constantList         = pOther->m_constantList;
	m_patternParameterList = pOther->m_patternParameterList;
	m_resultList           = pOther->m_resultList;
	m_pThreadProxy         = pOther->m_pThreadProxy;
	setCurrentTime(pOther->getCurrentTime());

	parent_type::copyFrom(pOther.object_parent_cast<parent_type>());
}

bool RDORuntime::equal(const LPRDORuntime& pOther) const
{
	if (pOther->m_resourceListByID.size() != m_resourceListByID.size()) return false;

	const std::size_t size = m_resourceListByID.size();
	for (std::size_t i = 0; i < size; ++i)
	{
		if (m_resourceListByID.at(i) == LPRDOResource(NULL) && pOther->m_resourceListByID.at(i) != LPRDOResource(NULL)) return false;
		if (m_resourceListByID.at(i) != LPRDOResource(NULL) && pOther->m_resourceListByID.at(i) == LPRDOResource(NULL)) return false;
		if (m_resourceListByID.at(i) == LPRDOResource(NULL) && pOther->m_resourceListByID.at(i) == LPRDOResource(NULL)) continue;
		if (*pOther->m_resourceListByID.at(i).get() != *m_resourceListByID.at(i).get()) return false;
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
	for (std::size_t i = 0; i < m_resourceListByID.size(); ++i)
	{
		if (m_resourceListByID[i])
		{
			m_resourceListByID[i]->makeTemporary(false);
		}
	}
}

void RDORuntime::writeExitCode()
{
	std::string status;
	switch (m_whyStop)
	{
	case rdo::simulation::report::EC_OK           : status = "NORMAL_TERMINATION"; break;
	case rdo::simulation::report::EC_NoMoreEvents : status = "NO_MORE_EVENTS";     break;
	case rdo::simulation::report::EC_RunTimeError : status = "RUN_TIME_ERROR";     break;
	case rdo::simulation::report::EC_UserBreak    : status = "USER_BREAK";         break;
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
		catch (const RDORuntimeException&)
		{}
		++it;
	}

	try
	{
		parent_type::postProcess();
		writeExitCode();
		getTracer()->stopWriting();
	}
	catch (const RDORuntimeException& e)
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

void RDORuntime::setFunBreakFlag(const FunBreakFlag& flag)
{
	m_funBreakFlag = flag;
}

const RDORuntime::FunBreakFlag& RDORuntime::getFunBreakFlag() const
{
	return m_funBreakFlag;
}

rdo::animation::Frame* RDORuntime::getPreparingFrame() const
{
	return m_pPreparingFrame;
}

void RDORuntime::setPreparingFrame(rdo::animation::Frame* pPreparingFrame)
{
	ASSERT(pPreparingFrame);
	ASSERT(!m_pPreparingFrame);
	m_pPreparingFrame = pPreparingFrame;
}

void RDORuntime::resetPreparingFrame()
{
	ASSERT(m_pPreparingFrame);
	m_pPreparingFrame = NULL;
}

CLOSE_RDO_RUNTIME_NAMESPACE
