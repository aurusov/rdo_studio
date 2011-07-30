/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_runtime.h
 * @authors   Барс Александ, Урусов Андрей, Лущан Дмитрий
 * @date      unknown
 * @brief     RDORuntime implementation
 * @indent    4T
 *********************************************************************************/

#ifndef _RDO_RUNTIME_H_
#define _RDO_RUNTIME_H_

#pragma warning(disable : 4786)

// *********************************************************************** INCLUDES
#include <time.h>
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdocommon.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/simtrace.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_memory.h"
#include "rdo_lib/rdo_runtime/thread_proxy_i.h"
// ********************************************************************************

class RDOThread;

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOResults
// ********************************************************************************
class RDOResults
{
public:
	RDOResults()
	{}

	virtual ~RDOResults()
	{}

	void width(ruint w)
	{
		getOStream().width(w);
	}

	template<class T> REF(RDOResults) operator<< (CREF(T) value)
	{
		getOStream() << value;
		return *this;
	}

	virtual void              flush     () = 0;
	virtual REF(std::ostream) getOStream() = 0;
};

// ********************************************************************************
// ******************** RDORuntime
// ********************************************************************************
class RDOEvent;
class RDORule;
class RDOOperation;
class RDOPROCProcess;
class RDOPMDPokaz;
PREDECLARE_POINTER(RDOFRMFrame);
class RDOCalcCreateResource;
PREDECLARE_POINTER(RDOEraseResRelCalc);

