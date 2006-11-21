#ifndef RDO_PAT_RUNTIME
#define RDO_PAT_RUNTIME

#include <rdotrace.h>

class RDOSimulator;
class RDOResourceTrace;

namespace rdoRuntime {

class RDOCalc;
class RDORuntime;
class RDOActivityRuntime;

class RDOPatternRuntime
{
protected:
	std::vector<RDOCalc *>	beginCalcs;
	std::vector<RDOCalc *>	endCalcs;
	std::string patternId;
	RDOCalc *timeField;
	std::vector<RDOCalc *>	choiceFromCalcs;
	RDORuntime *runtime;
	bool trace;
	RDOPatternRuntime(RDORuntime *_runtime, bool _trace);
public:
	virtual bool choiceFrom(RDOSimulator *sim);
	virtual void addBeginCalc(RDOCalc *calc) { beginCalcs.push_back(calc); }
	virtual void addEndCalc(RDOCalc *calc);
	void setPatternId(int _id) { patternId = toString(_id); }
	std::string& getPatternId() { return patternId; }

	void setTime(RDOCalc *_time) { timeField = _time; }
	void addChoiceFromCalc(RDOCalc *_calc) { choiceFromCalcs.push_back(_calc); }
	virtual RDOActivityRuntime *createActivity(std::string *_oprName) = 0;
	virtual RDOActivityRuntime *createActivity(RDOCalc *condition, std::string *_oprName);
	virtual ~RDOPatternRuntime() {}
};

class RDORuleRuntime: public RDOPatternRuntime
{
public:
	bool choiceFrom(RDOSimulator *sim);
	void convertRule(RDOSimulator *sim);
	std::string tracePatternId() { return patternId; }

	RDORuleRuntime(RDORuntime *rTime, bool _trace);
	RDOActivityRuntime *createActivity(std::string *_oprName); 
	RDOActivityRuntime *createActivity(RDOCalc *condition, std::string *_oprName);
};

class RDOIERuntime: public RDOPatternRuntime
{
public:
	double getNextTimeInterval(RDOSimulator *sim);
	void convertEvent(RDOSimulator *sim);
	std::string tracePatternId() { return patternId; }
	bool choiceFrom(RDOSimulator *sim);

	RDOIERuntime(RDORuntime *rTime, bool _trace);
	RDOActivityRuntime *createActivity(std::string *_oprName); 
};

class RDOOperationRuntime: public RDOPatternRuntime
{
	void addEndCalc(RDOCalc *calc)  { endCalcs.push_back(calc); }

public:
	bool choiceFrom(RDOSimulator *sim);
	double getNextTimeInterval(RDOSimulator *sim);
	void convertBegin(RDOSimulator *sim);
	void convertEnd(RDOSimulator *sim);
	std::string tracePatternId() { return patternId; }

	RDOOperationRuntime(RDORuntime *rTime, bool _trace);
	virtual RDOActivityRuntime *createActivity(std::string *_oprName); 
	virtual RDOActivityRuntime *createActivity(RDOCalc *condition, std::string *_oprName);
};

class RDOKeyboardRuntime: public RDOOperationRuntime
{
public:
	RDOKeyboardRuntime(RDORuntime *rTime, bool _trace)
		: RDOOperationRuntime(rTime, _trace)
	{}
	virtual RDOActivityRuntime *createActivity(std::string *_oprName); 
	virtual RDOActivityRuntime *createActivity(RDOCalc *condition, std::string *_oprName);
};

class RDOActivityRuntime
{
protected:
	std::string *oprName;
	std::vector<int> relResNumbers;
	std::vector<RDOCalc *>	setParamsCalcs;
	RDOPatternRuntime *pattern;
	RDOActivityRuntime(RDOPatternRuntime *_pattern, std::string *_oprName): pattern(_pattern), oprName(_oprName) {}
	void setPatternParameters(RDOSimulator *sim);
	virtual std::vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim);
	void incrementRelevantResourceReference(RDOSimulator *sim);
	void decrementRelevantResourceReference(RDOSimulator *sim);
public:
	void addParamCalc(RDOCalc *calc) { setParamsCalcs.push_back(calc); }
	virtual ~RDOActivityRuntime() {}
	int getRelResNumber( const int nRelRes ) const
	{
		if ( relResNumbers.size() <= nRelRes ) {
			return 0;
		}
		return relResNumbers.at(nRelRes); 
	} 
	void selectRelResource( int relNumb, int resNumb )
	{
		if ( relResNumbers.size() <= relNumb ) {
			relResNumbers.resize( relNumb + 1 );
		}
		relResNumbers[relNumb] = resNumb; 
	}
	virtual void addHotKey( std::string* hotKey );
	void writeModelStructure(std::stringstream &stream);
};

