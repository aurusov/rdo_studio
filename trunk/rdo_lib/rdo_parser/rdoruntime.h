#ifndef RDO_RUNTIME
#define RDO_RUNTIME

#include "rdocalcconst.h"
#include "rdopat.h"
#include "rdopatrtime.h"

#include <rdotrace.h>
#include <rdocommon.h>

namespace rdoRuntime
{
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

} // namespace rdoRuntime

class RandGenerator;
class RandGeneratorUniform;
class RandGeneratorExponential;
class RandGeneratorNormal;
class RandGeneratorByHistCommon;

namespace rdoRuntime {
class RDOCalc;
class RDOCalcEraseRes;
}

namespace rdoParse {
class RDOPATChoice;
class RDOPATSelectType;
class RDOPMDWatchQuant;
class RDOPMDWatchValue;
class RDOPMDWatchPar;
class RDOPMDPokaz;
class RDOPMDWatchState;
class RDOFRMFrame;
class RDORelevantResource;
class RDOFUNArithm;
class RDOPMDWatchQuant;
class RDOPMDWatchValue;
class RDOPMDWatchPar;	  
class RDOPMDWatchState;
void addCalcToRuntime( rdoRuntime::RDOCalc* calc );
void removeCalcToRuntime( rdoRuntime::RDOCalc* calc );
}

namespace rdoRuntime
{

class RDOResult
{
private:
	std::ofstream out;
	virtual std::ostream& getOStream() { return out; }

protected:
	bool isNullResult;

public:
	RDOResult(const char *const fileName): out(fileName, std::ios::out), isNullResult(false) {}
	RDOResult(): isNullResult(true) {}
	int width(int w) { if(isNullResult) return 0; return getOStream().width(w); }
	template<class TN>
		RDOResult& operator << (TN str) { if(!isNullResult) getOStream() << str; return *this; }

	virtual ~RDOResult() {}
};

class RDOCalcSeqNextNormal;
class RDOCalcSeqNextByHist;

class RDOActivityRuleRuntime;
class RDOActivityIERuntime;
class RDOActivityOperationRuntime;
class RDOPatternRuntime;
class RDOSearchRuntime;
class RDOPROCProcess;

template <class T> class compareNumber
{
public:
	const int num;
	compareNumber(const int _num): num(_num) {}
	bool operator() (const T * const other) { return (other->number == num); }
};

class RDOFunCalcExist;
class RDOFunCalcNotExist;
class RDOFunCalcForAll;
class RDOFunCalcNotForAll;
class RDOSelectResourceByTypeCalc;
class RDOSelectResourceByTypeCommonCalc;
class RDORuntime;

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
// ---------- RDORuntime
// ----------------------------------------------------------------------------
class RDORuntime: public RDOSimulatorTrace
{
friend class RDOFunCalcSelect;
friend class RDOFunCalcExist;
friend class RDOFunCalcNotExist;
friend class RDOFunCalcForAll;
friend class RDOFunCalcNotForAll;
friend class RDOSelectResourceByTypeCalc;
friend class RDOSelectResourceByTypeCommonCalc;
friend class rdoParse::RDOPMDWatchQuant;
friend class rdoParse::RDOPMDWatchValue;
friend class rdoParse::RDOPMDWatchPar;	  
friend class rdoParse::RDOPMDWatchState;

friend void rdoParse::addCalcToRuntime( RDOCalc* calc );
friend void rdoParse::removeCalcToRuntime( RDOCalc* calc );

private:
	std::vector< RDOResource* >      allResourcesByID;
	std::list  < RDOResource* >      allResourcesByTime;
	std::list  < RDOResourceTrace* > permanentResources;
	RDOTrace* tracer;
	std::list< RDOCalc* > allCalcs;
	std::list< RDOCalc* > initCalcs;

	std::vector< RDOValue >     funcStack;
	std::vector< RDOResource* > groupFuncStack;
	int currFuncTop;
	int savedFuncTop;