CLASS(RDORuntime): INSTANCE_OF(RDOSimulatorTrace)
{
DECLARE_FACTORY(RDORuntime);
public:
	void init  ();
	void deinit();

	typedef  std::vector<LPIPokaz>           LPIPokazList;
	typedef  std::vector<LPIPokazTrace>      LPIPokazTraceList;
	typedef  std::vector<LPIPokazWatchValue> LPIPokazWatchValueList;

	// Работа с уведомлениями
	enum Messages
	{
		RO_BEFOREDELETE = 0
	};
	void connect    (PTR(INotify) to, ruint message);
	void disconnect (PTR(INotify) to               );
	void disconnect (PTR(INotify) to, ruint message);
	void fireMessage(ruint message, PTR(void) param);

	std::vector< rdoSimulator::RDOSyntaxError > errors;
	void error(CREF(tstring) message, CREF(LPRDOCalc) pCalc = NULL);

	LPRDORuntime clone   () const;
	void         copyFrom(CREF(LPRDORuntime) pOther);
	rbool        equal   (CREF(LPRDORuntime) pOther) const;

	class RDOHotKeyToolkit
	{
	public:
		typedef unsigned int                     KeyCode;
		typedef std::map< tstring, KeyCode > KeySet;
		typedef KeySet::const_iterator           CIterator;
		enum    { UNDEFINED_KEY = ~0 };

		RDOHotKeyToolkit();
		KeyCode codeFromString( CREF(tstring) key ) const;

	private:
		KeySet m_keys;
	};
	RDOHotKeyToolkit rdoHotKeyToolkit;

	std::vector< unsigned int > using_scan_codes;
	rbool keyDown( unsigned int scan_code );
	void keyUp( unsigned int scan_code );
	rbool checkKeyPressed( unsigned int scan_code, rbool shift, rbool control );
	rbool checkAreaActivated( CREF(tstring) oprName );

	void setConstValue( unsigned int numberOfConst, RDOValue value );
	RDOValue getConstValue( int numberOfConst );
	void rdoInit( RDOTrace* tracer, RDOResults* customResults, RDOResults* customResultsInfo, CREF(LPIThreadProxy) pThreadProxy );

	RDOResults& getResults()     { return *results;      }
	RDOResults& getResultsInfo() { return *results_info; }

	double getTimeNow() { return getCurrentTime();                   }
	double getSeconds() { return (double)(time(NULL) - physic_time); }
	
	unsigned int getCurrentTerm() const        {return m_currentTerm;  }
	void setCurrentTerm( unsigned int value	)  {m_currentTerm = value; }

	REF(LPIActivity) getCurrentActivity()                           { return m_currActivity;      }
	void             setCurrentActivity(CREF(LPIActivity) activity) { m_currActivity = activity;  }

	void addRuntimeEvent    (LPIBaseOperationContainer logic, CREF(LPIEvent)      ev      );
	void addRuntimeRule     (LPIBaseOperationContainer logic, CREF(LPIRule)       rule    );
	void addRuntimeOperation(LPIBaseOperationContainer logic, CREF(LPIOperation)  opration);
	void addRuntimePokaz    (CREF(LPIPokaz)      pPokaz);
	void addRuntimeFrame    (CREF(LPRDOFRMFrame) pFrame);
	
	LPRDOFRMFrame lastFrame() const;

	CREF(LPIPokazList) getPokaz() const { return m_pokazAllList; }

	void addInitCalc(CREF(LPRDOCalc) initCalc) { initCalcs.push_back( initCalc ); }

	// Параметры ресурса
	RDOValue getResParamVal(ruint resID, ruint paramID) const
	{
		LPRDOResource pResource = getResourceByID(resID);
		ASSERT(pResource);
		return pResource->getParam(paramID);
	}
	REF(RDOValue) getResParamValRaw(ruint resID, ruint paramID)
	{
		LPRDOResource pResource = getResourceByID(resID);
		ASSERT(pResource);
		return pResource->getParamRaw(paramID);
	}
	void setResParamVal(ruint resID, ruint paramID, CREF(RDOValue) value)
	{
		LPRDOResource pResource = getResourceByID(resID);
		ASSERT(pResource);
		pResource->setParam(paramID, value);
	}

#ifdef _DEBUG
	std::vector< std::vector< RDOValue > > state;
	rbool checkState();
	void showResources( int node ) const;
#endif

	void onEraseRes(const int res_id, CREF(LPRDOEraseResRelCalc) pCalc);
	LPRDOResource createNewResource( unsigned int type, RDOCalcCreateResource* calc );
	LPRDOResource createNewResource( unsigned int type, rbool trace );
	void insertNewResource         (CREF(LPRDOResource) pResource);

	RDOValue      getFuncArgument (int numberOfParam); 
	LPRDOResource getGroupFuncRes ()                              { return groupFuncStack.back();                                  }
	void          pushFuncArgument( RDOValue arg )                { funcStack.push_back(arg);                                      }
	void          pushGroupFunc   (CREF(LPRDOResource) pResource) { groupFuncStack.push_back(pResource);                           }
	void          popFuncArgument ()                              { funcStack.pop_back();                                          }
	void          popGroupFunc    ()                              { groupFuncStack.pop_back();                                     }
	void          pushFuncTop     ()                              { funcStack.push_back(RDOValue(currFuncTop));                    }
	void          resetFuncTop    (int numArg)                    { currFuncTop = funcStack.size() - numArg;                       }
	void          popFuncTop      ()                              { currFuncTop = funcStack.back().getInt(); funcStack.pop_back(); }

	virtual rbool endCondition();
	void setTerminateIf(CREF(LPRDOCalc) _pTerminateIfCalc);

	virtual rbool breakPoints();
	void insertBreakPoint( CREF(tstring) name, CREF(LPRDOCalc) pCalc );
	LPRDOCalc findBreakPoint( CREF(tstring) name );
	tstring getLastBreakPointName() const;

	LPRDOResource getResourceByID(const int num) const {return num >= 0 ? allResourcesByID.at( num ) : NULL;}

	void setPatternParameter( unsigned int parNumb, RDOValue val )
	{ 
		if ( patternParameters.size() <= parNumb )
		{
			patternParameters.resize(parNumb + 1);
		}
		patternParameters.at(parNumb) = val;
	}
	RDOValue getPatternParameter(int parNumb) 
	{ 
		return patternParameters.at(parNumb);
	}

	std::vector< tstring >      activeAreasMouseClicked;
	std::list< unsigned int >   keysDown;
	std::vector<LPRDOFRMFrame>  allFrames;

	virtual void onPutToTreeNode();

	rdoSimulator::RDOExitCode whyStop;
	virtual void onNothingMoreToDo() { whyStop = rdoSimulator::EC_NoMoreEvents; }
	virtual void onEndCondition()    { whyStop = rdoSimulator::EC_OK;           }
	virtual void onRuntimeError()    { whyStop = rdoSimulator::EC_RunTimeError; }
	virtual void onUserBreak()       { whyStop = rdoSimulator::EC_UserBreak;    }

	virtual void postProcess();

	enum FunBreakFlag
	{
		FBF_CONTINUE = 0,
		FBF_BREAK,
		FBF_RETURN
	};
	void               setFunBreakFlag(CREF(FunBreakFlag) flag);
	CREF(FunBreakFlag) getFunBreakFlag() const;

	LPRDOMemoryStack getMemoryStack();

	typedef std::list<LPRDOResource> ResList;
	typedef ResList::const_iterator  ResCIterator;

	ResCIterator res_begin() const
	{
		return allResourcesByTime.begin();
	}
	ResCIterator res_end() const
	{
		return allResourcesByTime.end();
	}

	CREF(LPIThreadProxy) getThreadProxy() const { return m_pThreadProxy; }

	void setStudioThread(PTR(RDOThread) pStudioThread);

private:
	RDORuntime();
	virtual ~RDORuntime();

	typedef RDOSimulatorTrace           Parent;
	typedef std::list<LPRDOCalc>        CalcList;

	std::vector<LPRDOResource> allResourcesByID;      // Все ресурсы симулятора, даже NULL (NULL стоит на месте уже удаленного временного ресурса)
	std::list  <LPRDOResource> allResourcesByTime;    // Они же, только упорядочены по времени создания и без NULL-ов
	CalcList                    initCalcs;
	LPRDOMemoryStack            m_pMemoryStack;
	FunBreakFlag                m_funBreakFlag;
	LPIThreadProxy              m_pThreadProxy;
	PTR(RDOThread)              m_pStudioThread;

	OBJECT(BreakPoint) IS INSTANCE_OF(RDORuntimeObject)
	{
	DECLARE_FACTORY(BreakPoint)
	public:
		CREF(tstring) getName() const
		{
			return m_name;
		}
		CREF(LPRDOCalc) getCalc() const
		{
			return m_pCalc;
		}

	private:
		BreakPoint(CREF(tstring) name, CREF(LPRDOCalc) pCalc)
			: m_name (name )
			, m_pCalc(pCalc)
		{}

		tstring   m_name;
		LPRDOCalc m_pCalc;
	};

	typedef std::list<LPBreakPoint> BreakPointList;
	BreakPointList  breakPointsCalcs;
	LPBreakPoint    lastActiveBreakPoint;

	std::vector< RDOValue >     funcStack;
	std::vector<LPRDOResource>  groupFuncStack;
	int currFuncTop;
	int savedFuncTop;

	virtual void onInit();
	virtual void onDestroy();

	virtual ResList getResourcesBeforeSim() const
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

	LPIPokazList            m_pokazAllList;
	LPIPokazTraceList       m_pokazTraceList;
	LPIPokazWatchValueList  m_pokazWatchValueList;

	LPIActivity             m_currActivity;

	std::vector<RDOValue> patternParameters;

	time_t physic_time;
	virtual void preProcess()
	{
		Parent::preProcess();
		physic_time = time(NULL);
	}

	RDOResults* results;
	RDOResults* results_info;

	LPRDOCalc pTerminateIfCalc;
	std::vector< RDOValue > allConstants;

	void writeExitCode();

	rbool key_found;
	virtual rbool isKeyDown();

	typedef std::multimap<ruint, PTR(INotify)> Connected;
	Connected m_connected;

	virtual void onResetPokaz();
	virtual void onCheckPokaz();
	virtual void onAfterCheckPokaz();

	unsigned int m_currentTerm;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _RDO_RUNTIME_H_
