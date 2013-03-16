/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_runtime.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     RDORuntime
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RUNTIME_H_
#define _LIB_RUNTIME_RUNTIME_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif // COMPILER_VISUAL_STUDIO
#include <time.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdostream.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_memory.h"
#include "simulator/runtime/thread_proxy_i.h"
#include "simulator/runtime/notify.h"
#include "simulator/runtime/error.h"
#include "simulator/runtime/hotkey.h"
#include "simulator/report/error_code.h"
#include "simulator/runtime/src/db/general_db.h"
// --------------------------------------------------------------------------------

class RDOThread;

OPEN_RDO_ANIMATION_NAMESPACE
struct Frame;
CLOSE_RDO_ANIMATION_NAMESPACE

OPEN_RDO_RUNTIME_NAMESPACE

//! Результаты моделирования
class RDOResults
{
public:
	RDOResults();
	virtual ~RDOResults();

	void width(ruint w);

	template<class T>
	REF(RDOResults) operator<< (CREF(T) value);

	virtual void              flush     () = 0;
	virtual REF(rdo::ostream) getOStream() = 0;
};

class RDOEvent;
class RDORule;
class RDOOperation;
class RDOPROCProcess;
class RDOPMDResult;
PREDECLARE_POINTER(RDOFRMFrame);
class RDOCalcCreateResource;
PREDECLARE_POINTER(RDOEraseResRelCalc);