class RDOActivityRuleRuntime: public RDORuleTrace, public RDOActivityRuntime
{
	bool haveAdditionalCondition;
	RDOCalc *additionalCondition;
	bool choiceFrom(RDOSimulator *sim);
	void convertRule(RDOSimulator *sim);
	std::string tracePatternId() { return ((RDORuleRuntime*)pattern)->tracePatternId(); }
	std::vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim) { return RDOActivityRuntime::getRelevantResources(sim); }
	void onBeforeChoiceFrom(RDOSimulator *sim) { setPatternParameters(sim); }
public:
	RDOActivityRuleRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, std::string *_oprName);
	RDOActivityRuleRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, std::string *_oprName);
};

class RDOActivityIERuntime: public RDOIETrace, public RDOActivityRuntime
{
   double getNextTimeInterval(RDOSimulator *sim);
   void convertEvent(RDOSimulator *sim);
   std::string tracePatternId() { return ((RDOIERuntime*)pattern)->tracePatternId(); }
   bool choiceFrom(RDOSimulator *sim);
   std::vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim) { return RDOActivityRuntime::getRelevantResources(sim); }
   void onBeforeIrregularEvent(RDOSimulator *sim) { setPatternParameters(sim); RDOIETrace::onBeforeIrregularEvent(sim); }
public:
	RDOActivityIERuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, std::string *_oprName);
};

class RDOActivityOperationRuntime: public RDOOperationTrace, public RDOActivityRuntime
{
	bool haveAdditionalCondition;
	RDOCalc *additionalCondition;
	double getNextTimeInterval(RDOSimulator *sim);
	void convertBegin(RDOSimulator *sim);
	void convertEnd(RDOSimulator *sim);
	std::string tracePatternId() { return ((RDOOperationRuntime*)pattern)->tracePatternId(); }
	std::vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim) { return RDOActivityRuntime::getRelevantResources(sim); }
	RDOOperationTrace *clone2(RDOSimulator *sim);
	void onBeforeChoiceFrom(RDOSimulator *sim) { setPatternParameters(sim); }
	void onAfterOperationBegin(RDOSimulator *sim) { incrementRelevantResourceReference(sim); RDOOperationTrace::onAfterOperationBegin(sim); }
	void onBeforeOperationEnd(RDOSimulator *sim) { decrementRelevantResourceReference(sim); setPatternParameters(sim); RDOOperationTrace::onBeforeOperationEnd(sim); }
protected:
	virtual bool choiceFrom(RDOSimulator *sim);
public:
	RDOActivityOperationRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, std::string *_oprName);
	RDOActivityOperationRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, std::string *_oprName);
};

class RDOActivityKeyboardRuntime: public RDOActivityOperationRuntime
{
private:
	bool shift;
	bool control;
	unsigned int scan_code;

public:
	RDOActivityKeyboardRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, std::string* _oprName ):
		RDOActivityOperationRuntime( rTime, _pattern, _trace, _oprName ),
		shift( false ),
		control( false ),
		scan_code( 0 )
	{
	}
	RDOActivityKeyboardRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, RDOCalc* condition, std::string* _oprName ):
		RDOActivityOperationRuntime( rTime, _pattern, _trace, condition, _oprName ),
		shift( false ),
		control( false ),
		scan_code( 0 )
	{
	}
	void addHotKey( std::string* hotKey );
	bool choiceFrom( RDOSimulator* sim );
};

}

#endif // RDO_PAT_RUNTIME
