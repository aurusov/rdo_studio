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
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#ifdef COMPILER_GCC
	#include <float.h>
#endif // COMPILER_GCC
#include <iomanip>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch.h"
#include "simulator/runtime/keyboard.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdoprocess.h"
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
	, m_whyStop              (rdoSimulator::EC_OK)
	, m_keyFound             (false              )
	, m_currentTerm          (0                  )
	, m_funBreakFlag         (FBF_CONTINUE       )
	, m_pStudioThread        (NULL               )
{
	m_pTerminateIfCalc = NULL;
	m_pMemoryStack = rdo::Factory<RDOMemoryStack>::create();
	ASSERT(m_pMemoryStack);
}

RDORuntime::~RDORuntime()
{}

void RDORuntime::init()
{
	memory_insert(sizeof(rdoRuntime::RDORuntime));
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
	return m_pLastActiveBreakPoint ? m_pLastActiveBreakPoint->getName() + _T(": ") + m_pLastActiveBreakPoint->getCalc()->src_text() : _T("");
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
		error(rdo::format("Временный ресурс уже удален. Возможно, он удален ранее в этом же образце. Имя релевантного ресурса: %s", pCalc ? pCalc->getName().c_str() : "неизвестное имя"), pCalc);
	}
	if (!res->canFree())
	{
		error("Невозможно удалить ресурс, т.к. он еще используется", pCalc);
//		error("Try to erase used resource", fromCalc);
	}
	else
	{
		LPIPokazWatchValueList::iterator it = m_pokazWatchValueList.begin();
		while (it != m_pokazWatchValueList.end())
		{
			(*it)->checkResourceErased(res);
			++it;
		}
		m_resourceListByID.at(resourceID) = NULL;
		// Диструктор ресурса вызывается в std::list::erase, который вызывается из std::list::remove
		m_resourceListByTime.remove(res);
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
			error("Внутренняя ошибка: insertNewResource");
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

void RDORuntime::addRuntimePokaz(CREF(LPIPokaz) pPokaz)
{
	m_pokazAllList.push_back(pPokaz);
	LPIPokazTrace pokazTrace = pPokaz;
	LPITrace      trace      = pPokaz;
	if (pokazTrace && trace && trace->traceable())
	{
		m_pokazTraceList.push_back(pokazTrace);
	}
	if (pPokaz.query_cast<IPokazWatchValue>())
	{
		m_pokazWatchValueList.push_back(pPokaz);
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

rbool RDORuntime::keyDown(ruint scanCode)
{
	// Если нажаты VK_SHIFT или VK_CONTROL, то сбросим буфер клавиатуры
	if (scanCode == VK_SHIFT || scanCode == VK_CONTROL)
	{
		KeyList::iterator it = m_keysDown.begin();
		while (it != m_keysDown.end())
		{
			if (*it != VK_SHIFT && *it != VK_CONTROL)
			{
				it = m_keysDown.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	// Подсчитаем сколько раз клавиша уже в буфере
	int cnt = 0;
	KeyList::iterator it = m_keysDown.begin();
	while (it != m_keysDown.end())
	{
		if (*it == scanCode)
		{
			++cnt;
		}
		++it;
	}
	// Добавим клавишу в буфер
	if (cnt < 4)
	{
		m_keysDown.push_back(scanCode);
	}
	if (cnt == 0) m_keyFound = true;
	return cnt > 0;
}

void RDORuntime::keyUp(ruint scanCode)
{
	// Если отжаты VK_SHIFT или VK_CONTROL, то сбросим удалим их из буфера
	//if (scanCode == VK_SHIFT || scanCode == VK_CONTROL)
	//{
		KeyList::iterator it = m_keysDown.begin();
		while (it != m_keysDown.end())
		{
			if (*it == scanCode)
			{
				it = m_keysDown.erase(it);
			}
			else
			{
				++it;
			}
		}
	//}
}

rbool RDORuntime::checkKeyPressed(ruint scanCode, rbool shift, rbool control)
{
	if (scanCode == 0) return false;
	rbool shift_found   = false;
	rbool control_found = false;
	// Найдем VK_SHIFT и/или VK_CONTROL в буфере
	KeyList::iterator it = m_keysDown.begin();
	while (it != m_keysDown.end())
	{
		if (*it == VK_SHIFT)
		{
			shift_found = true;
			if (shift_found && control_found) break;
		}
		if (*it == VK_CONTROL)
		{
			control_found = true;
			if (shift_found && control_found) break;
		}
		++it;
	}
	// Теперь найдем саму клавишу в буфере
	// Удалим её из буфера перед выходом
	if (shift_found == shift && control_found == control)
	{
		KeyList::iterator it = m_keysDown.begin();
		while (it != m_keysDown.end())
		{
			if (*it == scanCode)
			{
				m_keysDown.erase(it);
				m_keyFound = true;
				return true;
			}
			++it;
		}
	}
	m_keyFound = false;
	return false;
}

rbool RDORuntime::checkAreaActivated(CREF(tstring) oprName)
{
	NameList::iterator it = std::find(m_activeAreasMouseClicked.begin(), m_activeAreasMouseClicked.end(), oprName);
	if (it == m_activeAreasMouseClicked.end())
	{
		return false;
	}
	m_activeAreasMouseClicked.erase(it);
	return true;
}

rbool RDORuntime::isKeyDown()
{
	return m_keyFound || !m_activeAreasMouseClicked.empty();
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

void RDORuntime::onResetPokaz()
{
	LPIPokazList::iterator it = m_pokazAllList.begin();
	while (it != m_pokazAllList.end())
	{
		(*it)->resetPokaz(this);
		++it;
	}
}

void RDORuntime::onCheckPokaz()
{
	LPIPokazList::iterator it = m_pokazAllList.begin();
	while (it != m_pokazAllList.end())
	{
		(*it)->checkPokaz(this);
		++it;
	}
}

void RDORuntime::onAfterCheckPokaz()
{
	LPIPokazTraceList::iterator it = m_pokazTraceList.begin();
	while (it != m_pokazTraceList.end())
	{
		(*it)->tracePokaz();
		++it;
	}
}

void RDORuntime::error(CREF(tstring) message, CREF(LPRDOCalc) pCalc)
{
	if (!message.empty())
	{
		m_errorList.push_back(rdoSimulator::RDOSyntaxError(rdoSimulator::RDOSyntaxError::UNKNOWN, rdo::format("Модельное время: %f. %s", getTimeNow(), message.c_str()), pCalc ? pCalc->src_pos().m_last_line : 0, pCalc ? pCalc->src_pos().m_last_pos : 0, pCalc ? pCalc->src_filetype() : rdoModelObjects::PAT));
	}
	throw RDORuntimeException("");
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
	case rdoSimulator::EC_OK          : status = _T("NORMAL_TERMINATION"); break;
	case rdoSimulator::EC_NoMoreEvents: status = _T("NO_MORE_EVENTS");     break;
	case rdoSimulator::EC_RunTimeError: status = _T("RUN_TIME_ERROR");     break;
	case rdoSimulator::EC_UserBreak   : status = _T("USER_BREAK");         break;
	default: NEVER_REACH_HERE;
	}

	getTracer()->writeStatus(this, status);
}

void RDORuntime::postProcess()
{
	getTracer()->startWriting();
	LPIPokazList::iterator it = m_pokazAllList.begin();
	while (it != m_pokazAllList.end())
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

RDORuntime::RDOHotKeyToolkit::RDOHotKeyToolkit()
{
	m_keys.insert(KeySet::value_type("ESCAPE", VK_ESCAPE));
	m_keys.insert(KeySet::value_type("TAB", VK_TAB));
	m_keys.insert(KeySet::value_type("SHIFT", VK_SHIFT));
	m_keys.insert(KeySet::value_type("CONTROL", VK_CONTROL));
	m_keys.insert(KeySet::value_type("BACK", VK_BACK));
	m_keys.insert(KeySet::value_type("RETURN", VK_RETURN));
	m_keys.insert(KeySet::value_type("INSERT", VK_INSERT));
	m_keys.insert(KeySet::value_type("HOME", VK_HOME));
	m_keys.insert(KeySet::value_type("PRIOR", VK_PRIOR));
	m_keys.insert(KeySet::value_type("DELETE", VK_DELETE));
	m_keys.insert(KeySet::value_type("END", VK_END));
	m_keys.insert(KeySet::value_type("NEXT", VK_NEXT));
	m_keys.insert(KeySet::value_type("UP", VK_UP));
	m_keys.insert(KeySet::value_type("LEFT", VK_LEFT));
	m_keys.insert(KeySet::value_type("DOWN", VK_DOWN));
	m_keys.insert(KeySet::value_type("RIGHT", VK_RIGHT));
	m_keys.insert(KeySet::value_type("DIVIDE", VK_DIVIDE));
	m_keys.insert(KeySet::value_type("MULTIPLY", VK_MULTIPLY));
	m_keys.insert(KeySet::value_type("SUBTRACT", VK_SUBTRACT));
	m_keys.insert(KeySet::value_type("ADD", VK_ADD));
	m_keys.insert(KeySet::value_type("CLEAR", VK_CLEAR));
	m_keys.insert(KeySet::value_type("SPACE", VK_SPACE));
	m_keys.insert(KeySet::value_type("DECIMAL", VK_DECIMAL));
	m_keys.insert(KeySet::value_type("F2", VK_F2));
	m_keys.insert(KeySet::value_type("F3", VK_F3));
	m_keys.insert(KeySet::value_type("F4", VK_F4));
	m_keys.insert(KeySet::value_type("F5", VK_F5));
	m_keys.insert(KeySet::value_type("F6", VK_F6));
	m_keys.insert(KeySet::value_type("F7", VK_F7));
	m_keys.insert(KeySet::value_type("F8", VK_F8));
	m_keys.insert(KeySet::value_type("F9", VK_F9));
	m_keys.insert(KeySet::value_type("F10", VK_F10));
	m_keys.insert(KeySet::value_type("F11", VK_F11));
	m_keys.insert(KeySet::value_type("F12", VK_F12));
	m_keys.insert(KeySet::value_type("NUMPAD0", VK_NUMPAD0));
	m_keys.insert(KeySet::value_type("NUMPAD1", VK_NUMPAD1));
	m_keys.insert(KeySet::value_type("NUMPAD2", VK_NUMPAD2));
	m_keys.insert(KeySet::value_type("NUMPAD3", VK_NUMPAD3));
	m_keys.insert(KeySet::value_type("NUMPAD4", VK_NUMPAD4));
	m_keys.insert(KeySet::value_type("NUMPAD5", VK_NUMPAD5));
	m_keys.insert(KeySet::value_type("NUMPAD6", VK_NUMPAD6));
	m_keys.insert(KeySet::value_type("NUMPAD7", VK_NUMPAD7));
	m_keys.insert(KeySet::value_type("NUMPAD8", VK_NUMPAD8));
	m_keys.insert(KeySet::value_type("NUMPAD9", VK_NUMPAD9));
	m_keys.insert(KeySet::value_type("NOKEY", 0));

	for (char i = '0'; i <= '9'; ++i)
	{
		m_keys.insert(KeySet::value_type(tstring(1, i), (KeyCode)i));
	}

	for (char i = 'A'; i <= 'Z'; ++i)
	{
		m_keys.insert(KeySet::value_type(tstring(1, i), (KeyCode)i));
	}
}

RDORuntime::RDOHotKeyToolkit::KeyCode RDORuntime::RDOHotKeyToolkit::codeFromString(CREF(tstring) key) const
{
	CIterator it = m_keys.find(key);
	if (it == m_keys.end())
	{
		return RDORuntime::RDOHotKeyToolkit::KeyCode(UNDEFINED_KEY);
//		throw RDORuntimeException("Unknown key name: " + key);
	}
	return (*it).second;
}

CLOSE_RDO_RUNTIME_NAMESPACE
