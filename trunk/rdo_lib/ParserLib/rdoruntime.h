#ifndef RDO_RUNTIME
#define RDO_RUNTIME


#include "rdocalcconst.h"

#include "rdotrace.h"
#include "rdosimcommon.h"

namespace rdoRuntime
{
struct RDOConfig
{
//////// Interactive /////////////////
	RDOSimulatorNS::ShowMode showAnimation;
	vector<string> allFrameNames;
	int currFrameToShow;
	vector<int> keysPressed;
	bool mouseClicked;
	vector<string> activeAreasMouseClicked;
	
//////// Frame /////////////////////
	vector<RDOSimulatorNS::RDOFrame *> frames;

//////// Timing ///////////////////
	double currTime;			// model time
	double newTime;			// model time
	double showRate;
	double realTimeDelay;	// msec
};

typedef void (*TracerCallBack) (string *newString, void *param);
typedef void (*FrameCallBack) (RDOConfig *config, void *param);

} //namespace rdoRuntime

class RandGenerator;
class RandGeneratorUniform;
class RandGeneratorExponential;
class RandGeneratorNormal;
class RandGeneratorByHistCommon;

namespace rdoRuntime {
class RDOCalc;
}

namespace rdoParse {
class RDOPATChoice;
class RDOPATFirst;
void addCalcToRuntime(rdoRuntime::RDOCalc *calc);
class RDOPMDWatchQuant;
class RDOPMDWatchValue;
class RDOPMDWatchPar;
class RDOPMDPokaz;
class RDOPMDWatchState;
class RDOFRMFrame;
}

using namespace rdoParse;

namespace rdoRuntime
{

class RDOResult
{
   ofstream out;
   virtual std::ostream &getOStream() { return out; }
protected:
	bool isNullResult;
public:
	RDOResult(const char *const fileName): out(fileName, ios::out), isNullResult(false) {}
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
class RDOActivityRuntime;
class RDOSearchRuntime;

template <class T> 
struct compareNumber
{
	const int num;
	compareNumber(const int _num): num(_num) {}
	bool operator() (const T * const other) { return (other->number == num); }
};

class RDOResource;
class RDOFunCalcExist;
class RDOFunCalcNotExist;
class RDOFunCalcForAll;
class RDOFunCalcNotForAll;
class RDOSelectResourceByTypeCalc;

class RDORuntime: public RDOSimulatorTrace
{
friend RDOFunCalcExist;
friend RDOFunCalcNotExist;
friend RDOFunCalcForAll;
friend RDOFunCalcNotForAll;
friend RDOSelectResourceByTypeCalc;
friend RDOPMDWatchQuant;
friend RDOPMDWatchValue;
friend RDOPMDWatchPar;	  
friend RDOPMDWatchState;

	friend void rdoParse::addCalcToRuntime(RDOCalc *calc);

   vector<RDOResource *> allResources;
   vector<RDOResourceTrace *> permanentResources;
   RDOTrace *tracer;
	list<RDOCalc *>		allCalcs;
	list<RDOCalc *>		initCalcs;

   vector<RDOValue>	funcStack;
   vector<RDOResource *>	groupFuncStack;
	int currFuncTop;
	int savedFuncTop;

   RDOTrace *getTracer();
   void onInit();
   void onDestroy();
	vector<RDOResourceTrace *> getPermanentResources();

	vector<RDOActivityRuleRuntime *> rules;
	vector<RDOActivityIERuntime *> ies;
	vector<RDOActivityOperationRuntime *> operations;
	vector<RDOPMDPokaz *> allPokaz;

	vector<RDOPatternRuntime *> allPatterns;

	vector<RDOSearchRuntime *> allDPTs;

	RDOActivityRuntime *currActivity;

	vector<RDOValue> patternParameters;

	time_t physic_time;
	virtual void preProcess()
	{
		RDOSimulatorTrace::preProcess();
		physic_time = time(NULL);
	}

	RDOResult *result;	// Output class for results (PMV)

	RDOCalc *terminateIfCalc;
   vector<RDOValue>	allConstants;

