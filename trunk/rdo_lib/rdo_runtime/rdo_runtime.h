#ifndef RDO_RUNTIME_H
#define RDO_RUNTIME_H

#include "rdotrace.h"
#include "simtrace.h"
#include "rdopatrtime.h"
#include <rdocommon.h>

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOConfig
// ----------------------------------------------------------------------------
struct RDOConfig
{
//////// Interactive /////////////////
	rdoSimulator::ShowMode     showAnimation;
//	int                        currFrameToShow;
	std::vector< std::string > allFrameNames;
	std::vector< std::string > activeAreasMouseClicked;
	std::list< unsigned int >  keyDown;
	bool                       mouseClicked;

//////// Frame /////////////////////
	std::vector< rdoSimulator::RDOFrame* > frames;

//////// Timing ///////////////////
	double currTime;		// model time
	double newTime;			// model time
	double showRate;
	double realTimeDelay;	// msec
};

typedef void (*TracerCallBack)( std::string* newString, void* param );
typedef void (*FrameCallBack)( RDOConfig* config, void* param );

// ----------------------------------------------------------------------------
// ---------- RDOResource
// ----------------------------------------------------------------------------
class RDOResource: public RDOResourceTrace
{
public:
	RDOResource( RDORuntime* rt );
	virtual ~RDOResource();

	int number; // unique for all resources alive in system
	int type;
	int referenceCount;
	std::vector< RDOValue > params;
	std::string getTypeId();
	std::string traceParametersValue();
	bool operator != ( RDOResource& other );
};

// ----------------------------------------------------------------------------
// ---------- RDOResult
// ----------------------------------------------------------------------------
class RDOResult
{
private:
	std::ofstream out;
	virtual std::ostream& getOStream() { return out; }

protected:
	bool isNullResult;

public:
	RDOResult( const char* const fileName ):
		out( fileName, std::ios::out ),
		isNullResult( false )
	{
	}
	RDOResult():
		isNullResult( true )
	{
	}
	int width( int w ) {
		return isNullResult ? 0 : getOStream().width(w);
	}
	template< class TN >
		RDOResult& operator << (TN str) {
			if ( !isNullResult ) getOStream() << str;
			return *this;
		}

	virtual ~RDOResult() {}
};

// ----------------------------------------------------------------------------
// ---------- RDORuntime
// ----------------------------------------------------------------------------
class RDOActivityRuleRuntime;
class RDOActivityIERuntime;
class RDOActivityOperationRuntime;
class RDOPROCProcess;
class RDOPMDPokaz;
class RDOPatternRuntime;
class RDOSearchRuntime;
class RDOCalcEraseRes;
class RDOFRMFrame;

class RDORuntime: public RDOSimulatorTrace
{
friend class RDOFunCalcSelect;
friend class RDOFunCalcExist;
friend class RDOFunCalcNotExist;
friend class RDOFunCalcForAll;
friend class RDOFunCalcNotForAll;
friend class RDOSelectResourceByTypeCalc;
friend class RDOSelectResourceByTypeCommonCalc;
friend class RDOPMDWatchQuant;
friend class RDOPMDWatchValue;
friend class RDOPMDWatchPar;	  
friend class RDOPMDWatchState;

private:
	std::vector< RDOResource* > allResourcesByID;    // ¬се ресурсы симул€тора, даже NULL (NULL стоит на месте уже удаленного временного ресурса)
	std::list  < RDOResource* > allResourcesByTime;  // ќни же, только упор€дочены по времени создани€ и без NULL-ов
	RDOTrace* tracer;
	std::list< RDOCalc* > initCalcs;

	std::vector< RDOValue >     funcStack;
	std::vector< RDOResource* > groupFuncStack;
	int currFuncTop;
	int savedFuncTop;

	virtual void onInit();
	virtual void onDestroy();

	virtual std::list< RDOResourceTrace* > getTracebleResources() const {
		std::list< RDOResourceTrace* > list;
		std::list< RDOResource* >::const_iterator it = allResourcesByTime.begin();
		while ( it != allResourcesByTime.end() ) {
			list.push_back( *it );
			it++;
		}
		return list;
	}

	std::vector< RDOPMDPokaz* > allPokaz;

	RDOActivityRuntime* currActivity;

	std::vector<RDOValue> patternParameters;

	time_t physic_time;
	virtual void preProcess()
	{
		RDOSimulatorTrace::preProcess();
		physic_time = time(NULL);
	}

	RDOResult* result; // Output class for results (PMV)

	RDOCalc* terminateIfCalc;
	std::vector< RDOValue > allConstants;

	virtual RDOSimulator* clone();
	virtual bool operator == ( RDOSimulator& other );

	void writeExitCode();

	bool key_found;
	virtual bool isKeyDown();

protected:
	virtual void onResetPokaz();
	virtual void onCheckPokaz();
	virtual void onAfterCheckPokaz();

public:
	std::vector< rdoSimulator::RDOSyntaxError > errors;
	void error( const char* message, const RDOCalc* calc = NULL );