//! Симулятор, вычислитель, рантайм машина
CLASS(RDORuntime): INSTANCE_OF(RDOSimulatorTrace)
{
DECLARE_FACTORY(RDORuntime);
public:
	void init  ();
	void deinit();

	typedef  std::vector<LPIResult>           LPIResultList;
	typedef  std::vector<LPIResultTrace>      LPIResultTraceList;
	typedef  std::vector<LPIResultWatchValue> LPIResultWatchValueList;

	//! Подписка на внутренние сообщения
	REF(Notify) notify();

	//! Формирование ошибок рантайма
	REF(Error) error();

	//! Горячие клавиши
	REF(RDOHotKey) hotkey();

	LPRDORuntime clone   () const;
	void         copyFrom(CREF(LPRDORuntime) pOther);
	rbool        equal   (CREF(LPRDORuntime) pOther) const;

	void     setConstValue(ruint constID, CREF(RDOValue) constValue);
	RDOValue getConstValue(ruint constID) const;

	void rdoInit(PTR(RDOTrace) tracer, PTR(RDOResults) customResults, PTR(RDOResults) customResultsInfo, CREF(LPIThreadProxy) pThreadProxy);

	REF(RDOResults) getResults    ();
	REF(RDOResults) getResultsInfo();

	double getTimeNow();
	double getSeconds();

	ruint getCurrentTerm() const;
	void  setCurrentTerm(ruint value);

	REF(LPIActivity) getCurrentActivity();
	void             setCurrentActivity(CREF(LPIActivity) activity);

	void addRuntimeEvent    (LPIBaseOperationContainer pLogic, CREF(LPIEvent)      pEvent    );
	void addRuntimeRule     (LPIBaseOperationContainer pLogic, CREF(LPIRule)       pRule     );
	void addRuntimeOperation(LPIBaseOperationContainer pLogic, CREF(LPIOperation)  pOperation);
	void addRuntimeResult   (CREF(LPIResult)     pResult);
	void addRuntimeFrame    (CREF(LPRDOFRMFrame) pFrame);

	LPRDOFRMFrame lastFrame() const;

	CREF(LPIResultList) getResult() const;

	void addInitCalc(CREF(LPRDOCalc) initCalc);

	// Параметры ресурса
	REF(RDOValue) getResParamValRaw(ruint resID, ruint paramID);
	void          setResParamVal   (ruint resID, ruint paramID, CREF(RDOValue) value);

#ifdef _DEBUG
	rbool checkState   ();
	void  showResources(int node) const;
#endif

	void onEraseRes(ruint resourceID, CREF(LPRDOEraseResRelCalc) pCalc);
	LPRDOResource createNewResource(ruint type, PTR(RDOCalcCreateResource) calc);
	LPRDOResource createNewResource(ruint type, rbool trace);
	void insertNewResource         (CREF(LPRDOResource) pResource);

	RDOValue      getFuncArgument (ruint paramID) const;
	LPRDOResource getGroupFuncRes () const;
	void          pushFuncArgument(RDOValue arg);
	void          pushGroupFunc   (CREF(LPRDOResource) pResource);
	void          popFuncArgument ();
	void          popGroupFunc    ();
	void          pushFuncTop     ();
	void          resetFuncTop    (int numArg);
	void          popFuncTop      ();

	virtual rbool endCondition();
	void setTerminateIf(CREF(LPRDOCalc) pTerminateIfCalc);

	virtual rbool breakPoints();
	void      insertBreakPoint     (CREF(tstring) name, CREF(LPRDOCalc) pCalc);
	LPRDOCalc findBreakPoint       (CREF(tstring) name);
	tstring   getLastBreakPointName() const;

	LPRDOResource getResourceByID(ruint resourceID) const;

	void     setPatternParameter(ruint paramID, CREF(RDOValue) paramValue);
	RDOValue getPatternParameter(ruint paramID) const;

	typedef  std::vector<LPRDOFRMFrame>  FrameList;
	FrameList m_frameList;

	virtual void onPutToTreeNode();

	rdo::simulation::report::RDOExitCode m_whyStop;

	virtual void onNothingMoreToDo();
	virtual void onEndCondition();
	virtual void onRuntimeError();
	virtual void onUserBreak();

	virtual void postProcess();

	/*!
	  \enum      FunBreakFlag
	  \brief     Флаг остановки функции
	*/
	enum FunBreakFlag
	{
		FBF_NONE = 0,
		FBF_BREAK,
		FBF_RETURN
	};

	void               setFunBreakFlag(CREF(FunBreakFlag) flag);
	CREF(FunBreakFlag) getFunBreakFlag() const;

	LPRDOMemoryStack getMemoryStack();

	typedef std::list<LPRDOResource> ResList;
	typedef ResList::const_iterator  ResCIterator;

	ResCIterator res_begin() const;
	ResCIterator res_end() const;

	CREF(LPIThreadProxy) getThreadProxy() const;

	void setStudioThread(PTR(RDOThread) pStudioThread);

	PTR(rdo::animation::Frame) getPreparingFrame() const;
	void setPreparingFrame  (PTR(rdo::animation::Frame) pPreparingFrame);
	void resetPreparingFrame();

	PTR(GeneralDB) getDB();

private:
	RDORuntime(PTR(Error) pError);
	virtual ~RDORuntime();

	typedef  RDOSimulatorTrace           parent_type;
	typedef  std::list<LPRDOCalc>        CalcList;
	typedef  std::vector<RDOValue>       ValueList;
	typedef  std::vector<LPRDOResource>  ResourceListByID;
	typedef  std::list  <LPRDOResource>  ResourceListByTime;

	ResourceListByID    m_resourceListByID;      // Все ресурсы симулятора, даже NULL (NULL стоит на месте уже удаленного временного ресурса)
	ResourceListByTime  m_resourceListByTime;    // Они же, только упорядочены по времени создания и без NULL-ов
	CalcList            m_initCalcList;
	LPRDOMemoryStack    m_pMemoryStack;
	FunBreakFlag        m_funBreakFlag;
	LPIThreadProxy      m_pThreadProxy;
	PTR(RDOThread)      m_pStudioThread;
	Notify              m_notify;
	PTR(Error)          m_pError;
	RDOHotKey           m_hotKey;
	GeneralDB*          m_db;

#ifdef _DEBUG
	typedef  std::vector<ValueList>  State;
	State  m_state;
#endif

	//! Точка останова
	OBJECT(BreakPoint) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(BreakPoint)
	public:
		CREF(tstring)   getName() const;
		CREF(LPRDOCalc) getCalc() const;

	private:
		BreakPoint(CREF(tstring) name, CREF(LPRDOCalc) pCalc);

		tstring   m_name;
		LPRDOCalc m_pCalc;
	};

	typedef std::list<LPBreakPoint> BreakPointList;

	BreakPointList  m_breakPointList;
	LPBreakPoint    m_pLastActiveBreakPoint;

	ValueList         m_funcStack;
	ResourceListByID  m_groupFuncStack;
	int               m_currFuncTop;
	int               m_savedFuncTop;

	virtual void onInit   ();
	virtual void onDestroy();

	virtual ResList getResourcesBeforeSim() const;

	LPIResultList            m_resultAllList;
	LPIResultTraceList       m_resultTraceList;
	LPIResultWatchValueList  m_resultWatchValueList;
	LPIActivity              m_currActivity;
	ValueList                m_patternParameterList;

	time_t m_physicTime;
	virtual void preProcess();

	PTR(RDOResults) m_resultList;
	PTR(RDOResults) m_resultListInfo;

	LPRDOCalc       m_pTerminateIfCalc;
	ValueList       m_constantList;

	void writeExitCode();

	virtual rbool isKeyDown() const;

	virtual void onResetResult();
	virtual void onCheckResult();
	virtual void onAfterCheckResult();

	ruint m_currentTerm;

	PTR(rdo::animation::Frame) m_pPreparingFrame;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_runtime.inl"

#endif // _LIB_RUNTIME_RUNTIME_H_
