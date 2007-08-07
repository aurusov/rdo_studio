#include "pch.h"
#include "rdodpt.h"
#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdorss.h"
#include <rdopatrtime.h>
#include <rdoprocess.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int dptlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void dpterror( char* mes )
{
}

int dpt_rss_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void dpt_rss_error( char* mes )
{
}

int dpt_opr_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void dpt_opr_error( char* mes )
{
}

/////////////////////////  "SEARCH" DECISION POINT /////////////////////////

RDODPTSearch::RDODPTSearch( RDOParser* _parser, std::string* _name, DPTSearchTrace _trace ):
	RDOParserObject( _parser ),
	name( _name ),
	trace( _trace ),
	lastActivity( NULL )
{
	if(std::find_if(parser->getDPTSome().begin(), parser->getDPTSome().end(), compareName<RDODPTSome>(_name)) != parser->getDPTSome().end())
		parser->error("DPT name: " + *_name + " already defined");

	if(std::find_if(parser->getDPTSearch().begin(), parser->getDPTSearch().end(), compareName<RDODPTSearch>(_name)) != parser->getDPTSearch().end())
		parser->error("DPT name: " + *_name + " already defined");

	parser->insertDPTSearch( this );
}

void RDODPTSearch::addNewActivity( std::string* _name, std::string* _ruleName )
{
	parser->checkActivityName( _name );
	lastActivity = new RDODPTSearchActivity( this, _name, _ruleName );
	activities.push_back( lastActivity );
}

void RDODPTSearch::setActivityValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost)
{
	lastActivity->setValue(_value, _ruleCost);
}

void RDODPTSearch::end()
{
	rdoRuntime::RDOCalc *condCalc;
	if(!conditon)
		condCalc = new rdoRuntime::RDOCalcConst(parser->runTime, 1);
	else
		condCalc = conditon->calc;

	rdoRuntime::RDOSearchRuntime *dpt = new rdoRuntime::RDOSearchRuntime(parser->runTime, 
		condCalc, 
		termConditon->calc, 
		evalBy->createCalc(), 
		compTops);

	switch(trace)
	{
	case DPTnotrace:
		dpt->traceFlag = rdoRuntime::DPT_no_trace;
		break;
	case DPTtracestat:
		dpt->traceFlag = rdoRuntime::DPT_trace_stat;
		break;
	case DPTtracetops:
		dpt->traceFlag = rdoRuntime::DPT_trace_tops;
		break;
	case DPTtraceall:
		dpt->traceFlag = rdoRuntime::DPT_trace_all;
		break;
	};

	parser->runTime->addRuntimeDPT( dpt );

	int size = activities.size();
	for(int i = 0; i < size; i++)
	{
		RDODPTSearchActivity *curr = activities.at(i);
		rdoRuntime::RDOSearchActivityRuntime *act = curr->createActivityRuntime(parser->runTime);
		dpt->addActivity(act);
	}
}

RDODPTSearchActivity::RDODPTSearchActivity( const RDOParserObject* _parent, std::string* _name, std::string* _ruleName ):
	RDOParserObject( _parent ),
	name( _name )
{
	const RDOPATPattern* pat = parser->findPattern( *_ruleName );
	pat->testGoodForSearchActivity();
	rule = (RDOPATPatternRule *)pat;
	currParamNum = 0;
}

void RDODPTSearchActivity::setValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost)
{
	if(currParamNum != rule->params.size())
		parser->error("Too few parameters for rule: " + rule->getName());

	value = _value;
	ruleCost = _ruleCost;
}

