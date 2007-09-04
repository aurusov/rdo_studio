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

// ----------------------------------------------------------------------------
// ---------- DPT Search
// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const std::string& _rule ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	rule( NULL ),
	ruleCost( NULL )
{
	const RDOPATPattern* pat = getParser()->findPattern( _rule );
	pat->testGoodForSearchActivity();
	rule = (RDOPATPatternRule *)pat;
	currParamNum = 0;
}

void RDODPTSearchActivity::setValue( DPTSearchValue _value, RDOFUNArithm* _ruleCost )
{
	if(currParamNum != rule->params.size())
		getParser()->error("Too few parameters for rule: " + rule->getName());

	value = _value;
	ruleCost = _ruleCost;
}

void RDODPTSearchActivity::addParam(int _param) 
{
	if(currParamNum >= rule->params.size())
		getParser()->error("Too many parameters for rule: " + rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(_param, RDOParserSrcInfo("левое"));
	params.push_back(val);
}

void RDODPTSearchActivity::addParam(double _param) 
{
	if(currParamNum >= rule->params.size())
		getParser()->error("Too many parameters for rule: " + rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(_param, RDOParserSrcInfo("левое"));
	params.push_back(val);
}

void RDODPTSearchActivity::addParam( const std::string& _param )
{
	if(currParamNum >= rule->params.size())
		getParser()->error("Too many parameters for rule: " + rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(_param, RDOParserSrcInfo("левое"));
	params.push_back(val);
}

void RDODPTSearchActivity::addParam() 
{
	if(currParamNum >= rule->params.size())
		getParser()->error("Too many parameters for rule: " + rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getParamDefaultValue(RDOParserSrcInfo("левое"));
	params.push_back(val);
}

rdoRuntime::RDOSearchActivityRuntime* RDODPTSearchActivity::createActivityRuntime( rdoRuntime::RDORuntime* sim )
{
	rdoRuntime::RDOActivityRuleRuntime* activity = new rdoRuntime::RDOActivityRuleRuntime( sim, rule->patRuntime, true, getName() );
	rdoRuntime::RDOSearchActivityRuntime *act = new rdoRuntime::RDOSearchActivityRuntime(sim, 
		dynamic_cast<rdoRuntime::RDORule*>(activity),
		(value == DPT_value_after), 
		ruleCost->createCalc());

	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(getParser()->runtime, i, params.at(i)));

	return act;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, DPTSearchTrace _trace ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	trace( _trace ),
	lastActivity( NULL )
{
	if(std::find_if(getParser()->getDPTSome().begin(), getParser()->getDPTSome().end(), compareName2<RDODPTSome>(getName())) != getParser()->getDPTSome().end())
		getParser()->error("DPT name: " + getName() + " already defined");

	if(std::find_if(getParser()->getDPTSearch().begin(), getParser()->getDPTSearch().end(), compareName2<RDODPTSearch>(getName())) != getParser()->getDPTSearch().end())
		getParser()->error("DPT name: " + getName() + " already defined");

	getParser()->insertDPTSearch( this );
}

void RDODPTSearch::addNewActivity( const RDOParserSrcInfo& _src_info, const std::string& _pattern )
{
	getParser()->checkActivityName( _src_info );
	lastActivity = new RDODPTSearchActivity( this, _src_info, _pattern );
	activities.push_back( lastActivity );
}

void RDODPTSearch::addActivityParam()
{
	lastActivity->addParam();
}

void RDODPTSearch::setActivityValue( RDODPTSearchActivity::DPTSearchValue _value, RDOFUNArithm* _ruleCost )
{
	lastActivity->setValue( _value, _ruleCost );
}

void RDODPTSearch::end()
{
	rdoRuntime::RDOCalc *condCalc;
	if(!conditon)
		condCalc = new rdoRuntime::RDOCalcConst(getParser()->runtime, 1);
	else
		condCalc = conditon->calc;

	rdoRuntime::RDOSearchRuntime *dpt = new rdoRuntime::RDOSearchRuntime(getParser()->runtime, 
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

	getParser()->runtime->addRuntimeDPT( dpt );

	int size = activities.size();
	for(int i = 0; i < size; i++)
	{
		RDODPTSearchActivity *curr = activities.at(i);
		rdoRuntime::RDOSearchActivityRuntime *act = curr->createActivityRuntime(getParser()->runtime);
		dpt->addActivity(act);
	}
}

// ----------------------------------------------------------------------------
// ---------- DPT Some
// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const std::string& _pattern ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	pattern( NULL ),
	currParamNum( 0 )
{
	pattern = getParser()->findPattern( _pattern );
	pattern->testGoodForSomeActivity();
//	currParamNum = 0;
}

void RDODPTSomeActivity::addParam(int _param) 
{
	if(currParamNum >= pattern->params.size())
		getParser()->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(_param, RDOParserSrcInfo("левое"));
	params.push_back(val);
}

void RDODPTSomeActivity::addParam(double _param) 
{
	if(currParamNum >= pattern->params.size())
		getParser()->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(_param, RDOParserSrcInfo("левое"));
	params.push_back(val);
}

void RDODPTSomeActivity::addParam( const std::string& _param )
{
	if(currParamNum >= pattern->params.size())
		getParser()->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(_param, RDOParserSrcInfo("левое"));
	params.push_back(val);
}

void RDODPTSomeActivity::addParam() 
{
	if(currParamNum >= pattern->params.size())
		getParser()->error("Too many parameters for pattern: " + pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getParamDefaultValue(RDOParserSrcInfo("левое"));
	params.push_back(val);
}

void RDODPTSomeActivity::createActivityRuntime(RDOFUNLogic *conditon)
{
	rdoRuntime::RDOActivityRuntime *activity;
	if(!conditon)
		activity = pattern->patRuntime->createActivity( getName() );
	else
		activity = pattern->patRuntime->createActivity(conditon->calc, getName());

	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(getParser()->runtime, i, params.at(i)));
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	lastActivity( NULL )
{
	if(std::find_if(getParser()->getDPTSome().begin(), getParser()->getDPTSome().end(), compareName2<RDODPTSome>(getName())) != getParser()->getDPTSome().end())
		getParser()->error("DPT name: " + getName() + " already defined");

	if(std::find_if(getParser()->getDPTSearch().begin(), getParser()->getDPTSearch().end(), compareName2<RDODPTSearch>(getName())) != getParser()->getDPTSearch().end())
		getParser()->error("DPT name: " + getName() + " already defined");

	getParser()->insertDPTSome( this );
}

void RDODPTSome::addNewActivity( const RDOParserSrcInfo& _src_info, const std::string& _pattern )
{
	getParser()->checkActivityName( _src_info );
	lastActivity = new RDODPTSomeActivity( this, _src_info, _pattern );
	activities.push_back( lastActivity );
}

void RDODPTSome::end()
{
	getParser()->runtime->dptCounter++;
	int size = activities.size();
	for(int i = 0; i < size; i++)
		activities.at(i)->createActivityRuntime(conditon);
}

// ----------------------------------------------------------------------------
// ---------- DPT Free
// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
RDODPTFreeActivity::RDODPTFreeActivity( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	pattern( NULL ),
	currParam( 0 ),
	activity( NULL )
{
	getParser()->checkActivityName( src_info() );
	pattern = getParser()->findPattern( _pattern_src_info.src_text() );
	if ( !pattern ) {
		getParser()->error( _pattern_src_info, rdo::format("Не найден образец: %s", _pattern_src_info.src_text().c_str()) );
	}
	if ( pattern->getType() != RDOPATPattern::PT_IE && pattern->getType() != RDOPATPattern::PT_Keyboard ) {
		getParser()->error_push_only( src_info(), "Только нерегулярные события и клавиатурные операции могут быть использованы в списке свободных активностей" );
		getParser()->error_push_only( pattern->src_info(), "См. образец" );
		getParser()->error_push_done();
	}
	activity = pattern->patRuntime->createActivity( getName() );
	getParser()->insertDPTFreeActivity( this );
}

void RDODPTFreeActivity::addParam( const std::string& stringParam, const YYLTYPE& param_pos )
{
	if ( pattern->params.size() <= currParam ) {
		getParser()->error_push_only( param_pos, rdo::format("Слишком много параметров для образца '%s' при описании активности '%s', а если это имя новой активности, то оно должно заканчиваться двоеточием", pattern->getName().c_str(), getName().c_str()) );
		getParser()->error_push_only( pattern->src_info(), "См. образец" );
		getParser()->error_push_done();
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case RDORTPParamType::pt_int : getParser()->error( param_pos, rdo::format("Ожидается параметр целого типа: %s", param->getType()->src_text().c_str()) ); break;
		case RDORTPParamType::pt_real: getParser()->error( param_pos, rdo::format("Ожидается параметр вещественного типа: %s", param->getType()->src_text().c_str()) ); break;
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue( stringParam, param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( getParser()->runtime, currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param_pos, rdo::format("Параметр образца %s.%s = %s", pattern->getName().c_str(), param->getName().c_str(), stringParam.c_str())) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDODPTFreeActivity::addParam( int intParam, const YYLTYPE& param_pos ) 
{
	if ( pattern->params.size() <= currParam ) {
		getParser()->error_push_only( param_pos, rdo::format("Слишком много параметров для образца '%s' при описании активности '%s'", pattern->getName().c_str(), getName().c_str()) );
		getParser()->error_push_only( pattern->src_info(), "См. образец" );
		getParser()->error_push_done();
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case RDORTPParamType::pt_enum: getParser()->error( param_pos, rdo::format("Ожидается параметр перечислимого типа: %s", param->getType()->src_text().c_str()) ); break;
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue( intParam, param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( getParser()->runtime, currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param_pos, rdo::format("Параметр образца %s.%s = %d", pattern->getName().c_str(), param->getName().c_str(), intParam)) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDODPTFreeActivity::addParam( double realParam, const YYLTYPE& param_pos ) 
{
	if ( pattern->params.size() <= currParam ) {
		getParser()->error_push_only( param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
		getParser()->error_push_only( pattern->src_info(), "См. образец" );
		getParser()->error_push_done();
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case RDORTPParamType::pt_int : getParser()->error( param_pos, rdo::format("Ожидается параметр целого типа: %s", param->getType()->src_text().c_str()) ); break;
		case RDORTPParamType::pt_enum: getParser()->error( param_pos, rdo::format("Ожидается параметр перечислимого типа: %s", param->getType()->src_text().c_str()) ); break;
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue( realParam, param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( getParser()->runtime, currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param_pos, rdo::format("Параметр образца %s.%s = %f", pattern->getName().c_str(), param->getName().c_str(), realParam)) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDODPTFreeActivity::addParam( const YYLTYPE& param_pos ) 
{
	if ( pattern->params.size() <= currParam ) {
		getParser()->error_push_only( param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
		getParser()->error_push_only( pattern->src_info(), "См. образец" );
		getParser()->error_push_done();
	}

	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	if ( !param->getType()->dv->isExist() ) {
		getParser()->error_push_only( param_pos, rdo::format("Нет значения по-умолчанию для параметра '%s'", param->src_text().c_str()) );
		getParser()->error_push_only( param->src_info(), rdo::format("См. параметр '%s', тип '%s'", param->src_text().c_str(), param->getType()->src_text().c_str()) );
		getParser()->error_push_done();
	}
	rdoRuntime::RDOValue val = param->getType()->getParamDefaultValue( param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( getParser()->runtime, currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param_pos, rdo::format("Параметр образца %s.%s = *", pattern->getName().c_str(), param->getName().c_str())) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDODPTFreeActivity::addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos )
{
	switch ( activity->addHotKey( hotKey ) ) {
		case rdoRuntime::RDOActivityRuntime::addhk_ok      : {
			break;
		}
		case rdoRuntime::RDOActivityRuntime::addhk_already : {
			getParser()->error( hotkey_pos, rdo::format("Для операции '%s' клавиша уже назначена", src_text().c_str()) );
			break;
		}
		case rdoRuntime::RDOActivityRuntime::addhk_notfound: {
			getParser()->error( hotkey_pos, rdo::format("Неизвестная клавиша: %s", hotKey.c_str()) );
			break;
		}
		case rdoRuntime::RDOActivityRuntime::addhk_dont    : {
			getParser()->error_push_only( src_info(), rdo::format("Операция '%s' не является клавиатурной", src_text().c_str()) );
			getParser()->error_push_only( getType()->src_info(), "См. образец" );
			getParser()->error_push_done();
			break;
		}
		default: {
			getParser()->error( src_info(), "Внутренная ошибка: RDOOPROperation::addHotKey" );
		}
	}
}

void RDODPTFreeActivity::end( const YYLTYPE& opr_pos )
{
	if ( pattern->params.size() > currParam ) {
		RDOFUNFunctionParam* param = pattern->params.at( currParam );
		if ( pattern->params.size() == currParam + 1 ) {
			getParser()->error( opr_pos, rdo::format("Указаны не все параметра образца, ожидаемый параметр '%s' имеет тип '%s'", param->getName().c_str(), param->getType()->src_text().c_str()) );
		} else {
			getParser()->error_push_only( opr_pos, rdo::format("Указаны не все параметра образца:") );
			for ( int i = currParam; i < pattern->params.size(); i++ ) {
				param = pattern->params.at( i );
				getParser()->error_push_only( param->src_info(), rdo::format("Ожидаемый параметр '%s' имеет тип '%s'", param->getName().c_str(), param->getType()->src_text().c_str()) );
			}
			getParser()->error_push_done();
		}
	}
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
	getParser()->insertDPTProcess( this );
	runtime = new rdoRuntime::RDOPROCProcess( name, getParser()->runtime );
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
	RDORTPResType( _parser, "Транзакты", false )
{
	// Создадим параметр вещественного типа 'Время_создания'
	addParam( new RDORTPParam( this, "Время_создания", new RDORTPRealParamType( this ) ) );
	// Создадим параметр целого типа 'Просто_так'
	addParam( new RDORTPParam( this, "Просто_так", new RDORTPIntParamType( this ) ) );
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
	runtime = new rdoRuntime::RDOPROCGenerate( getParser()->getLastDPTProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
RDOPROCSeize::RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name ):
	RDOPROCOperator( _process, _name )
{
	const RDORSSResource* rss = getParser()->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCSeize( getParser()->getLastDPTProcess()->getRunTime(), rss->getNumber() );
	} else {
		// error: не нашли parser-ресурс
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name ):
	RDOPROCOperator( _process, _name )
{
	const RDORSSResource* rss = getParser()->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCRelease( getParser()->getLastDPTProcess()->getRunTime(), rss->getNumber() );
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
	runtime = new rdoRuntime::RDOPROCAdvance( getParser()->getLastDPTProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name ):
	RDOPROCOperator( _process, _name )
{
	runtime = new rdoRuntime::RDOPROCTerminate( getParser()->getLastDPTProcess()->getRunTime() );
}

} // namespace rdoParse
