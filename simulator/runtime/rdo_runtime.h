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
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#pragma warning(disable : 4786)
#endif // COMPILER_VISUAL_STUDIO
#include <time.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_memory.h"
#include "simulator/runtime/thread_proxy_i.h"
#include "simulator/runtime/notify.h"
#include "simulator/runtime/error.h"
#include "simulator/runtime/hotkey.h"
#include "simulator/report/src/error_code.h"
// --------------------------------------------------------------------------------

class RDOThread;

namespace rdo {
namespace animation {

struct Frame;

}} // namespace rdo::animation

OPEN_RDO_RUNTIME_NAMESPACE

//! Результаты моделирования
class RDOResults
{
public:
	RDOResults();
	virtual ~RDOResults();

	void width(std::size_t w);

	template<class T>
	RDOResults& operator<< (CREF(T) value);

	virtual void          flush     () = 0;
	virtual std::ostream& getOStream() = 0;
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
class RDORuntime: public RDOSimulatorTrace
{
DECLARE_FACTORY(RDORuntime);
public:
	void init  ();
	void deinit();

	typedef  std::vector<LPIResult>           LPIResultList;
	typedef  std::vector<LPIResultTrace>      LPIResultTraceList;
	typedef  std::vector<LPIResultWatchValue> LPIResultWatchValueList;

	//! Подписка на внутренние сообщения
	Notify& notify();

	//! Формирование ошибок рантайма
	Error& error();

	//! Горячие клавиши
	RDOHotKey& hotkey();

	LPRDORuntime clone   () const;
	void copyFrom(CREF(LPRDORuntime) pOther);
	bool equal(CREF(LPRDORuntime) pOther) const;

	void setConstValue(std::size_t constID, CREF(RDOValue) constValue);
	RDOValue getConstValue(std::size_t constID) const;

	void rdoInit(RDOTrace* tracer, RDOResults* customResults, RDOResults* customResultsInfo, CREF(LPIThreadProxy) pThreadProxy);

	RDOResults& getResults();
	RDOResults& getResultsInfo();

	double getTimeNow();
	double getSeconds();

	std::size_t getCurrentTerm() const;
	void  setCurrentTerm(std::size_t value);

	LPIActivity& getCurrentActivity();
	void setCurrentActivity(CREF(LPIActivity) activity);

	void addRuntimeEvent    (LPIBaseOperationContainer pLogic, CREF(LPIEvent)      pEvent    );
	void addRuntimeRule     (LPIBaseOperationContainer pLogic, CREF(LPIRule)       pRule     );
	void addRuntimeOperation(LPIBaseOperationContainer pLogic, CREF(LPIOperation)  pOperation);
	void addRuntimeResult   (CREF(LPIResult)     pResult);
	void addRuntimeFrame    (CREF(LPRDOFRMFrame) pFrame);

	LPRDOFRMFrame lastFrame() const;

	CREF(LPIResultList) getResult() const;

	void addInitCalc(CREF(LPRDOCalc) initCalc);

	// Параметры ресурса
	RDOValue& getResParamValRaw(std::size_t resID, std::size_t paramID);
	void setResParamVal(std::size_t resID, std::size_t paramID, CREF(RDOValue) value);

#ifdef _DEBUG
	bool checkState();
	void showResources(int node) const;
#endif

	void onEraseRes(std::size_t resourceID, CREF(LPRDOEraseResRelCalc) pCalc);
	LPRDOResource createNewResource(std::size_t type, RDOCalcCreateResource* calc);
	LPRDOResource createNewResource(std::size_t type, bool trace);
	void insertNewResource(CREF(LPRDOResource) pResource);

	RDOValue      getFuncArgument (std::size_t paramID) const;
	LPRDOResource getGroupFuncRes () const;
	void          pushFuncArgument(RDOValue arg);
	void          pushGroupFunc   (CREF(LPRDOResource) pResource);
	void          popFuncArgument ();
	void          popGroupFunc    ();
	void          pushFuncTop     ();
	void          resetFuncTop    (int numArg);
	void          popFuncTop      ();

	virtual bool endCondition();
	void setTerminateIf(CREF(LPRDOCalc) pTerminateIfCalc);

	virtual bool breakPoints();
	void insertBreakPoint(CREF(std::string) name, CREF(LPRDOCalc) pCalc);
	LPRDOCalc findBreakPoint(CREF(std::string) name);
	std::string getLastBreakPointName() const;

	LPRDOResource getResourceByID(std::size_t resourceID) const;

	void     setPatternParameter(std::size_t paramID, CREF(RDOValue) paramValue);
	RDOValue getPatternParameter(std::size_t paramID) const;

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

	void addResType(CREF(LPRDOResourceTypeList) pResType);
	CREF(LPRDOResourceTypeList) getResType(std::size_t number) const;

	CREF(LPIThreadProxy) getThreadProxy() const;

	void setStudioThread(RDOThread* pStudioThread);

	rdo::animation::Frame* getPreparingFrame() const;
	void setPreparingFrame(rdo::animation::Frame* pPreparingFrame);
	void resetPreparingFrame();

private:
	RDORuntime(Error* pError);
	virtual ~RDORuntime();

	typedef  RDOSimulatorTrace                   parent_type;
	typedef  std::list<LPRDOCalc>                CalcList;
	typedef  std::vector<RDOValue>               ValueList;
	typedef  std::vector<LPRDOResourceTypeList>  ResourceTypeList;
	typedef  std::vector<LPRDOResource>          ResourceListByID;

	ResourceTypeList    m_resourceTypeList;
	ResourceListByID    m_resourceListByID;      // Все ресурсы симулятора, даже NULL (NULL стоит на месте уже удаленного временного ресурса)
	CalcList            m_initCalcList;
	LPRDOMemoryStack    m_pMemoryStack;
	FunBreakFlag        m_funBreakFlag;
	LPIThreadProxy      m_pThreadProxy;
	RDOThread*          m_pStudioThread;
	Notify              m_notify;
	Error*              m_pError;
	RDOHotKey           m_hotKey;

#ifdef _DEBUG
	typedef  std::vector<ValueList>  State;
	State  m_state;
#endif

	//! Точка останова
	PREDECLARE_POINTER(BreakPoint);
	class BreakPoint
		: public rdo::counter_reference
		, public RDORuntimeObject
	{
	DECLARE_FACTORY(BreakPoint)
	public:
		CREF(std::string) getName() const;
		CREF(LPRDOCalc) getCalc() const;

	private:
		BreakPoint(CREF(std::string) name, CREF(LPRDOCalc) pCalc);

		std::string m_name;
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

	RDOResults* m_resultList;
	RDOResults* m_resultListInfo;

	LPRDOCalc       m_pTerminateIfCalc;
	ValueList       m_constantList;

	void writeExitCode();

	virtual bool isKeyDown() const;

	virtual void onResetResult();
	virtual void onCheckResult();
	virtual void onAfterCheckResult();

	std::size_t m_currentTerm;

	rdo::animation::Frame* m_pPreparingFrame;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_runtime.inl"

#endif // _LIB_RUNTIME_RUNTIME_H_