	void onInit();
	void onDestroy();
	std::list< RDOResourceTrace* > getPermanentResources();

	std::vector< RDOActivityRuleRuntime* >      rules;
	std::vector< RDOActivityIERuntime* >        ies;
	std::vector< RDOActivityOperationRuntime* > operations;
	std::vector< rdoRuntime::RDOPROCProcess* >  process;
	std::vector< rdoParse::RDOPMDPokaz* >       allPokaz;

	std::vector<RDOPatternRuntime *> allPatterns;

	std::vector<RDOSearchRuntime *> allDPTs;

	RDOActivityRuntime* currActivity;

	std::vector<RDOValue> patternParameters;

	time_t physic_time;
	virtual void preProcess()
	{
		RDOSimulatorTrace::preProcess();
		physic_time = time(NULL);
	}

	RDOResult *result;	// Output class for results (PMV)

	RDOCalc *terminateIfCalc;
	std::vector<RDOValue> allConstants;

	RDOSimulator *clone();
	bool operator == (RDOSimulator &other);

	void writeExitCode();

	bool key_found;
	virtual bool isKeyDown();

protected:
	virtual void onResetPokaz();
	virtual void onCheckPokaz();
	virtual void onAfterCheckPokaz();

public:
//	std::list< int > allResourcesChoiced;

	std::vector< rdoSimulator::RDOSyntaxError > errors;
	void error( const char* message, const rdoRuntime::RDOCalc* calc );

	std::vector< unsigned int > using_scan_codes;
	bool keyDown( unsigned int scan_code );
	void keyUp( unsigned int scan_code );
	bool checkKeyPressed( unsigned int scan_code, bool shift, bool control );
	bool checkAreaActivated( const std::string& oprName );

	void setConstValue(int numberOfConst, RDOValue value);
	RDOValue getConstValue(int numberOfConst);
	RDOResult& getResult() { return *result; }
	void rdoInit(RDOTrace *customTracer, RDOResult *customResult);

	double getTimeNow() { return getCurrentTime(); }
	double getSeconds() { return (time(NULL) - physic_time); }

	void addPattern(RDOPatternRuntime *pat) { allPatterns.push_back(pat); }
	void setCurrentActivity(RDOActivityRuntime *pat) { currActivity = pat; }
	void addRuntimeOperation(RDOActivityOperationRuntime *oper);
	void addRuntimeRule(RDOActivityRuleRuntime *rule);
	void addRuntimeIE(RDOActivityIERuntime *ie);
	void addRuntimeProcess( rdoRuntime::RDOPROCProcess* _process );
	void addRuntimePokaz( rdoParse::RDOPMDPokaz* pok );
	void addRuntimeFrame( rdoParse::RDOFRMFrame* frm );
	rdoParse::RDOFRMFrame* lastFrame() const;

	void addDPT(RDOSearchRuntime *dpt);

	void addInitCalc(RDOCalc *initCalc) { initCalcs.push_back(initCalc); }

	// Параметры ресурса
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

	// Релевантные ресурсы
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

	bool endCondition();
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

	void rdoDelay( double fromTime, double toTime );
  	TracerCallBack tracerCallBack;
	FrameCallBack  frameCallBack;
	void* param;		// this param send back to tracerCallBack and frameCallBack
	RDOConfig config;
	std::vector< rdoParse::RDOFRMFrame* > allFrames;

	virtual void onPutToTreeNode();

	rdoSimulator::RDOExitCode whyStop;
	virtual void onNothingMoreToDo() { whyStop = rdoSimulator::EC_NoMoreEvents; }
	virtual void onEndCondition()    { whyStop = rdoSimulator::EC_OK;           }
	void onRuntimeError()            { whyStop = rdoSimulator::EC_RunTimeError; }
	void onUserBreak()               { whyStop = rdoSimulator::EC_UserBreak;    }

	virtual void postProcess();

