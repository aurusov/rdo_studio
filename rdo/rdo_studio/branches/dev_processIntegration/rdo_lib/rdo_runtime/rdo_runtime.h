/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_runtime.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_RUNTIME_H_
#define _RDO_RUNTIME_H_

#pragma warning(disable : 4786)  

// ====================================================================== INCLUDES
#include <time.h>
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdocommon.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/simtrace.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_memory.h"
#include "rdo_lib/rdo_runtime/thread_proxy_i.h"
// ===============================================================================

class RDOThread;

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOResults
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDORuntime
// ----------------------------------------------------------------------------
class RDOEvent;
class RDORule;
class RDOOperation;
class RDOPROCProcess;
class RDOPMDPokaz;
class RDOPattern;
class RDOFRMFrame;
class RDOCalcCreateResource;
PREDECLARE_POINTER(RDOEraseResRelCalc);

class RDORuntime: public RDOSimulatorTrace
{
public:
	RDORuntime();
	virtual ~RDORuntime();

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

	class RDOHotKeyToolkit
	{
	public:
		typedef unsigned int                     KeyCode;
		typedef std::map< std::string, KeyCode > KeySet;
		typedef KeySet::const_iterator           CIterator;
		enum    { UNDEFINED_KEY = ~0 };

		RDOHotKeyToolkit();
		KeyCode codeFromString( const std::string& key ) const;

	private:
		KeySet m_keys;
	};
	RDOHotKeyToolkit rdoHotKeyToolkit;

	std::vector< unsigned int > using_scan_codes;
	rbool keyDown( unsigned int scan_code );
	void keyUp( unsigned int scan_code );
	rbool checkKeyPressed( unsigned int scan_code, rbool shift, rbool control );
	rbool checkAreaActivated( const std::string& oprName );

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
	void addRuntimePokaz    (CREF(LPIPokaz)      pokaz   );
	void addRuntimeFrame    (PTR(RDOFRMFrame)    frame   );
	
	RDOFRMFrame* lastFrame() const;

	CREF(LPIPokazList) getPokaz() const { return m_pokazAllList; }

	void addInitCalc(CREF(LPRDOCalc) initCalc) { initCalcs.push_back( initCalc ); }

	// Параметры ресурса
	RDOValue getResParamVal(ruint resID, ruint paramID) const
	{
		CPTR(RDOResource) res = getResourceByID(resID);
		ASSERT(res);
		return res->getParam(paramID);
	}
	REF(RDOValue) getResParamValRaw(ruint resID, ruint paramID)
	{
		PTR(RDOResource) res = getResourceByID(resID);
		ASSERT(res);
		return res->getParamRaw(paramID);
	}
	void setResParamVal( unsigned int res_id, unsigned int param_id, const RDOValue& value ) {
		RDOResource* res = getResourceByID( res_id );
		res->setParam( param_id, value );
	}

#ifdef _DEBUG
	std::vector< std::vector< RDOValue > > state;
	rbool checkState();
	void showResources( int node ) const;
#endif

	void onEraseRes(const int res_id, CREF(LPRDOEraseResRelCalc) pCalc);
	RDOResource* createNewResource( unsigned int type, RDOCalcCreateNumberedResource* calc );
	RDOResource* createNewResource( unsigned int type, rbool trace );
	void insertNewResource( RDOResource* res );

	RDOValue getFuncArgument(int numberOfParam); 
	RDOResource* getGroupFuncRes()         { return groupFuncStack.back();                         }
	void pushFuncArgument( RDOValue arg )  { funcStack.push_back( arg );                           }
	void pushGroupFunc( RDOResource* res ) { groupFuncStack.push_back( res );                      }
	void popFuncArgument()                 { funcStack.pop_back();                                 }
	void popGroupFunc()                    { groupFuncStack.pop_back();                            }
	void pushFuncTop()                     { funcStack.push_back( RDOValue(currFuncTop) );         }
	void resetFuncTop( int numArg )        { currFuncTop = funcStack.size() - numArg;              }
	void popFuncTop()                      { currFuncTop = funcStack.back().getInt(); funcStack.pop_back(); }

	virtual rbool endCondition();
	void setTerminateIf(CREF(LPRDOCalc) _pTerminateIfCalc);

	virtual rbool breakPoints();
	void insertBreakPoint( const std::string& name, CREF(LPRDOCalc) pCalc );
	LPRDOCalc findBreakPoint( const std::string& name );
	std::string getLastBreakPointName() const;

	RDOResource* getResourceByID( const int num ) const { return num >= 0 ? allResourcesByID.at( num ) : NULL; }

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

	std::vector< std::string >  activeAreasMouseClicked;
	std::list< unsigned int >   keysDown;
	std::vector< RDOFRMFrame* > allFrames;

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

	typedef std::list< RDOResource* > ResList;
	typedef ResList::const_iterator ResCIterator;

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
	typedef RDOSimulatorTrace           Parent;
	typedef std::list<LPRDOCalc>        CalcList;
	std::vector< RDOResource* > allResourcesByID;      // Все ресурсы симулятора, даже NULL (NULL стоит на месте уже удаленного временного ресурса)
	std::list  < RDOResource* > allResourcesByTime;    // Они же, только упорядочены по времени создания и без NULL-ов
	std::list  < RDOResource* > allResourcesBeforeSim; // Они же, только упорядочены по типу перед запуском
	CalcList                    initCalcs;
	LPRDOMemoryStack            m_pMemoryStack;
	FunBreakFlag                m_funBreakFlag;
	LPIThreadProxy              m_pThreadProxy;
	PTR(RDOThread)              m_pStudioThread;
	
	class BreakPoint: public RDORuntimeObject
	{
	public:
		std::string name;
		LPRDOCalc   pCalc;
		BreakPoint( RDORuntimeParent* _parent, const std::string& _name, CREF(LPRDOCalc) _pCalc ):
			RDORuntimeObject( _parent ),
			name ( _name  ),
			pCalc( _pCalc )
		{}
	};

	std::list< BreakPoint* > breakPointsCalcs;
	BreakPoint*              lastActiveBreakPoint;

	std::vector< RDOValue >     funcStack;
	std::vector< RDOResource* > groupFuncStack;
	int currFuncTop;
	int savedFuncTop;

	virtual void onInit();
	virtual void onDestroy();

	virtual ResList getResourcesBeforeSim() const
	{
		ResList list;
		ResCIterator it = allResourcesBeforeSim.begin();
		while ( it != allResourcesBeforeSim.end() ) {
			list.push_back( *it );
			it++;
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

	virtual RDOSimulator* clone();
	virtual void operator=  (const RDORuntime& other);
	virtual rbool operator== (CREF(RDOSimulator) other);

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

#endif //! _RDO_RUNTIME_H_
