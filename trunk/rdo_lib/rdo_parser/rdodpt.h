#ifndef RDODPT_DPT
#define RDODPT_DPT

#include "rdoparser_object.h"
#include "rdofun.h"
#include "rdopat.h"
#include <rdodptrtime.h>

namespace rdoRuntime
{
class RDOCalc;
class RDOPatternRuntime;
class RDOPROCProcess;
class RDOPROCGenerate;
class RDOPROCSeize;
class RDOPROCTerminate;
class RDOPROCAdvance;
class RDOPROCRelease;
}

namespace rdoParse 
{

int dptparse( void* lexer );
int dptlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void dpterror( char* mes );

int dpt_rss_parse( void* lexer );
int dpt_rss_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void dpt_rss_error( char* mes );

int dpt_opr_parse( void* lexer );
int dpt_opr_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void dpt_opr_error( char* mes );

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

class RDODPTSearchActivity: public RDOParserObject
{
private:
	std::string *name;
	RDOPATPatternRule *rule;
	DPTSearchValue value;
	RDOFUNArithm *ruleCost;
	int currParamNum;
	std::vector< rdoRuntime::RDOValue > params;

public:
	RDODPTSearchActivity( const RDOParserObject* _parent, std::string* _name, std::string* _ruleName );
	void setValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost);
	const std::string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(std::string *_param);
	void addParam();
	RDOPATPatternRule *getRule() { return rule; }

	rdoRuntime::RDOSearchActivityRuntime* createActivityRuntime( rdoRuntime::RDORuntime* sim );
};

class RDODPTSearch: public RDOParserObject
{
private:
	const std::string* const name;
	DPTSearchTrace trace;
	RDOFUNLogic*   conditon;
	RDOFUNLogic*   termConditon;
	RDOFUNArithm*  evalBy;
	bool compTops;
	std::vector< RDODPTSearchActivity* > activities;

public:
	RDODPTSearch( RDOParser* _parser, std::string* _name, DPTSearchTrace _trace = DPTnotrace );
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
	RDODPTSearchActivity* lastActivity;

	void addActivityParam() { lastActivity->addParam(); }

	void setActivityValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost);
	void end();

	const std::string *getName() const { return name; }
	const std::vector<RDODPTSearchActivity *>& getActivities() const { return activities; }
};


/////////////////////////  "SOME" DECISION POINT /////////////////////////

class RDODPTSomeActivity: public RDOParserObject
{
private:
	std::string *name;
	const RDOPATPattern *pattern;
	int currParamNum;
	std::vector< rdoRuntime::RDOValue > params;

public:
	RDODPTSomeActivity( const RDOParserObject* _parent, std::string* _name, std::string* _ruleName );
	const std::string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(std::string *_param);
	void addParam();

	void createActivityRuntime(RDOFUNLogic *conditon);
};

class RDODPTSome: public RDOParserObject
{
private:
	const std::string *const name;
	RDOFUNLogic *conditon;
	std::vector< RDODPTSomeActivity* > activities;
	RDODPTSomeActivity* lastActivity;

public:
	RDODPTSome( RDOParser* _parser, std::string* _name );
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

	const std::vector< RDODPTSomeActivity* >& getActivities() const { return activities; }
};


/////////////////////////  FREE ACTIVITIES /////////////////////////

class RDODPTFreeActivity: public RDOParserObject
{
private:
	std::string*         name;
	const RDOPATPattern* pattern;
	int currParamNum;
	std::vector< rdoRuntime::RDOValue > params;
	std::vector< std::string* >         hotKeys;

public:
	RDODPTFreeActivity( RDOParser* _parser, std::string* _name, std::string* _ruleName );
	const std::string *getName() const { return name; }
	void addParam(int _param);
	void addParam(double * _param);
	void addParam(std::string *_param);
	void addParam();
	void end();
	void addHotKey(std::string *hotKey) { hotKeys.push_back(hotKey); }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCProcess: public RDOParserObject
{
friend class RDOPROCOperator;

protected:
	std::string                   name;
	bool                          m_end;
	RDOPROCProcess*               parent;
	std::list< RDOPROCProcess* >  child;
	std::list< RDOPROCOperator* > operations;
	rdoRuntime::RDOPROCProcess*   runtime;

public:
	static std::string name_prefix;
	static std::string name_sufix;

	RDOPROCProcess( RDOParser* _parser, const std::string& _name );

	void end();
	bool isend() const { return m_end; }

	void insertChild( RDOPROCProcess* value );

	rdoRuntime::RDOPROCProcess* getRunTime() const { return runtime; }
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
class RDOPROCTransact: public RDORTPResType
{
protected:
	static bool created;
	RDOPROCTransact( RDOParser* _parser );

public:
	static RDOPROCTransact* makeRTP( RDOParser* _parser );
	virtual ~RDOPROCTransact();
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperator
// ----------------------------------------------------------------------------
class RDOPROCOperator: public RDOParserObject
{
protected:
	std::string     name;
	RDOPROCProcess* process;

	RDOPROCOperator( RDOPROCProcess* _process, const std::string& _name );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
class RDOPROCGenerate: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCGenerate* runtime;

public:
	RDOPROCGenerate( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
class RDOPROCSeize: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCSeize* runtime;

public:
	RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name, const std::string* res_name );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCRelease* runtime;

public:
	RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name, const std::string* res_name );
};


// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
class RDOPROCAdvance: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCAdvance* runtime;

public:
	RDOPROCAdvance( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
class RDOPROCTerminate: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCTerminate* runtime;

public:
	RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name );
};

} // namespace rdoParse 

#endif // RDODPT_DPT
