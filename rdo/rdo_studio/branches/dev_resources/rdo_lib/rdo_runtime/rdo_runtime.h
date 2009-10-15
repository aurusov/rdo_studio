#ifndef RDO_RUNTIME_H
#define RDO_RUNTIME_H

#pragma warning(disable : 4786)  

// ====================================================================== INCLUDES
#include <time.h>
// ====================================================================== SYNOPSIS
#include "rdotrace.h"
#include "simtrace.h"
#include "rdo_resource.h"
#include "rdo_runtime_interface_registrator.h"
#include <rdocommon.h>
// ===============================================================================

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOResults
// ----------------------------------------------------------------------------
class RDOResults
{
private:
	std::ofstream out;
	virtual std::ostream& getOStream() { return out; }

protected:
	bool isNullResult;

public:
	RDOResults( const char* const fileName ):
		out( fileName, std::ios::out ),
		isNullResult( false )
	{
	}
	RDOResults():
		isNullResult( true )
	{
	}
	virtual ~RDOResults()
	{
	}
	int width( int w ) {
		return isNullResult ? 0 : getOStream().width(w);
	}
	template< class TN > RDOResults& operator << (TN str) {
		if ( !isNullResult ) getOStream() << str;
		return *this;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDORuntime
// ----------------------------------------------------------------------------
class RDOIrregEvent;
class RDORule;
class RDOOperation;
class RDOPROCProcess;
class RDOPMDPokaz;
class RDOPattern;
class RDOCalcEraseRes;
class RDOFRMFrame;
class RDOCalcCreateNumberedResource;

class RDORuntime: public RDOSimulatorTrace
{
public:
	RDORuntime();
	virtual ~RDORuntime();

	typedef  std::vector<LPIPokaz>           LPIPokazList;
	typedef  std::vector<LPIPokazTrace>      LPIPokazTraceList;
	typedef  std::vector<LPIPokazWatchValue> LPIPokazWatchValueList;

	// –абота с уведомлени€ми
	enum Messages
	{
		RO_BEFOREDELETE = 0
	};
	void connect    (PTR(INotify) to, ruint message);
	void disconnect (PTR(INotify) to               );
	void disconnect (PTR(INotify) to, ruint message);
	void fireMessage(ruint message, PTR(void) param);

	std::vector< rdoSimulator::RDOSyntaxError > errors;
	void error( const std::string& message, const RDOCalc* calc = NULL );

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
	bool keyDown( unsigned int scan_code );
	void keyUp( unsigned int scan_code );
	bool checkKeyPressed( unsigned int scan_code, bool shift, bool control );
	bool checkAreaActivated( const std::string& oprName );

	void setConstValue( unsigned int numberOfConst, RDOValue value );
	RDOValue getConstValue( int numberOfConst );
	void rdoInit( RDOTrace* tracer, RDOResults* customResults, RDOResults* customResultsInfo );

	RDOResults& getResults()     { return *results;      }
	RDOResults& getResultsInfo() { return *results_info; }

	double getTimeNow() { return getCurrentTime();                   }
	double getSeconds() { return (double)(time(NULL) - physic_time); }
	
	unsigned int getCurrentTerm() const        {return m_currentTerm;  }
	void setCurrentTerm( unsigned int value	)  {m_currentTerm = value; }

	REF(LPIActivity) getCurrentActivity()                           { return m_currActivity;      }
	void             setCurrentActivity(CREF(LPIActivity) activity) { m_currActivity = activity;  }

	void addRuntimeIE       (CREF(LPIIrregEvent) ie      );
	void addRuntimeRule     (CREF(LPIRule)       rule    );
	void addRuntimeOperation(CREF(LPIOperation)  opration);
	void addRuntimePokaz    (CREF(LPIPokaz)      pokaz   );
	void addRuntimeFrame    (PTR(RDOFRMFrame)    frame   );
	
	RDOFRMFrame* lastFrame() const;

	CREF(LPIPokazList) getPokaz() const { return m_pokazAllList; }

	void addInitCalc( RDOCalc* initCalc) { initCalcs.push_back( initCalc ); }

	// ѕараметры ресурса
	RDOValue getResParamVal( unsigned int res_id, unsigned int param_id ) const
	{
		RDOResource* res = getResourceByID( res_id );
		return res->getParam( param_id );
	}
	void setResParamVal( unsigned int res_id, unsigned int param_id, const RDOValue& value ) {
		RDOResource* res = getResourceByID( res_id );
		res->setParam( param_id, value );
	}

#ifdef _DEBUG
	std::vector< std::vector< RDOValue > > state;
	bool checkState();
	void showResources( int node ) const;
#endif

	void onEraseRes( const int res_id, const RDOCalcEraseRes* calc );
	RDOResource* createNewResource( unsigned int type, RDOCalcCreateNumberedResource* calc );
	RDOResource* createNewResource( unsigned int type, bool trace );
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

	virtual bool endCondition();
	void setTerminateIf( RDOCalc* _terminateIfCalc );

	virtual bool breakPoints();
	void insertBreakPoint( const std::string& name, RDOCalc* calc );
	RDOCalc* findBreakPoint( const std::string& name );
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

private:
	typedef RDOSimulatorTrace Parent;
	std::vector< RDOResource* > allResourcesByID;      // ¬се ресурсы симул€тора, даже NULL (NULL стоит на месте уже удаленного временного ресурса)
	std::list  < RDOResource* > allResourcesByTime;    // ќни же, только упор€дочены по времени создани€ и без NULL-ов
	std::list  < RDOResource* > allResourcesBeforeSim; // ќни же, только упор€дочены по типу перед запуском
	std::list  < RDOCalc*     > initCalcs;
	
	
	class BreakPoint: public RDORuntimeObject
	{
	public:
		std::string name;
		RDOCalc*    calc;
		BreakPoint( RDORuntimeParent* _parent, const std::string& _name, RDOCalc* _calc ):
			RDORuntimeObject( _parent ),
			name( _name ),
			calc( _calc )
		{
		}
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

	RDOCalc* terminateIfCalc;
	std::vector< RDOValue > allConstants;

	virtual RDOSimulator* clone();
	virtual void operator=  (const RDORuntime& other);
	virtual bool operator== (RDOSimulator& other);

	void writeExitCode();

	bool key_found;
	virtual bool isKeyDown();

	typedef std::multimap<ruint, PTR(INotify)> Connected;
	Connected m_connected;

	virtual void onResetPokaz();
	virtual void onCheckPokaz();
	virtual void onAfterCheckPokaz();

	unsigned int m_currentTerm;
};

} // namespace rdoRuntime

#endif // RDO_RUNTIME_H