   RDOSimulator *clone();
   bool operator == (RDOSimulator &other);

public:
	vector<RDOSimulatorNS::RDOSyntaxError> errors;
	void error( const char *mes, const rdoRuntime::RDOCalc *calc );
	bool checkKeyPressed(int scanCode);
	bool checkAreaActivated(string *oprName);
	void setConstValue(int numberOfConst, RDOValue value);
	RDOValue getConstValue(int numberOfConst);
	RDOResult& getResult() { return *result; }
   void rdoInit(RDOTrace *customTracer, RDOResult *customResult);

	double getTimeNow() { return getCurrentTime(); }
	double getSeconds() { return (time(NULL) - physic_time); }

	void addPattern(RDOPatternRuntime *pat) { allPatterns.push_back(pat); }
	void setCurrentActivity(RDOActivityRuntime *pat) { currActivity = pat; }
	void addRuntimeOperation(RDOActivityOperationRuntime *oper) { operations.push_back(oper); }
	void addRuntimeRule(RDOActivityRuleRuntime *rule) { rules.push_back(rule); }
	void addRuntimeIE(RDOActivityIERuntime *ie) { ies.push_back(ie); }
	void addRuntimePokaz(RDOPMDPokaz *pok);
	void addRuntimeFrame(RDOFRMFrame *frm);

	void addDPT(RDOSearchRuntime *dpt) { allDPTs.push_back(dpt); }

	void addInitCalc(RDOCalc *initCalc) { initCalcs.push_back(initCalc); }
   RDOValue getResParamVal(const int nRes, const int nParam) const;
   void setResParamVal(const int nRes, const int nParam, RDOValue val);
   int getRelResNumber(const int nRelRes) const;
	RDOValue eraseRes(const int resNumb);
	RDOResource *createNewResource();
	RDOResource *createNewResource(int number, bool isPermanent);
	RDORuntime();
	~RDORuntime();

	RDOValue getFuncArgument(int numberOfParam); 
	RDOResource * getGroupFuncRes() { return groupFuncStack.back(); }
	void pushFuncArgument(RDOValue arg) { funcStack.push_back(arg); }
	void pushGroupFunc(RDOResource *res) { groupFuncStack.push_back(res); }
	void popFuncArgument() { funcStack.pop_back(); }
	void popGroupFunc() { groupFuncStack.pop_back(); }
	void pushFuncTop() { funcStack.push_back(currFuncTop); }
	void resetFuncTop(int numArg) { currFuncTop = funcStack.size() - numArg; }
	void popFuncTop() { currFuncTop = funcStack.back(); funcStack.pop_back(); }

   bool endCondition();
   bool setTerminateIf(RDOCalc *_terminateIfCalc);
   RDOResource *findResource(const int num) const;
	void selectRelResource(int relNumb, int resNumb);

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
	string writeActivitiesStructure();
	string writePokazStructure();

