#ifndef RDODPT_DPT
#define RDODPT_DPT

using namespace std;
#include "rdoStdFuncs.h"

#include "rdofun.h"
#include "rdopat.h"

#include "rdodptrtime.h"

namespace rdoRuntime
{
class RDOCalc;
class RDOPatternRuntime;
}

using namespace rdoRuntime;

namespace rdoParse 
{

/////////////////////////  "SEARCH" DECISION POINT /////////////////////////

enum DPTSearchTrace
{
	DPTnotrace,
	DPTtracestat,
	DPTtracetops, 
	DPTtraceall  
};

enum DPTSearchValue
{
	DPT_value_before,
	DPT_value_after
};

class RDODPTSearchActivity: public RDODeletable
{
	string *name;
	RDOPATPatternRule *rule;
	DPTSearchValue value;
	RDOFUNArithm *ruleCost;
	int currParamNum;
	vector<RDOValue> params;
public:
	RDODPTSearchActivity(string *_name, string *_ruleName);
	void setValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost);
	const string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(string *_param);
	void addParam();
	RDOPATPatternRule *getRule() { return rule; }

	RDOSearchActivityRuntime *createActivityRuntime(RDORuntime *sim);
};

class RDODPTSearch: public RDODeletable
{
	const string *const name;
	DPTSearchTrace trace;
	RDOFUNLogic *conditon;
	RDOFUNLogic *termConditon;
	RDOFUNArithm *evalBy;
	bool compTops;
	vector<RDODPTSearchActivity *> activities;
	RDODPTSearchActivity *lastActivity;
public:
	RDODPTSearch(string *_name, DPTSearchTrace _trace = DPTnotrace);
	void setCondition(RDOFUNLogic *_conditon = NULL) { conditon = _conditon; }
	void setTermCondition(RDOFUNLogic *_termConditon) { termConditon = _termConditon; }
	void setEvaluateBy(RDOFUNArithm *_evalBy) { evalBy = _evalBy; }
	void setCompareTops(bool _compTops) { compTops = _compTops; }
	void addNewActivity(string *_name, string *_ruleName);
	template <typename T>
		void addActivityParam(T _param)
	{
		lastActivity->addParam(_param);
	}

	void addActivityParam() { lastActivity->addParam(); }

	void setActivityValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost);
	void end();

	const string *getName() const { return name; }
	vector<RDODPTSearchActivity *> & getActivities() { return activities; }
};


/////////////////////////  "SOME" DECISION POINT /////////////////////////

class RDODPTSomeActivity: public RDODeletable
{
	string *name;
	const RDOPATPattern *pattern;
	int currParamNum;
	vector<RDOValue> params;
public:
	RDODPTSomeActivity(string *_name, string *_ruleName);
	const string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(string *_param);
	void addParam();

	void createActivityRuntime(RDOFUNLogic *conditon);
};

class RDODPTSome: public RDODeletable
{
	const string *const name;
	RDOFUNLogic *conditon;
	vector<RDODPTSomeActivity *> activities;
	RDODPTSomeActivity *lastActivity;
public:
	RDODPTSome(string *_name);
	void setCondition(RDOFUNLogic *_conditon = NULL) { conditon = _conditon; }
	void addNewActivity(string *_name, string *_patternName);
	template <typename T>
		void addActivityParam(T _param)
	{
		lastActivity->addParam(_param);
	}

	void addActivityParam() { lastActivity->addParam(); }

	void end();

	const string *getName() const { return name; }
};


/////////////////////////  FREE ACTIVITIES /////////////////////////

class RDODPTFreeActivity: public RDODeletable
{
	string *name;
	const RDOPATPattern *pattern;
	int currParamNum;
	vector<RDOValue> params;
	vector<string *> hotKeys;
public:
	RDODPTFreeActivity(string *_name, string *_ruleName);
	const string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(string *_param);
	void addParam();
	void end();
	void addHotKey(string *hotKey) { hotKeys.push_back(hotKey); }
};

}		// namespace rdoParse 

#endif //RDODPT_DPT
