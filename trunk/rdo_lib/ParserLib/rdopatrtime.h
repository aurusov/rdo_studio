#ifndef RDO_PAT_RUNTIME
#define RDO_PAT_RUNTIME

using namespace std;
#include "rdotrace.h"

class RDOSimulator;
class RDOResourceTrace;

namespace rdoRuntime {

class RDOCalc;
class RDORuntime;
class RDOActivityRuntime;

class RDOPatternRuntime
{
protected:
	vector<RDOCalc *>	beginCalcs;
	vector<RDOCalc *>	endCalcs;
	string patternId;
	RDOCalc *timeField;
	vector<RDOCalc *>	choiceFromCalcs;
	RDORuntime *runtime;
	bool trace;
	RDOPatternRuntime(RDORuntime *_runtime, bool _trace);
public:
   virtual bool choiceFrom(RDOSimulator *sim);
	virtual void addBeginCalc(RDOCalc *calc) { beginCalcs.push_back(calc); }
	virtual void addEndCalc(RDOCalc *calc);
	void setPatternId(int _id) { patternId = toString(_id); }
	string& getPatternId() { return patternId; }

	void setTime(RDOCalc *_time) { timeField = _time; }
	void addChoiceFromCalc(RDOCalc *_calc) { choiceFromCalcs.push_back(_calc); }
	virtual RDOActivityRuntime *createActivity(string *_oprName) = 0;
	virtual RDOActivityRuntime *createActivity(RDOCalc *condition, string *_oprName);
	virtual ~RDOPatternRuntime() {}
};

class RDORuleRuntime: public RDOPatternRuntime
{
public:
   bool choiceFrom(RDOSimulator *sim);
   void convertRule(RDOSimulator *sim);
   string tracePatternId() { return patternId; }

	RDORuleRuntime(RDORuntime *rTime, bool _trace);
	RDOActivityRuntime *createActivity(string *_oprName); 
	RDOActivityRuntime *createActivity(RDOCalc *condition, string *_oprName);
};

class RDOIERuntime: public RDOPatternRuntime
{
public:
   double getNextTimeInterval(RDOSimulator *sim);
   void convertEvent(RDOSimulator *sim);
   string tracePatternId() { return patternId; }
   bool choiceFrom(RDOSimulator *sim);

	RDOIERuntime(RDORuntime *rTime, bool _trace);
	RDOActivityRuntime *createActivity(string *_oprName); 
};

class RDOOperationRuntime: public RDOPatternRuntime
{
	void addEndCalc(RDOCalc *calc)  { endCalcs.push_back(calc); }

public:
   bool choiceFrom(RDOSimulator *sim);
   double getNextTimeInterval(RDOSimulator *sim);
   void convertBegin(RDOSimulator *sim);
   void convertEnd(RDOSimulator *sim);
   string tracePatternId() { return patternId; }

	RDOOperationRuntime(RDORuntime *rTime, bool _trace);
	virtual RDOActivityRuntime *createActivity(string *_oprName); 
	virtual RDOActivityRuntime *createActivity(RDOCalc *condition, string *_oprName);
};

class RDOKeyboardRuntime: public RDOOperationRuntime
{
public:
	RDOKeyboardRuntime(RDORuntime *rTime, bool _trace)
		: RDOOperationRuntime(rTime, _trace)
	{}
	virtual RDOActivityRuntime *createActivity(string *_oprName); 
	virtual RDOActivityRuntime *createActivity(RDOCalc *condition, string *_oprName);
};

class RDOActivityRuntime
{
protected:
	string *oprName;
   vector<int> relResNumbers;
	vector<RDOCalc *>	setParamsCalcs;
	RDOPatternRuntime *pattern;
	RDOActivityRuntime(RDOPatternRuntime *_pattern, string *_oprName): pattern(_pattern), oprName(_oprName) {}
	void setPatternParameters(RDOSimulator *sim);
   virtual vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim);
public:
	void addParamCalc(RDOCalc *calc) { setParamsCalcs.push_back(calc); }
	virtual ~RDOActivityRuntime() {}
	int getRelResNumber(const int nRelRes) const { return relResNumbers.at(nRelRes); } 
	void selectRelResource(int relNumb, int resNumb) 
	{ 
		if(relResNumbers.size() <= relNumb)
			relResNumbers.resize(relNumb + 1); 

		relResNumbers[relNumb] = resNumb; 
	}
	virtual void addHotKey(string *hotKey);
	void writeModelStructure(stringstream &stream);
};

class RDOActivityRuleRuntime: public RDORuleTrace, public RDOActivityRuntime
{
	bool haveAdditionalCondition;
	RDOCalc *additionalCondition;
   bool choiceFrom(RDOSimulator *sim);
   void convertRule(RDOSimulator *sim);
   string tracePatternId() { return ((RDORuleRuntime*)pattern)->tracePatternId(); }
   vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim) { return RDOActivityRuntime::getRelevantResources(sim); }
   void onBeforeChoiceFrom(RDOSimulator *sim) { setPatternParameters(sim); }
public:
	RDOActivityRuleRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, string *_oprName);
	RDOActivityRuleRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, string *_oprName);
};

class RDOActivityIERuntime: public RDOIETrace, public RDOActivityRuntime
{
   double getNextTimeInterval(RDOSimulator *sim);
   void convertEvent(RDOSimulator *sim);
   string tracePatternId() { return ((RDOIERuntime*)pattern)->tracePatternId(); }
   bool choiceFrom(RDOSimulator *sim);
   vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim) { return RDOActivityRuntime::getRelevantResources(sim); }
   void onBeforeIrregularEvent(RDOSimulator *sim) { setPatternParameters(sim); RDOIETrace::onBeforeIrregularEvent(sim); }
public:
	RDOActivityIERuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, string *_oprName);
};

class RDOActivityOperationRuntime: public RDOOperationTrace, public RDOActivityRuntime
{
	bool haveAdditionalCondition;
	RDOCalc *additionalCondition;
   double getNextTimeInterval(RDOSimulator *sim);
   void convertBegin(RDOSimulator *sim);
   void convertEnd(RDOSimulator *sim);
   string tracePatternId() { return ((RDOOperationRuntime*)pattern)->tracePatternId(); }
   vector<RDOResourceTrace *> getRelevantResources(RDOSimulator *sim) { return RDOActivityRuntime::getRelevantResources(sim); }
   RDOOperationTrace *clone2(RDOSimulator *sim);
   void onBeforeChoiceFrom(RDOSimulator *sim) { setPatternParameters(sim); }
   void onBeforeOperationEnd(RDOSimulator *sim) { setPatternParameters(sim); RDOOperationTrace::onBeforeOperationEnd(sim); }
protected:
   virtual bool choiceFrom(RDOSimulator *sim);
public:
	RDOActivityOperationRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, string *_oprName);
	RDOActivityOperationRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, string *_oprName);
};

class RDOActivityKeyboardRuntime: public RDOActivityOperationRuntime
{
	vector<int> keyScanCodes;
public:
	RDOActivityKeyboardRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, string *_oprName)
		: RDOActivityOperationRuntime(rTime, _pattern, _trace, _oprName)
	{}
	RDOActivityKeyboardRuntime(RDORuntime *rTime, RDOPatternRuntime *_pattern, bool _trace, RDOCalc *condition, string *_oprName)
		: RDOActivityOperationRuntime(rTime, _pattern, _trace, condition, _oprName)
	{}
	void addHotKey(string *hotKey);
   bool choiceFrom(RDOSimulator *sim);
};

}

#endif //RDO_PAT_RUNTIME