   void rdoDelay(double fromTime, double toTime);
  	TracerCallBack tracerCallBack;
	FrameCallBack frameCallBack;
	void *param;		// this param send back to tracerCallBack and frameCallBack
	RDOConfig config;
	vector<RDOFRMFrame *> allFrames;
};
/*
struct RDORuntimeException: public RDOException
{
   string getType() const { return "RDO Runtime Exception"; };
   RDORuntimeException(const char *str): RDOException(str) {}
};
*/
/*
struct RDOValue
{
   union 
	{
      int      intVal;
      double   doubleVal;
   }	uVal;

   RDOValue(const int i) {	uVal.intVal = i; }
   RDOValue(const double d) { uVal.doubleVal = d; }
};
*/

class RDOResource: public RDOResourceTrace
{
public:
   int number; // unique for all resources alive in system
	int type;
   vector<RDOValue> params;
   string getTypeId();
   string traceParametersValue();
	RDOResource(RDORuntime *rt): RDOResourceTrace(rt) {}
   bool operator != (RDOResource &other);
};
/*
class RDOCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const = 0;
   virtual ~RDOCalc() {}
   RDOCalc();
};

class RDOCalcConst: public RDOCalc
{
   RDOValue constanta;
public:
	RDOCalcConst(RDOValue val) : constanta(val) {}
   RDOValue calcValue(RDORuntime *sim) const { return constanta; }
};
*/

// --------------------  Binary Ariphmetic calcs ---------------------------------
class RDOCalcBinary: public RDOCalc
{
protected:
   const RDOCalc *const left;
   const RDOCalc *const right;

public:
	RDOCalcBinary(const RDOCalc *const _left, const RDOCalc *const _right): left(_left), right(_right) {}
};

class RDOCalcPlus: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const { return RDOValue(left->calcValue(sim) + right->calcValue(sim)); }
	RDOCalcPlus(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcMinus: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const { return RDOValue(left->calcValue(sim) - right->calcValue(sim)); }
	RDOCalcMinus(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcMult: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const { return RDOValue(left->calcValue(sim) * right->calcValue(sim)); }
	RDOCalcMult(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcDiv: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const;
	RDOCalcDiv(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};


// --------------------  Binary Logic calcs ---------------------------------


class RDOCalcAnd: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const 
	{
		if(!left->calcValue(sim))
			return false;
		if(!right->calcValue(sim))
			return false;

		return true;
	}
	RDOCalcAnd(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcOr: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const 
	{
		if(left->calcValue(sim))
			return true;
		if(right->calcValue(sim))
			return true;

		return false;
	}
	RDOCalcOr(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsEqual: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const { return (left->calcValue(sim) == right->calcValue(sim)); }
	RDOCalcIsEqual(const RDOCalc *const _left, const RDOCalc *const _right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsNotEqual: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const {	return (left->calcValue(sim) != right->calcValue(sim));	}
	RDOCalcIsNotEqual(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsLess: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const {	return (left->calcValue(sim) < right->calcValue(sim));	}
	RDOCalcIsLess(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsGreater: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const {	return (left->calcValue(sim) > right->calcValue(sim));	}
	RDOCalcIsGreater(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsLEQ: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const {	return (left->calcValue(sim) <= right->calcValue(sim));	}
	RDOCalcIsLEQ(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};

class RDOCalcIsGEQ: public RDOCalcBinary
{
public:
   RDOValue calcValue(RDORuntime *sim) const {	return (left->calcValue(sim) >= right->calcValue(sim));	}
	RDOCalcIsGEQ(RDOCalc *_left, RDOCalc *_right): RDOCalcBinary(_left, _right) {}
};


// --------------------  Unary Ariphmetic calcs ---------------------------------
class RDOCalcUnary: public RDOCalc
{
protected:
   RDOCalc *oper;
	RDOCalcUnary(RDOCalc *_oper): oper(_oper) {}
};

class RDOCalcUMinus: public RDOCalcUnary
{
public:
   RDOValue calcValue(RDORuntime *sim) const { return RDOValue(-oper->calcValue(sim)); }
};

class RDOCalcDoubleToInt: public RDOCalcUnary
{
public:
   RDOValue calcValue(RDORuntime *sim) const { return RDOValue((int)oper->calcValue(sim)); }
	RDOCalcDoubleToInt(RDOCalc *_oper): RDOCalcUnary(_oper) {}
};

class RDOCalcCheckDiap: public RDOCalcUnary
{
   RDOValue minVal, maxVal;
public:
	RDOCalcCheckDiap(RDOValue _minVal, RDOValue _maxVal, RDOCalc *_oper): minVal(_minVal), maxVal(_maxVal), RDOCalcUnary(_oper) {}
   RDOValue calcValue(RDORuntime *sim) const;
};


// --------------------  Resource Param Ariphmetic calcs ---------------------------------
class RDOCalcResParamAccess: public RDOCalc
{
public:
   int nRes;
   int nParam;
};

class RDOCalcResourceParam: public RDOCalcResParamAccess
{
public:
   RDOValue calcValue(RDORuntime *sim) const { return RDOValue(sim->getResParamVal(nRes, nParam)); }
};


// -------------------- System calcs --------------------------------

class RDOCalcCreateResource: public RDOCalc
{
	int type;
	bool traceFlag;
   vector<RDOValue> paramsCalcs;
public:
	RDOCalcCreateResource(int _type, bool _traceFlag, const vector<RDOValue> &_paramsCalcs);
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOCalcCreateNumberedResource: public RDOCalc
{
	int type;
	bool traceFlag;
   vector<RDOValue> paramsCalcs;
	int number;
	bool isPermanent;
public:
	RDOCalcCreateNumberedResource(int _type, bool _traceFlag, const vector<RDOValue> &_paramsCalcs, int _number, bool _isPermanent);
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOCalcCreateEmptyResource: public RDOCalc
{
	int type;
	bool traceFlag;
	int relResNumber;
public:
	RDOCalcCreateEmptyResource(int _type, bool _traceFlag, int _relResNumber);
   virtual RDOValue calcValue(RDORuntime *sim) const;
};


class RDOCalcFuncParam: public RDOCalc
{
	int numberOfParam;
public:
	RDOCalcFuncParam(int _numberOfParam): numberOfParam(_numberOfParam)	{}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOFunCalc: public RDOCalc
{
};

class RDOFuncTableCalc: public RDOFunCalc
{
	vector<RDOCalcConst *> results;
	RDOCalc *argCalc;
public:
	RDOFuncTableCalc(RDOCalc *_argCalc):
		argCalc(_argCalc) {}
	void addResultCalc(RDOCalcConst *res)
		{	results.push_back(res); }
   virtual RDOValue calcValue(RDORuntime *sim) const
	{
		int index = argCalc->calcValue(sim);
		return results.at(index)->calcValue(sim);
	}
};

class RDOFunListCalc: public RDOFunCalc
{
	vector<RDOCalc *> cases;
	vector<RDOCalcConst *> results;
	RDOCalcConst *defaultValue;

public:
	RDOFunListCalc(RDOCalcConst *_defaultValue):
		defaultValue(_defaultValue) {}

	void addCase(RDOCalc *_caseCalc, RDOCalcConst *_resultCalc)
	{
		cases.push_back(_caseCalc); 
		results.push_back(_resultCalc);
	}

   virtual RDOValue calcValue(RDORuntime *sim) const
	{
		int size = cases.size();
		for(int i = 0; i < size; i++)
		{
			RDOCalc *cas = cases[i];
			if(cas->calcValue(sim))
				return results[i]->calcValue(sim);
		}

		return defaultValue->calcValue(sim);
	}
};

class RDOCalcGetResParam: public RDOCalc
{
	int resNumb;
	int parNumb;
public:
	RDOCalcGetResParam(int _resNumb, int _parNumb): resNumb(_resNumb), parNumb(_parNumb) {}
   virtual RDOValue calcValue(RDORuntime *sim) const
	{
		return sim->getResParamVal(resNumb, parNumb);
	}
};

class RDOCalcFunctionCall: public RDOCalc
{
	vector<const RDOCalc *> parameters;
	const RDOFunCalc *const function;
public:
	RDOCalcFunctionCall(const RDOFunCalc *const _function): function(_function) {}
	void addParameter(const RDOCalc *calc) { parameters.push_back(calc); }
   virtual RDOValue calcValue(RDORuntime *sim) const
	{
		sim->pushFuncTop();
		int size = parameters.size();
		for(int i = 0; i < size; i++)
		{
			RDOValue arg = parameters[i]->calcValue(sim);
			sim->pushFuncArgument(arg);
		}
		sim->resetFuncTop(parameters.size());
		RDOValue val = function->calcValue(sim);
		size = parameters.size();
		for(i = 0; i < size; i++)
			sim->popFuncArgument();

		sim->popFuncTop();
		return val;
	}
};

class RDOFunAlgorithmicCalc: public RDOFunCalc
{
	vector<RDOCalc *> conditions;
	vector<RDOCalc *> actions;
public:
	void addCalcIf(RDOCalc *cond, RDOCalc *act)
	{
		conditions.push_back(cond);
		actions.push_back(act);
	}

   virtual RDOValue calcValue(RDORuntime *sim) const
	{
		int size = conditions.size();
		for(int i = 0; i < size; i++)
		{
			if(conditions[i]->calcValue(sim))
				return actions[i]->calcValue(sim);
		}
		return 0;
	}
};

class RDOCalcGetGroupResParam: public RDOCalc
{
	int parNumb;
public:
	RDOCalcGetGroupResParam(int _parNumb): parNumb(_parNumb) {}
   virtual RDOValue calcValue(RDORuntime *sim) const
	{
		RDOResource *currRes = (RDOResource *)sim->getGroupFuncRes();
	   return currRes->params[parNumb];
	}
};

// -------------------- Native functions --------------------------------

class RDOFunCalcGroup: public RDOFunCalc
{
protected:
	int nTempResType;
	RDOCalc *condition;
	RDOFunCalcGroup(int _nTempResType, RDOCalc *_condition):
		nTempResType(_nTempResType), condition(_condition)	{}
};

class RDOFunCalcExist: public RDOFunCalcGroup
{
public:
	RDOFunCalcExist(int _nTempResType, RDOCalc *_condition):
		RDOFunCalcGroup(_nTempResType, _condition)	{}
	
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOFunCalcNotExist: public RDOFunCalcGroup
{
public:
	RDOFunCalcNotExist(int _nTempResType, RDOCalc *_condition):
		RDOFunCalcGroup(_nTempResType, _condition)	{}
	
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOFunCalcForAll: public RDOFunCalcGroup
{
public:
	RDOFunCalcForAll(int _nTempResType, RDOCalc *_condition):
		RDOFunCalcGroup(_nTempResType, _condition)	{}
	
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOFunCalcNotForAll: public RDOFunCalcGroup
{
public:
	RDOFunCalcNotForAll(int _nTempResType, RDOCalc *_condition):
		RDOFunCalcGroup(_nTempResType, _condition)	{}
	
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOFunCalcAbs: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(fabs(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcArcCos: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(acos(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcArcSin: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(asin(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcArcTan: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(atan(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcCos: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(cos(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcCotan: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(1./tan(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcExp: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(exp(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcFloor: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(floor(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcFrac: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		double tmp;
		return RDOValue(modf(sim->getFuncArgument(0), &tmp));
	}
};

class RDOFunCalcIAbs: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(abs(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcIMax: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue((int)_MAX(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcIMin: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue((int)_MIN(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcInt: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(int(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcIntPower: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(pow(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcLn: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(log(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcLog10: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(log10(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcLog2: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(log(sim->getFuncArgument(0)) / log(2) );
	}
};

class RDOFunCalcLogN: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(log(sim->getFuncArgument(0)) / log(sim->getFuncArgument(1)));
	}
};

class RDOFunCalcMax: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(_MAX(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcMin: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(_MIN(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcPower: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(pow(sim->getFuncArgument(0), sim->getFuncArgument(1)));
	}
};

class RDOFunCalcRound: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(floor(sim->getFuncArgument(0) + 0.5));
	}
};

class RDOFunCalcSin: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(sin(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcSqrt: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(sqrt(sim->getFuncArgument(0)));
	}
};

class RDOFunCalcTan: public RDOFunCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const	{
		return RDOValue(tan(sim->getFuncArgument(0)));
	}
};


class RDOCalcSeqInit: public RDOCalc
{
	int base;
	RandGenerator *gen;
public:
	RDOCalcSeqInit(int _base, RandGenerator *_gen): base(_base), gen(_gen) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
	~RDOCalcSeqInit();
	void setBase(int _base) { base = _base; }
};

class RDOCalcSeqNext: public RDOFunCalc
{
};

class RDOCalcSeqNextUniform: public RDOCalcSeqNext
{
	RandGeneratorUniform *gen;
public:
	RDOCalcSeqNextUniform(RandGeneratorUniform *_gen): gen(_gen) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOCalcSeqNextExponential: public RDOCalcSeqNext
{
	RandGeneratorExponential *gen;
public:
	RDOCalcSeqNextExponential(RandGeneratorExponential *_gen): gen(_gen) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOCalcSeqNextNormal: public RDOCalcSeqNext
{
	RandGeneratorNormal *gen;
public:
	RDOCalcSeqNextNormal(RandGeneratorNormal *_gen): gen(_gen) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOCalcSeqNextByHist: public RDOCalcSeqNext
{
	RandGeneratorByHistCommon *gen;
public:
	RDOCalcSeqNextByHist(RandGeneratorByHistCommon *_gen): gen(_gen) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOCalcGetRelevantResParam: public RDOCalc
{
	int relNumb;
	int parNumb;
public:
	RDOCalcGetRelevantResParam(int _relNumb, int _parNumb): relNumb(_relNumb), parNumb(_parNumb) {}
   virtual RDOValue calcValue(RDORuntime *sim) const
	{
		return sim->getResParamVal(sim->getRelResNumber(relNumb), parNumb);
	}
};



class RDOCalcEraseRes: public RDOCalc
{
	int relNumb;
public:
	RDOCalcEraseRes(int _relNumb): relNumb(_relNumb) {}
   virtual RDOValue calcValue(RDORuntime *sim) const
	{
		return sim->eraseRes(sim->getRelResNumber(relNumb));
	}
};

class RDOSelectResourceCalc: public RDOCalc
{
protected:
	int relNumb;
	RDOCalc *choice;
	RDOCalc *first;
	bool hasChoice;
	bool isFirst;
	bool isWithMin;
	RDOSelectResourceCalc(int _relNumb, RDOPATFirst *_first, RDOPATChoice *_choice);
};

class RDOSelectResourceDirectCalc: public RDOSelectResourceCalc
{
	int resNumb;
public:
	RDOSelectResourceDirectCalc(int _relNumb, int _resNumb, RDOPATFirst *_first, RDOPATChoice *_choice):
		RDOSelectResourceCalc(_relNumb, _first, _choice), resNumb(_resNumb) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOSelectResourceByTypeCalc: public RDOSelectResourceCalc
{
	int resType;
public:
	RDOSelectResourceByTypeCalc(int _relNumb, int _resType, RDOPATFirst *_first, RDOPATChoice *_choice):
		RDOSelectResourceCalc(_relNumb, _first, _choice), resType(_resType) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOSetRelParamCalc: public RDOCalc
{
	int relNumb;
	int parNumb;
	RDOCalc *calc;
public:
	RDOSetRelParamCalc(int _relNumb, int _parNumb, RDOCalc *_calc):
		relNumb(_relNumb), parNumb(_parNumb), calc(_calc) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOSetResourceParamCalc: public RDOCalc
{
	int resNumb;
	int parNumb;
	RDOCalc *calc;
public:
	RDOSetResourceParamCalc(int _resNumb, int _parNumb, RDOCalc *_calc):
		resNumb(_resNumb), parNumb(_parNumb), calc(_calc) {}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOSetPatternParamCalc: public RDOCalc
{
	int parNumb;
	RDOValue val;
public:
	RDOSetPatternParamCalc(int _parNumb, RDOValue _val):
		parNumb(_parNumb), val(_val) {}
   virtual RDOValue calcValue(RDORuntime *sim) const { sim->setPatternParameter(parNumb, val); return 0; }
};

class RDOCalcPatParam: public RDOCalc
{
	int numberOfParam;
public:
	RDOCalcPatParam(int _numberOfParam): numberOfParam(_numberOfParam)	{}
   virtual RDOValue calcValue(RDORuntime *sim) const;
};

class RDOCalcGetTimeNow: public RDOCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const { return sim->getTimeNow(); } 
};

class RDOCalcGetSeconds: public RDOCalc
{
public:
   virtual RDOValue calcValue(RDORuntime *sim) const { return sim->getSeconds(); } 
};

class RDOCalcGetConst: public RDOCalc
{
	int number;
public:
	RDOCalcGetConst(int _number): number(_number) {}
   virtual RDOValue calcValue(RDORuntime *sim) const { return sim->getConstValue(number); } 
};

class RDOCalcSetConst: public RDOCalc
{
	int number;
	RDOCalc *value;
public:
	RDOCalcSetConst(int _number, RDOCalc *_value): number(_number), value(_value) {}
   virtual RDOValue calcValue(RDORuntime *sim) const { sim->setConstValue(number, value->calcValue(sim)); return 0; } 
};


}  // namespace rdoRuntime

#endif //RDO_RUNTIME