	class RDOHotKeyToolkit {
	private:
		std::map< std::string, int > keys;
	public:
		RDOHotKeyToolkit();
		int codeFromString( std::string* key );
	};
	RDOHotKeyToolkit rdoHotKeyToolkit;

	std::vector< unsigned int > using_scan_codes;
	bool keyDown( unsigned int scan_code );
	void keyUp( unsigned int scan_code );
	bool checkKeyPressed( unsigned int scan_code, bool shift, bool control );
	bool checkAreaActivated( const std::string& oprName );

	void setConstValue(int numberOfConst, RDOValue value);
	RDOValue getConstValue(int numberOfConst);
	RDOResult& getResult() { return *result; }
	void rdoInit( RDOTrace* customTracer, RDOResult* customResult );

	double getTimeNow() { return getCurrentTime(); }
	double getSeconds() { return (time(NULL) - physic_time); }

	void setCurrentActivity( RDOActivityRuntime* pat )           { currActivity = pat;                  }
	void addRuntimeOperation( RDOActivityOperationRuntime* opr );
	void addRuntimeRule( RDOActivityRuleRuntime* rule );
	void addRuntimeIE( RDOActivityIERuntime* ie );
	void addRuntimeProcess( RDOPROCProcess* process );
	void addRuntimeDPT( RDOSearchRuntime* dpt );
	void addRuntimePokaz( RDOPMDPokaz* pok );
	void addRuntimeFrame( RDOFRMFrame* frame );
	RDOFRMFrame* lastFrame() const;


	void addInitCalc(RDOCalc *initCalc) { initCalcs.push_back(initCalc); }

	// ѕараметры ресурса
	RDOValue getResParamVal( const int res_id, const int param_id ) const {
		RDOResource* res = getResourceByID( res_id );
		return res->params.at( param_id );
	}
	void setResParamVal( const int res_id, const int param_id, RDOValue val ) {
		RDOResource* res = getResourceByID( res_id );
		if ( res->params.size() <= param_id ) {
			res->params.resize( param_id + 1 );
		}
		res->params.at( param_id ) = val;
	}

	// –елевантные ресурсы
	int getResByRelRes( const int rel_res_id ) const { return currActivity->getResByRelRes( rel_res_id ); }
	void setRelRes( int rel_res_id, int res_id )     { currActivity->setRelRes( rel_res_id, res_id );     }

	void onEraseRes( const int res_id, const RDOCalcEraseRes* calc );
	RDOResource* createNewResource();
	RDOResource* createNewResource( int number, bool isPermanent );
	void insertNewResource( RDOResource* res );
	RDORuntime();
	~RDORuntime();

	RDOValue getFuncArgument(int numberOfParam); 
	RDOResource* getGroupFuncRes()         { return groupFuncStack.back();                         }
	void pushFuncArgument( RDOValue arg )  { funcStack.push_back( arg );                           }
	void pushGroupFunc( RDOResource* res ) { groupFuncStack.push_back( res );                      }
	void popFuncArgument()                 { funcStack.pop_back();                                 }
	void popGroupFunc()                    { groupFuncStack.pop_back();                            }
	void pushFuncTop()                     { funcStack.push_back( currFuncTop );                   }
	void resetFuncTop( int numArg )        { currFuncTop = funcStack.size() - numArg;              }
	void popFuncTop()                      { currFuncTop = funcStack.back(); funcStack.pop_back(); }

	virtual bool endCondition();
	bool setTerminateIf( RDOCalc* _terminateIfCalc );
	RDOResource* getResourceByID( const int num ) const { return num >= 0 ? allResourcesByID.at( num ) : NULL; }

	void setPatternParameter(int parNumb, RDOValue val) 
	{ 
		if(patternParameters.size() <= parNumb) 
			patternParameters.resize(parNumb + 1);

		patternParameters.at(parNumb) = val;
	}
	RDOValue getPatternParameter(int parNumb) 
	{ 
		return patternParameters.at(parNumb);
	}
	std::string writeActivitiesStructure( int& counter );
	std::string writePokazStructure();

  	TracerCallBack tracerCallBack;
	FrameCallBack  frameCallBack;
	void* param;		// this param send back to tracerCallBack and frameCallBack
	RDOConfig config;
	std::vector< RDOFRMFrame* > allFrames;

	virtual void onPutToTreeNode();

	rdoSimulator::RDOExitCode whyStop;
	virtual void onNothingMoreToDo() { whyStop = rdoSimulator::EC_NoMoreEvents; }
	virtual void onEndCondition()    { whyStop = rdoSimulator::EC_OK;           }
	virtual void onRuntimeError()    { whyStop = rdoSimulator::EC_RunTimeError; }
	virtual void onUserBreak()       { whyStop = rdoSimulator::EC_UserBreak;    }

	virtual void postProcess();

	RDOTrace* getTracer();
};

} // namespace rdoRuntime

#endif // RDO_RUNTIME_H