void RDODPTSearchActivity::addParam(int _param) 
{
	if(currParamNum >= rule->params.size())
		parser->error("Too many parameters for rule: " + rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam(double *_param) 
{
	if(currParamNum >= rule->params.size())
		parser->error("Too many parameters for rule: " + rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam(std::string *_param) 
{
	if(currParamNum >= rule->params.size())
		parser->error("Too many parameters for rule: " + rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam() 
{
	if(currParamNum >= rule->params.size())
		parser->error("Too many parameters for rule: " + rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

rdoRuntime::RDOSearchActivityRuntime* RDODPTSearchActivity::createActivityRuntime( rdoRuntime::RDORuntime* sim )
{
	rdoRuntime::RDOActivityRuleRuntime *activity = new rdoRuntime::RDOActivityRuleRuntime(sim, rule->patRuntime, true, *name);
	rdoRuntime::RDOSearchActivityRuntime *act = new rdoRuntime::RDOSearchActivityRuntime(sim, 
		dynamic_cast<rdoRuntime::RDORule*>(activity),
		(value == DPT_value_after), 
		ruleCost->createCalc());

	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(parser->runTime, i, params.at(i)));

	return act;
}

/////////////////////////  "SOME" DECISION POINT //////////////////////

RDODPTSome::RDODPTSome( RDOParser* _parser, std::string* _name ):
	RDOParserObject( _parser ),
	name( _name ),
	lastActivity( NULL )
{
	if(std::find_if(parser->getDPTSome().begin(), parser->getDPTSome().end(), compareName<RDODPTSome>(_name)) != parser->getDPTSome().end())
		parser->error("DPT name: " + *_name + " already defined");

	if(std::find_if(parser->getDPTSearch().begin(), parser->getDPTSearch().end(), compareName<RDODPTSearch>(_name)) != parser->getDPTSearch().end())
		parser->error("DPT name: " + *_name + " already defined");

	parser->insertDPTSome( this );
}

void RDODPTSome::addNewActivity( std::string* _name, std::string* _patternName )
{
	parser->checkActivityName( _name );
	lastActivity = new RDODPTSomeActivity( this, _name, _patternName );
	activities.push_back( lastActivity );
}

void RDODPTSome::end()
{
	parser->runTime->dptCounter++;
	int size = activities.size();
	for(int i = 0; i < size; i++)
		activities.at(i)->createActivityRuntime(conditon);
}

RDODPTSomeActivity::RDODPTSomeActivity( const RDOParserObject* _parent, std::string* _name, std::string* _patternName):
	RDOParserObject( _parent ),
	name( _name )
{
	pattern = parser->findPattern( *_patternName );
	pattern->testGoodForSomeActivity();
	currParamNum = 0;
}

void RDODPTSomeActivity::addParam(int _param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam(double *_param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam(std::string *_param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam() 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

void RDODPTSomeActivity::createActivityRuntime(RDOFUNLogic *conditon)
{
	rdoRuntime::RDOActivityRuntime *activity;
	if(!conditon)
		activity = pattern->patRuntime->createActivity( *name );
	else
		activity = pattern->patRuntime->createActivity(conditon->calc, *name);

	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(parser->runTime, i, params.at(i)));
}


/////////////////////////  FREE ACTIVITIES /////////////////////////

RDODPTFreeActivity::RDODPTFreeActivity( RDOParser* _parser, std::string* _name, std::string* _patternName ):
	RDOParserObject( _parser ),
	name( _name )
{
	parser->checkActivityName( _name );
	parser->insertDPTFreeActivity( this );

	pattern = parser->findPattern( *_patternName );
	if ( !pattern ) {
		parser->error( rdo::format("Не найден образец: %s", _patternName->c_str()) );
	}
	pattern->testGoodForFreeActivity();
	currParamNum = 0;
}

void RDODPTFreeActivity::addParam(int _param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam(double *_param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam(std::string *_param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam() 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

void RDODPTFreeActivity::end()
{
	rdoRuntime::RDOActivityRuntime *activity = pattern->patRuntime->createActivity(*name);
	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(parser->runTime, i, params.at(i)));

	size = hotKeys.size();
	for(i = 0; i < size; i++)
		activity->addHotKey(hotKeys.at(i));
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
std::string RDOPROCProcess::name_prefix = "";
std::string RDOPROCProcess::name_sufix  = "s";

RDOPROCProcess::RDOPROCProcess( RDOParser* _parser, const std::string& _name ):
	RDOParserObject( _parser ),
	name( _name ),
	m_end( false ),
	parent( NULL ),
	runtime( NULL )
{
	parser->insertDPTProcess( this );
	runtime = new rdoRuntime::RDOPROCProcess( name, parser->runTime );
}

void RDOPROCProcess::end()
{
	m_end = true;
}

void RDOPROCProcess::insertChild( RDOPROCProcess* value )
{
	if ( value ) {
		child.push_back( value );
		value->parent = this;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
bool RDOPROCTransact::created = false;

RDOPROCTransact::RDOPROCTransact( RDOParser* _parser ):
	RDORTPResType( _parser, _parser->registerName( "Транзакты" ), false )
{
	// Создадим параметр вещественного типа 'Время_создания'
	addParam( new RDORTPParamDesc( parser->registerName( "Время_создания" ), new RDORTPRealResParam( this ) ) );
	// Создадим параметр целого типа 'Просто_так'
	addParam( new RDORTPParamDesc( parser->registerName( "Просто_так" ), new RDORTPIntResParam( this ) ) );
	// Больше этот ресурс создавать не надо
	RDOPROCTransact::created = true;
}

RDOPROCTransact::~RDOPROCTransact()
{
	RDOPROCTransact::created = false;
}

RDOPROCTransact* RDOPROCTransact::makeRTP( RDOParser* _parser )
{
	if ( RDOPROCTransact::created ) {
		RDOPROCTransact* rtp = static_cast<RDOPROCTransact*>(const_cast<RDORTPResType*>(_parser->findRTPResType( "Транзакты" )));
		return rtp;
	} else {
		RDOPROCTransact* rtp = new RDOPROCTransact( _parser );
		rdoRuntime::RDOPROCTransact::typeID = rtp->getNumber();
		return rtp;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperator
// ----------------------------------------------------------------------------
RDOPROCOperator::RDOPROCOperator( RDOPROCProcess* _process, const std::string& _name ):
	RDOParserObject( _process ),
	name( _name ),
	process( _process )
{
	process->operations.push_back( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time ):
	RDOPROCOperator( _process, _name ),
	runtime( NULL )
{
	runtime = new rdoRuntime::RDOPROCGenerate( parser->getLastDPTProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
RDOPROCSeize::RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name, const std::string* res_name ):
	RDOPROCOperator( _process, _name )
{
	const RDORSSResource* rss = parser->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCSeize( parser->getLastDPTProcess()->getRunTime(), rss->getNumber() );
	} else {
		// error: не нашли parser-ресурс
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name, const std::string* res_name ):
	RDOPROCOperator( _process, _name )
{
	const RDORSSResource* rss = parser->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCRelease( parser->getLastDPTProcess()->getRunTime(), rss->getNumber() );
	} else {
		// error: не нашли parser-ресурс
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time ):
	RDOPROCOperator( _process, _name )
{
	runtime = new rdoRuntime::RDOPROCAdvance( parser->getLastDPTProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name ):
	RDOPROCOperator( _process, _name )
{
	runtime = new rdoRuntime::RDOPROCTerminate( parser->getLastDPTProcess()->getRunTime() );
}

} // namespace rdoParse
