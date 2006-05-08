#ifndef RDODPT_DPT
#define RDODPT_DPT

#include "rdogramma.h"
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

int dptparse( void* lexer );
int dptlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void dpterror( char* mes );

int dpt_rss_parse( void* lexer );
int dpt_rss_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void dpt_rss_error( char* mes );

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
	std::string *name;
	RDOPATPatternRule *rule;
	DPTSearchValue value;
	RDOFUNArithm *ruleCost;
	int currParamNum;
	std::vector<RDOValue> params;
public:
	RDODPTSearchActivity(std::string *_name, std::string *_ruleName);
	void setValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost);
	const std::string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(std::string *_param);
	void addParam();
	RDOPATPatternRule *getRule() { return rule; }

	RDOSearchActivityRuntime *createActivityRuntime(RDORuntime *sim);
};

class RDODPTSearch: public RDODeletable
{
	const std::string *const name;
	DPTSearchTrace trace;
	RDOFUNLogic *conditon;
	RDOFUNLogic *termConditon;
	RDOFUNArithm *evalBy;
	bool compTops;
	std::vector<RDODPTSearchActivity *> activities;
public:
	RDODPTSearchActivity *lastActivity;
public:
	RDODPTSearch(std::string *_name, DPTSearchTrace _trace = DPTnotrace);
	void setCondition(RDOFUNLogic *_conditon = NULL) { conditon = _conditon; }
	void setTermCondition(RDOFUNLogic *_termConditon) { termConditon = _termConditon; }
	void setEvaluateBy(RDOFUNArithm *_evalBy) { evalBy = _evalBy; }
	void setCompareTops(bool _compTops) { compTops = _compTops; }
	void addNewActivity(std::string *_name, std::string *_ruleName);
	template <typename T>
		void addActivityParam(T _param)
	{
		lastActivity->addParam(_param);
	}

	void addActivityParam() { lastActivity->addParam(); }

	void setActivityValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost);
	void end();

	const std::string *getName() const { return name; }
	std::vector<RDODPTSearchActivity *> & getActivities() { return activities; }
};


/////////////////////////  "SOME" DECISION POINT /////////////////////////

class RDODPTSomeActivity: public RDODeletable
{
	std::string *name;
	const RDOPATPattern *pattern;
	int currParamNum;
	std::vector<RDOValue> params;
public:
	RDODPTSomeActivity(std::string *_name, std::string *_ruleName);
	const std::string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(std::string *_param);
	void addParam();

	void createActivityRuntime(RDOFUNLogic *conditon);
};

class RDODPTSome: public RDODeletable
{
	const std::string *const name;
	RDOFUNLogic *conditon;
	std::vector<RDODPTSomeActivity *> activities;
	RDODPTSomeActivity *lastActivity;
public:
	RDODPTSome(std::string *_name);
	void setCondition(RDOFUNLogic *_conditon = NULL) { conditon = _conditon; }
	void addNewActivity(std::string *_name, std::string *_patternName);
	template <typename T>
		void addActivityParam(T _param)
	{
		lastActivity->addParam(_param);
	}

	void addActivityParam() { lastActivity->addParam(); }

	void end();

	const std::string *getName() const { return name; }
};


/////////////////////////  FREE ACTIVITIES /////////////////////////

class RDODPTFreeActivity: public RDODeletable
{
	std::string *name;
	const RDOPATPattern *pattern;
	int currParamNum;
	std::vector<RDOValue> params;
	std::vector<std::string *> hotKeys;
public:
	RDODPTFreeActivity(std::string *_name, std::string *_ruleName);
	const std::string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(std::string *_param);
	void addParam();
	void end();
	void addHotKey(std::string *hotKey) { hotKeys.push_back(hotKey); }
};

///////////////////////// PROCESS /////////////////////////

class RDODPTProcess
{
public:
	static std::string name_prefix;
	static std::string name_sufix;
};

}		// namespace rdoParse 

#endif //RDODPT_DPT