	RDOTrace* getTracer();
};
/*
class RDORuntimeException: public RDOException
{
public:
	string getType() const { return "RDO Runtime Exception"; };
	RDORuntimeException(const char *str): RDOException(str) {}
};

class RDOValue
{
public:
	union
	{
		int    intVal;
		double doubleVal;
	} uVal;

	RDOValue( const int i )    { uVal.intVal    = i; }
	RDOValue( const double d ) { uVal.doubleVal = d; }
};
*/

// ----------------------------------------------------------------------------
// ---------- RDOCalcBinary (Binary Ariphmetic calcs)
// ----------------------------------------------------------------------------
class RDOCalcBinary: public RDOCalc
{
protected:
	const RDOCalc* const left;
	const RDOCalc* const right;

public:
	RDOCalcBinary( const RDOCalc* const _left, const RDOCalc* const _right ):
		left( _left ),
		right( _right )
	{
		if ( left && right ) {
			setErrorPos( left->error().first_line, left->error().first_column, right->error().last_line, right->error().last_column );
		}
	}
};

class RDOCalcPlus: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return RDOValue(left->calcValueBase(sim) + right->calcValueBase(sim)); }
	RDOCalcPlus(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcMinus: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return RDOValue(left->calcValueBase(sim) - right->calcValueBase(sim)); }
	RDOCalcMinus(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcMult: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return RDOValue(left->calcValueBase(sim) * right->calcValueBase(sim)); }
	RDOCalcMult(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcDiv: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const;
	RDOCalcDiv(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

// --------------------  Binary Logic calcs ---------------------------------
class RDOCalcAnd: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const {
		if ( !left->calcValueBase( sim )  ) return false;
		if ( !right->calcValueBase( sim ) ) return false;
		return true;
	}
	RDOCalcAnd( RDOCalc* _left, RDOCalc* _right ): RDOCalcBinary( _left, _right ) {};
};

class RDOCalcOr: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const {
		if ( left->calcValueBase( sim )  ) return true;
		if ( right->calcValueBase( sim ) ) return true;
		return false;
	}
	RDOCalcOr( RDOCalc* _left, RDOCalc* _right ): RDOCalcBinary( _left, _right ) {};
};

class RDOCalcNot: public RDOCalc
{
private:
	RDOCalc* calc;

public:
	RDOValue calcValue( RDORuntime* sim ) const {
		return !calc->calcValueBase( sim );
	}
	RDOCalcNot( RDOCalc* _calc ):
		calc( _calc )
	{
		if ( calc ) setErrorPos( calc->error() );
	}
};

class RDOCalcIsEqual: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const {
		return ( left->calcValueBase(sim) == right->calcValueBase(sim) );
	}
	RDOCalcIsEqual( const RDOCalc* const _left, const RDOCalc* const _right): RDOCalcBinary( _left, _right ) {}
};

class RDOCalcIsNotEqual: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const {
		return ( left->calcValueBase(sim) != right->calcValueBase(sim) );
	}
	RDOCalcIsNotEqual( RDOCalc* _left, RDOCalc* _right ): RDOCalcBinary( _left, _right ) {}
};

class RDOCalcIsLess: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return (left->calcValueBase(sim) < right->calcValueBase(sim));	}
	RDOCalcIsLess(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsGreater: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return (left->calcValueBase(sim) > right->calcValueBase(sim));	}
	RDOCalcIsGreater(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsLEQ: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return (left->calcValueBase(sim) <= right->calcValueBase(sim));	}
	RDOCalcIsLEQ(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsGEQ: public RDOCalcBinary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return (left->calcValueBase(sim) >= right->calcValueBase(sim));	}
	RDOCalcIsGEQ(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

// --------------------  Unary Ariphmetic calcs ---------------------------------
class RDOCalcUnary: public RDOCalc
{
protected:
	RDOCalc* oper;
	RDOCalcUnary( RDOCalc* _oper ):
		oper( _oper )
	{
		if ( oper ) setErrorPos( oper->error() );
	}
};

class RDOCalcUMinus: public RDOCalcUnary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return RDOValue(-oper->calcValueBase(sim)); }
	RDOCalcUMinus( RDOCalc* _oper ): RDOCalcUnary(_oper) {}
};

