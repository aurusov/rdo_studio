#ifndef RDODPT_DPT
#define RDODPT_DPT

#include "rdoparser_object.h"
#include "rdoparser_logic.h"
#include "rdofun.h"
#include "rdopat.h"
#include <rdo_activity.h>
#include <searchtrace.h>

namespace rdoRuntime
{
class RDOCalc;
class RDODPTFree;
class RDODPTSome;
class RDODPTSearchRuntime;
class RDOPROCProcess;
class RDOPROCGenerate;
class RDOPROCSeize;
class RDOPROCTerminate;
class RDOPROCAdvance;
class RDOPROCRelease;
struct parser_for_Seize;
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
	RDODPTActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );

	const std::string&       name() const     { return src_info().src_text(); }
	rdoRuntime::RDOActivity* activity() const { return m_activity;            }
	const RDOPATPattern*     pattern() const  { return m_pattern;             }

	void addParam( const std::string& _param, const YYLTYPE& _param_pos );
	void addParam( int _param, const YYLTYPE& _param_pos );
	void addParam( double _param, const YYLTYPE& _param_pos );
	void addParam( const YYLTYPE& _param_pos );
	void endParam( const YYLTYPE& _param_pos );

protected:
	rdoRuntime::RDOActivity* m_activity;

private:
	unsigned int             m_currParam;
	const RDOPATPattern*     m_pattern;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
class RDODPTActivityHotKey: public RDODPTActivity
{
public:
	void addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos );

protected:
	RDODPTActivityHotKey( const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
class RDODPTFreeActivity: public RDODPTActivityHotKey
{
friend class RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>;
private:
	RDODPTFreeActivity( const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
class RDODPTFree: public RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>
{
public:
	RDODPTFree( RDOParser* _parser, const RDOParserSrcInfo& _src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
class RDODPTSomeActivity: public RDODPTActivityHotKey
{
friend class RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>;
private:
	RDODPTSomeActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
class RDODPTSome: public RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>
{
public:
	RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info );

	RDOFUNLogic* getConditon() const                  { return m_conditon;     }
	void setCondition( RDOFUNLogic* conditon = NULL ) { m_conditon = conditon; }

	void end();

private:
	RDOFUNLogic* m_conditon;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
class RDODPTSearchActivity: public RDODPTActivity
{
friend class RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>;
public:
	rdoRuntime::RDODPTSearch::Activity::ValueTime getValue() const { return m_value; }
	void setValue( rdoRuntime::RDODPTSearch::Activity::ValueTime value, RDOFUNArithm* ruleCost, const YYLTYPE& _param_pos );

	RDOFUNArithm* getRuleCost() const { return m_ruleCost; }

private:
	RDODPTSearchActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );

	rdoRuntime::RDODPTSearch::Activity::ValueTime  m_value;
	RDOFUNArithm*                                  m_ruleCost;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
class RDODPTSearch: public RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>
{
public:
	RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace = rdoRuntime::RDODPTSearchTrace::DPT_no_trace );

	void setCondition( RDOFUNLogic* conditon = NULL )         { m_conditon     = conditon;     }
	void setTermCondition( RDOFUNLogic* termConditon = NULL ) { m_termConditon = termConditon; }
	void setEvaluateBy( RDOFUNArithm* evalBy )                { m_evalBy       = evalBy;       }
	void setCompareTops( bool compTops )                      { m_compTops     = compTops;     }

	void end();
	bool closed() const { return m_closed; }

private:
	RDOFUNLogic*   m_conditon;
	RDOFUNLogic*   m_termConditon;
	RDOFUNArithm*  m_evalBy;
	bool           m_compTops;
	bool           m_closed;
	rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag  m_trace;
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
class RDOPROCProcess: public RDOParserObject
{
friend class RDOPROCOperator;

public:
	static std::string s_name_prefix;
	static std::string s_name_sufix;

	RDOPROCProcess( RDOParser* _parser, const std::string& name );

	void end();
	bool closed() const { return m_closed; }

	void insertChild( RDOPROCProcess* value );

	rdoRuntime::RDOPROCProcess* getRunTime() const { return m_runtime; }

protected:
	std::string                   m_name;
	bool                          m_closed;
	RDOPROCProcess*               m_parent;
	std::list< RDOPROCProcess* >  m_child;
	std::list< RDOPROCOperator* > m_operations;
	rdoRuntime::RDOPROCProcess*   m_runtime;
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
	std::list< std::string > Resources;
	std::vector< rdoRuntime::parser_for_Seize > parser_for_runtime;
public:
	static void makeSeizeType		(  RDOParser *parser, const std::string& res_name, const RDOParserSrcInfo& info );
	static void makeSeizeResourse	(  RDOParser *parser, const std::string& res_name, const RDOParserSrcInfo& info );
	void add_Seize_Resourse			( std::string res_name){Resources.push_back( res_name );}	
	void create_runtime_Seize( RDOParser *parser );
	RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name );
};

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
class RDOPROCRelease: public RDOPROCOperator
{
protected:
	rdoRuntime::RDOPROCRelease* runtime;
	std::list< std::string > Resources;
	std::vector < rdoRuntime::parser_for_Seize > parser_for_runtime;
	
public:
	void create_runtime_Release( RDOParser *parser );
	void add_Release_Resourse			( std::string res_name){Resources.push_back( res_name );}	
	static void checkReleaseType		(  RDOParser *parser, const std::string& res_name, const RDOParserSrcInfo& info );
	static void checkReleaseResourse	(  RDOParser *parser, const std::string& res_name, const RDOParserSrcInfo& info );
	RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name);
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
