#ifndef RDODPT_DPT
#define RDODPT_DPT

#include "rdoparser_object.h"
#include "rdofun.h"
#include "rdopat.h"
#include <rdodptrtime.h>

namespace rdoRuntime
{
class RDOCalc;
class RDODPTSome;
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

int proc_rtp_parse( void* lexer );
int proc_rtp_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void proc_rtp_error( char* mes );

int proc_rss_parse( void* lexer );
int proc_rss_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void proc_rss_error( char* mes );

int proc_opr_parse( void* lexer );
int proc_opr_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void proc_opr_error( char* mes );

// ----------------------------------------------------------------------------
// ---------- RDODPTActivity
// ----------------------------------------------------------------------------
class RDODPTActivity: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDODPTActivity( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
	RDODPTActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );

	const std::string&       getName() const     { return src_info().src_text(); }
	const RDOPATPattern*     getType() const     { return m_pattern;             }
	rdoRuntime::RDOActivity* getActivity() const { return m_activity;            }

	void addParam( const std::string& _param, const YYLTYPE& _param_pos );
	void addParam( int _param, const YYLTYPE& _param_pos );
	void addParam( double _param, const YYLTYPE& _param_pos );
	void addParam( const YYLTYPE& _param_pos );
	void endParam( const YYLTYPE& _param_pos );

protected:
	const RDOPATPattern*     m_pattern;
	unsigned int             m_currParam;
	rdoRuntime::RDOActivity* m_activity;

private:
	void init( const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
class RDODPTSearch: public RDOParserObject, public RDOParserSrcInfo
{
private:
	rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag m_trace;
	RDOFUNLogic*   conditon;
	RDOFUNLogic*   termConditon;
	RDOFUNArithm*  evalBy;
	bool compTops;
	std::vector< RDODPTSearchActivity* > activities;

public:
	RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace = rdoRuntime::RDODPTSearchTrace::DPT_no_trace );
	const std::string& getName() const                         { return src_info().src_text(); }
	void setCondition( RDOFUNLogic* _conditon = NULL )         { conditon     = _conditon;     }
	void setTermCondition( RDOFUNLogic* _termConditon = NULL ) { termConditon = _termConditon; }
	void setEvaluateBy( RDOFUNArithm* _evalBy )                { evalBy       = _evalBy;       }
	void setCompareTops( bool _compTops )                      { compTops     = _compTops;     }

	RDODPTSearchActivity* addNewActivity( const RDOParserSrcInfo& _activity_src_info, const RDOParserSrcInfo& _pattern_src_info );
	RDODPTSearchActivity* getLastActivity() const {
		return !activities.empty() ? activities.back() : NULL;
	}
	const std::vector< RDODPTSearchActivity* >& getActivities() const { return activities; }

	void end();
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
class RDODPTSearchActivity: public RDODPTActivity
{
private:
	rdoRuntime::RDODPTSearch::Activity::ValueTime value;
	RDOFUNArithm* ruleCost;

public:
	RDODPTSearchActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );

	rdoRuntime::RDODPTSearch::Activity::ValueTime getValue() const { return value; }
	void setValue( rdoRuntime::RDODPTSearch::Activity::ValueTime _value, RDOFUNArithm* _ruleCost, const YYLTYPE& _param_pos );

	RDOFUNArithm* getRuleCost() const { return ruleCost; }
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info );

	const std::string& getName() const                { return src_info().src_text(); }

	RDOFUNLogic* getConditon() const                  { return m_conditon;             }
	void setCondition( RDOFUNLogic* conditon = NULL ) { m_conditon = conditon;         }

	RDODPTSomeActivity* addNewActivity( const RDOParserSrcInfo& _activity_src_info, const RDOParserSrcInfo& _pattern_src_info );
	RDODPTSomeActivity* getLastActivity() const
	{
		return !m_activities.empty() ? m_activities.back() : NULL;
	}
	const std::vector< RDODPTSomeActivity* >& getActivities() const { return m_activities; }
	void end();

private:
	rdoRuntime::RDODPTSome*            m_dpt;
	RDOFUNLogic*                       m_conditon;
	std::vector< RDODPTSomeActivity* > m_activities;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
class RDODPTSomeActivity: public RDODPTActivity
{
public:
	RDODPTSomeActivity( const RDODPTSome* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
public:
	RDODPTActivityHotKey( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
	void addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
class RDODPTFreeActivity: public RDODPTActivityHotKey
{
public:
	RDODPTFreeActivity( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
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
	RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCRelease* runtime;

public:
	RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name );
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