class RDOCalcDoubleToInt: public RDOCalcUnary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return RDOValue((int)oper->calcValueBase(sim)); }
	RDOCalcDoubleToInt( RDOCalc* _oper ): RDOCalcUnary(_oper) {}
};

class RDOCalcCheckDiap: public RDOCalcUnary
{
private:
	RDOValue minVal, maxVal;

public:
	RDOCalcCheckDiap( RDOValue _minVal, RDOValue _maxVal, RDOCalc* _oper ):
		minVal( _minVal ),
		maxVal( _maxVal ),
		RDOCalcUnary( _oper )
	{
	}
	RDOValue calcValue( RDORuntime* sim ) const;
};


// --------------------  Resource Param Ariphmetic calcs ---------------------------------
/*
class RDOCalcResParamAccess: public RDOCalc
{
public:
	int nRes;
	int nParam;
};

class RDOCalcResourceParam: public RDOCalcResParamAccess
{
public:
	RDOValue calcValue( RDORuntime* sim ) const { return RDOValue(sim->getResParamVal(nRes, nParam)); }
};
*/

// -------------------- System calcs --------------------------------
// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateNumberedResource (создание нового временного ресурса по индексу с параметрами)
// ----------------------------------------------------------------------------
class RDOCalcCreateNumberedResource: public RDOCalc
{
private:
	int  type;
	bool traceFlag;
	std::vector< RDOValue > paramsCalcs;
	int  number;
	bool isPermanent;

public:
	RDOCalcCreateNumberedResource( int _type, bool _traceFlag, const std::vector< RDOValue >& _paramsCalcs, int _number, bool _isPermanent );
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOCalcCreateEmptyResource (создание нового временного ресурса с пустым списком параметров)
// ----------------------------------------------------------------------------
class RDOCalcCreateEmptyResource: public RDOCalc
{
private:
	int  type;
	bool traceFlag;
	int  rel_res_id;
	int  numParameters;

public:
	RDOCalcCreateEmptyResource( int _type, bool _traceFlag, int _rel_res_id, int _numParameters );
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOCalcFuncParam: public RDOCalc
{
private:
	int numberOfParam;

public:
	RDOCalcFuncParam(int _numberOfParam): numberOfParam(_numberOfParam)	{}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalc: public RDOCalc
{
};

class RDOFuncTableCalc: public RDOFunCalc
{
private:
	std::vector< RDOCalcConst* > results;
	RDOCalc* argCalc;

public:
	RDOFuncTableCalc( RDOCalc* _argCalc ): argCalc( _argCalc ) {}
	void addResultCalc( RDOCalcConst* res ) { results.push_back(res); }
	virtual RDOValue calcValue( RDORuntime* sim ) const
	{
		int index = argCalc->calcValueBase(sim);
		return results.at(index)->calcValueBase(sim);
	}
};

class RDOFunListCalc: public RDOFunCalc
{
private:
	std::vector< RDOCalc* > cases;
	std::vector< RDOCalcConst* > results;
	RDOCalcConst* defaultValue;

public:
	RDOFunListCalc( RDOCalcConst* _defaultValue ): defaultValue( _defaultValue ) {}

	void addCase( RDOCalc* _caseCalc, RDOCalcConst* _resultCalc )
	{
		cases.push_back(_caseCalc); 
		results.push_back(_resultCalc);
	}

	virtual RDOValue calcValue( RDORuntime* sim ) const
	{
		int size = cases.size();
		for(int i = 0; i < size; i++) {
			RDOCalc* cas = cases[i];
			if ( cas->calcValueBase(sim) ) return results[i]->calcValueBase(sim);
		}
		return defaultValue->calcValueBase(sim);
	}
};

class RDOCalcGetResParam: public RDOCalc
{
private:
	int resNumb;
	int parNumb;

public:
	RDOCalcGetResParam( int _resNumb, int _parNumb ): resNumb( _resNumb ), parNumb( _parNumb ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return sim->getResParamVal( resNumb, parNumb );
	}
};

class RDOCalcFunctionCall: public RDOCalc
{
private:
	std::vector< const RDOCalc* > parameters;
	const RDOFunCalc* const function;

public:
	RDOCalcFunctionCall( const RDOFunCalc* const _function ): function( _function ) {}

	void addParameter(const RDOCalc *calc) { parameters.push_back(calc); }
	virtual RDOValue calcValue( RDORuntime* sim ) const
	{
		sim->pushFuncTop();
		int size = parameters.size();
		for(int i = 0; i < size; i++)
		{
			RDOValue arg = parameters[i]->calcValueBase(sim);
			sim->pushFuncArgument(arg);
		}
		sim->resetFuncTop(parameters.size());
		RDOValue val = function->calcValueBase(sim);
		size = parameters.size();
		for(i = 0; i < size; i++)
			sim->popFuncArgument();

		sim->popFuncTop();
		return val;
	}
};

class RDOFunAlgorithmicCalc: public RDOFunCalc
{
	std::vector<RDOCalc *> conditions;
	std::vector<RDOCalc *> actions;
public:
	void addCalcIf(RDOCalc *cond, RDOCalc *act)
	{
		conditions.push_back(cond);
		actions.push_back(act);
	}

   virtual RDOValue calcValue( RDORuntime* sim ) const
	{
		int size = conditions.size();
		for(int i = 0; i < size; i++)
		{
			if(conditions[i]->calcValueBase(sim))
				return actions[i]->calcValueBase(sim);
		}
		return 0;
	}
};

class RDOCalcGetGroupResParam: public RDOCalc
{
	int parNumb;
public:
	RDOCalcGetGroupResParam(int _parNumb): parNumb(_parNumb) {}
   virtual RDOValue calcValue( RDORuntime* sim ) const
	{
		RDOResource *currRes = (RDOResource *)sim->getGroupFuncRes();
	   return currRes->params[parNumb];
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcGroup
// ----------------------------------------------------------------------------
class RDOFunCalcGroup: public RDOFunCalc
{
protected:
	int      nResType;
	RDOCalc* condition;

	RDOFunCalcGroup( int _nResType, RDOCalc* _condition ):
		RDOFunCalc(),
		nResType( _nResType ),
		condition( _condition )
	{
	}
};

class RDOFunCalcExist: public RDOFunCalcGroup
{
public:
	RDOFunCalcExist( int _nResType, RDOCalc* _condition): RDOFunCalcGroup( _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalcNotExist: public RDOFunCalcGroup
{
public:
	RDOFunCalcNotExist( int _nResType, RDOCalc* _condition ): RDOFunCalcGroup( _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalcForAll: public RDOFunCalcGroup
{
public:
	RDOFunCalcForAll( int _nResType, RDOCalc* _condition ): RDOFunCalcGroup( _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalcNotForAll: public RDOFunCalcGroup
{
public:
	RDOFunCalcNotForAll( int _nResType, RDOCalc* _condition ): RDOFunCalcGroup( _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFunCalcSelect
// ----------------------------------------------------------------------------
class RDOFunCalcSelect: public RDOFunCalcGroup
{
public:
	mutable std::list< RDOResource* > res_list;
	void prepare( RDORuntime* sim ) const;

	RDOFunCalcSelect( int _nResType, RDOCalc* _condition ): RDOFunCalcGroup( _nResType, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalcSelectBase: public RDOFunCalc
{
protected:
	RDOFunCalcSelect* select;
	RDOCalc*          condition;

public:
	RDOFunCalcSelectBase( RDOFunCalcSelect* _select, RDOCalc* _condition ):
		RDOFunCalc(),
		select( _select ),
		condition( _condition )
	{
	}
};

class RDOFunCalcSelectSize: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectSize( RDOFunCalcSelect* _select ): RDOFunCalcSelectBase( _select, NULL ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalcSelectExist: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectExist( RDOFunCalcSelect* _select, RDOCalc* _condition ): RDOFunCalcSelectBase( _select, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalcSelectNotExist: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectNotExist( RDOFunCalcSelect* _select, RDOCalc* _condition ): RDOFunCalcSelectBase( _select, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalcSelectForAll: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectForAll( RDOFunCalcSelect* _select, RDOCalc* _condition ): RDOFunCalcSelectBase( _select, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOFunCalcSelectNotForAll: public RDOFunCalcSelectBase
{
public:
	RDOFunCalcSelectNotForAll( RDOFunCalcSelect* _select, RDOCalc* _condition ): RDOFunCalcSelectBase( _select, _condition ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

// ----------------------------------------------------------------------------
// ---------- Native functions
// ----------------------------------------------------------------------------
class RDOFunCalcAbs: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(fabs(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcArcCos: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(acos(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcArcSin: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(asin(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcArcTan: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(atan(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcCos: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(cos(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcCotan: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(1./tan(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcExp: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(exp(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcFloor: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(floor(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcFrac: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		double tmp;
		return RDOValue(modf(sim->getFuncArgument(0), &tmp));
	}
};

class RDOFunCalcIAbs: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(abs(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcIMax: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue((int)std::_MAX(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcIMin: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue((int)std::_MIN(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcInt: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(int(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcIntPower: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(pow(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcLn: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(log(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcLog10: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(log10(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcLog2: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(log(sim->getFuncArgument(0)) / log(2) );
	}
};

class RDOFunCalcLogN: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(log(sim->getFuncArgument(0)) / log(sim->getFuncArgument(1)));
	}
};

class RDOFunCalcMax: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(std::_MAX(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcMin: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(std::_MIN(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcPower: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(pow(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcRound: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(floor(sim->getFuncArgument(0) + 0.5));
	}
};

class RDOFunCalcSin: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(sin(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcSqrt: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(sqrt(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcTan: public RDOFunCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return RDOValue(tan(sim->getFuncArgument(0)));
	}
};

class RDOCalcSeqInit: public RDOCalc
{
private:
	int            base;
	RandGenerator* gen;

public:
	RDOCalcSeqInit( int _base, RandGenerator* _gen ):
		base( _base ),
		gen( _gen )
	{
	}
	~RDOCalcSeqInit();
	virtual RDOValue calcValue( RDORuntime* sim ) const;
	void setBase( int _base ) { base = _base; }
};

class RDOCalcSeqNext: public RDOFunCalc
{
public:
	bool     res_real;
	bool     diap;
	RDOValue diap_min;
	RDOValue diap_max;

	RDOCalcSeqNext():
		RDOFunCalc(),
		res_real( true ),
		diap( false ),
		diap_min( 0 ),
		diap_max( 0 )
	{
	}
};

class RDOCalcSeqNextUniform: public RDOCalcSeqNext
{
private:
	RandGeneratorUniform* gen;

public:
	RDOCalcSeqNextUniform( RandGeneratorUniform* _gen ):
		RDOCalcSeqNext(),
		gen( _gen )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOCalcSeqNextExponential: public RDOCalcSeqNext
{
private:
	RandGeneratorExponential* gen;

public:
	RDOCalcSeqNextExponential( RandGeneratorExponential* _gen ):
		RDOCalcSeqNext(),
		gen( _gen )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOCalcSeqNextNormal: public RDOCalcSeqNext
{
private:
	RandGeneratorNormal* gen;

public:
	RDOCalcSeqNextNormal( RandGeneratorNormal* _gen ):
		RDOCalcSeqNext(),
		gen( _gen )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOCalcSeqNextByHist: public RDOCalcSeqNext
{
private:
	RandGeneratorByHistCommon* gen;

public:
	RDOCalcSeqNextByHist( RandGeneratorByHistCommon* _gen ):
		RDOCalcSeqNext(),
		gen( _gen )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOCalcGetRelevantResParam: public RDOCalc
{
private:
	int relNumb;
	int parNumb;

public:
	RDOCalcGetRelevantResParam( int _relNumb, int _parNumb ): relNumb( _relNumb ), parNumb( _parNumb ) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		return sim->getResParamVal( sim->getResByRelRes(relNumb), parNumb );
	}
};
/*
class RDOCalcRelResConvert: public RDOCalc
{
private:
	int relNumb;

public:
	RDOCalcEraseRes( int _relNumb, const std::string& _relName ):
		relNumb( _relNumb ),
		relName( _relName )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		if ( first_call ) {
			sim->onEraseResBefore( sim->getResByRelRes(relNumb), this );
			first_call = false;
		} else {
			sim->onEraseRes( sim->getResByRelRes(relNumb), this );
			first_call = true;
		}
		return 1;
	}
	std::string getName() const { return relName; }
};
*/

class RDOCalcEraseRes: public RDOCalc
{
private:
	int          rel_res_id;
	std::string  rel_res_name;

public:
	RDOCalcEraseRes( int _rel_res_id, const std::string& _rel_res_name ):
		rel_res_id( _rel_res_id ),
		rel_res_name( _rel_res_name )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		sim->onEraseRes( sim->getResByRelRes(rel_res_id), this );
		return 1;
	}
	std::string getName() const { return rel_res_name; }
};

class RDOSelectResourceCalc: public RDOCalc
{
protected:
	int                              rel_res_id;
	RDOCalc*                         choice;
	RDOCalc*                         selection_calc;
	rdoParse::RDOPATSelectType::Type selection_type;
	RDOSelectResourceCalc( int _rel_res_id, rdoParse::RDOPATChoice* _choice, rdoParse::RDOPATSelectType* _selection_type );
};

class RDOSelectResourceNonExistCalc: public RDOSelectResourceCalc
{
public:
	RDOSelectResourceNonExistCalc( int _rel_res_id ):
		RDOSelectResourceCalc( _rel_res_id, NULL, NULL )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const {
		sim->setRelRes( rel_res_id, -1 );
		return 1;
	}
};

class RDOSelectResourceDirectCalc: public RDOSelectResourceCalc
{
protected:
	int res_id;

public:
	RDOSelectResourceDirectCalc( int _rel_res_id, int _res_id, rdoParse::RDOPATChoice* _choice, rdoParse::RDOPATSelectType* _selection_type ):
		RDOSelectResourceCalc( _rel_res_id, _choice, _selection_type ),
		res_id( _res_id )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOSelectResourceByTypeCalc: public RDOSelectResourceCalc
{
protected:
	int resType;

public:
	RDOSelectResourceByTypeCalc( int _rel_res_id, int _resType, rdoParse::RDOPATChoice* _choice, rdoParse::RDOPATSelectType* _selection_type ):
		RDOSelectResourceCalc( _rel_res_id, _choice, _selection_type ),
		resType( _resType )
	{
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOSelectResourceCommon
{
public:
	virtual std::vector<int> getPossibleNumbers( RDORuntime* sim ) const = 0;
	virtual bool callChoice( RDORuntime* sim ) const = 0;
};

class RDOSelectResourceDirectCommonCalc: public RDOSelectResourceDirectCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceDirectCommonCalc( int _relNumb, int _resNumb, rdoParse::RDOPATChoice *_choice, rdoParse::RDOPATSelectType* _selection_type ):
		RDOSelectResourceDirectCalc( _relNumb, _resNumb, _choice, _selection_type ) {}
	std::vector<int> getPossibleNumbers( RDORuntime* sim ) const;
	virtual bool callChoice( RDORuntime* sim ) const;
};

class RDOSelectResourceByTypeCommonCalc: public RDOSelectResourceByTypeCalc, public RDOSelectResourceCommon
{
public:
	RDOSelectResourceByTypeCommonCalc( int _relNumb, int _resType, rdoParse::RDOPATChoice *_choice, rdoParse::RDOPATSelectType* _selection_type ):
		RDOSelectResourceByTypeCalc( _relNumb, _resType, _choice, _selection_type ) {}
	std::vector<int> getPossibleNumbers( RDORuntime* sim ) const;
	virtual bool callChoice( RDORuntime* sim ) const;
};

class RDOSelectResourceCommonCalc: public RDOCalc
{
private:
	RDOCalc* choice;
	std::vector<RDOSelectResourceCommon *> resSelectors;
	bool useCommonWithMax;
	void getBest(std::vector<std::vector<int> > &allNumbs, int level, std::vector<int> &res, RDOValue &bestVal, RDORuntime *sim, bool &hasBest) const;

public:
	RDOSelectResourceCommonCalc( const std::vector< RDOSelectResourceCommon* >& _resSelectors, bool _useCommonWithMax, RDOCalc* _choice ):
		resSelectors( _resSelectors ),
		useCommonWithMax( _useCommonWithMax ),
		choice( _choice )
	{
		if ( choice ) setErrorPos( choice->error() );
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};


class RDOSetRelParamCalc: public RDOCalc
{
private:
	int relNumb;
	int parNumb;
	RDOCalc* calc;

public:
	RDOSetRelParamCalc( int _relNumb, int _parNumb, RDOCalc* _calc ):
		relNumb( _relNumb ),
		parNumb( _parNumb ),
		calc( _calc )
	{
		if ( calc ) setErrorPos( calc->error() );
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOSetResourceParamCalc: public RDOCalc
{
private:
	int resNumb;
	int parNumb;
	RDOCalc* calc;

public:
	RDOSetResourceParamCalc( int _resNumb, int _parNumb, RDOCalc* _calc ):
		resNumb( _resNumb ),
		parNumb( _parNumb ),
		calc( _calc )
	{
		if ( calc ) setErrorPos( calc->error() );
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOSetPatternParamCalc: public RDOCalc
{
private:
	int parNumb;
	RDOValue val;

public:
	RDOSetPatternParamCalc(int _parNumb, RDOValue _val):
		parNumb(_parNumb), val(_val) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const { sim->setPatternParameter(parNumb, val); return 0; }
};

class RDOCalcPatParam: public RDOCalc
{
private:
	int numberOfParam;

public:
	RDOCalcPatParam(int _numberOfParam): numberOfParam(_numberOfParam)	{}
	virtual RDOValue calcValue( RDORuntime* sim ) const;
};

class RDOCalcGetTimeNow: public RDOCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const { return sim->getTimeNow(); } 
};

class RDOCalcGetSeconds: public RDOCalc
{
public:
	virtual RDOValue calcValue( RDORuntime* sim ) const { return sim->getSeconds(); } 
};

class RDOCalcGetConst: public RDOCalc
{
private:
	int number;

public:
	RDOCalcGetConst( int _number ): number(_number) {}
	virtual RDOValue calcValue( RDORuntime* sim ) const { return sim->getConstValue(number); } 
};

class RDOCalcSetConst: public RDOCalc
{
private:
	int number;
	RDOCalc* value;

public:
	RDOCalcSetConst(int _number, RDOCalc *_value):
		number( _number ),
		value( _value )
	{
		if ( value ) setErrorPos( value->error() );
	}
	virtual RDOValue calcValue( RDORuntime* sim ) const { sim->setConstValue(number, value->calcValueBase(sim)); return 0; } 
};

class RDOCalcInt: public RDOCalcUnary
{
public:
	RDOValue calcValue( RDORuntime* sim ) const {
		RDOValue res = oper->calcValueBase(sim);
		return static_cast<int>(res > 0 ? res + 0.5 : res - 0.5);
	}
	RDOCalcInt( RDOCalc* oper ): RDOCalcUnary( oper ) {}
};

} // namespace rdoRuntime

#endif // RDO_RUNTIME
