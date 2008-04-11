#include "pch.h"
#include "rdodpt.h"
#include "rdoopr.h"
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

int proc_rtp_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void proc_rtp_error( char* mes )
{
}

int proc_rss_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void proc_rss_error( char* mes )
{
}

int proc_opr_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void proc_opr_error( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivity
// ----------------------------------------------------------------------------
RDODPTActivity::RDODPTActivity( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	pattern( NULL ),
	currParam( 0 ),
	activity( NULL )
{
	init( _pattern_src_info );
}

RDODPTActivity::RDODPTActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	pattern( NULL ),
	currParam( 0 ),
	activity( NULL )
{
	init( _pattern_src_info );
}

void RDODPTActivity::init( const RDOParserSrcInfo& _pattern_src_info )
{
	parser()->checkActivityName( src_info() );
	pattern = parser()->findPattern( _pattern_src_info.src_text() );
	if ( !pattern ) {
		parser()->error( _pattern_src_info, rdo::format("Не найден образец: %s", _pattern_src_info.src_text().c_str()) );
	}
}

void RDODPTActivity::addParam( const std::string& _param, const YYLTYPE& _param_pos )
{
	if ( pattern->params.size() <= currParam ) {
		if ( _param_pos.first_line == src_pos().first_line ) {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error_push_only( _param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
			} else {
				parser()->error_push_only( _param_pos, rdo::format("Слишком много параметров для образца '%s' при описании активности '%s'", pattern->getName().c_str(), getName().c_str()) );
			}
			parser()->error_push_only( pattern->src_info(), "См. образец" );
			parser()->error_push_done();
		} else {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error( _param_pos, "Имя операции должно заканчиваться двоеточием" );
			} else {
				parser()->error( _param_pos, "Имя активности должно заканчиваться двоеточием" );
			}
		}
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case rdoRuntime::RDOValue::rvt_int : parser()->error( _param_pos, rdo::format("Ожидается параметр целого типа: %s", param->getType()->src_text().c_str()) ); break;
		case rdoRuntime::RDOValue::rvt_real: parser()->error( _param_pos, rdo::format("Ожидается параметр вещественного типа: %s", param->getType()->src_text().c_str()) ); break;
		case rdoRuntime::RDOValue::rvt_enum: break;
		default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue( _param, _param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(_param_pos, rdo::format("Параметр образца %s.%s = %s", pattern->getName().c_str(), param->getName().c_str(), _param.c_str())) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDODPTActivity::addParam( int _param, const YYLTYPE& _param_pos ) 
{
	if ( pattern->params.size() <= currParam ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
		} else {
			parser()->error_push_only( _param_pos, rdo::format("Слишком много параметров для образца '%s' при описании активности '%s'", pattern->getName().c_str(), getName().c_str()) );
		}
		parser()->error_push_only( pattern->src_info(), "См. образец" );
		parser()->error_push_done();
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case rdoRuntime::RDOValue::rvt_int :
		case rdoRuntime::RDOValue::rvt_real: break;
		case rdoRuntime::RDOValue::rvt_enum: parser()->error( _param_pos, rdo::format("Ожидается параметр перечислимого типа: %s", param->getType()->src_text().c_str()) ); break;
		default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue( _param, _param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(_param_pos, rdo::format("Параметр образца %s.%s = %d", pattern->getName().c_str(), param->getName().c_str(), _param)) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDODPTActivity::addParam( double _param, const YYLTYPE& _param_pos ) 
{
	if ( pattern->params.size() <= currParam ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
		} else {
			parser()->error_push_only( _param_pos, rdo::format("Слишком много параметров для образца '%s' при описании активности '%s'", pattern->getName().c_str(), getName().c_str()) );
		}
		parser()->error_push_only( pattern->src_info(), "См. образец" );
		parser()->error_push_done();
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	switch ( param->getType()->getType() ) {
		case rdoRuntime::RDOValue::rvt_int : parser()->error( _param_pos, rdo::format("Ожидается параметр целого типа: %s", param->getType()->src_text().c_str()) ); break;
		case rdoRuntime::RDOValue::rvt_real: break;
		case rdoRuntime::RDOValue::rvt_enum: parser()->error( _param_pos, rdo::format("Ожидается параметр перечислимого типа: %s", param->getType()->src_text().c_str()) ); break;
		default: parser()->error( src_info(), "Внутренняя ошибка: обработать все типы RDOValue" );
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue( _param, _param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(_param_pos, rdo::format("Параметр образца %s.%s = %f", pattern->getName().c_str(), param->getName().c_str(), _param)) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDODPTActivity::addParam( const YYLTYPE& _param_pos ) 
{
	if ( pattern->params.size() <= currParam ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", pattern->getName().c_str(), getName().c_str()) );
		} else {
			parser()->error_push_only( _param_pos, rdo::format("Слишком много параметров для образца '%s' при описании активности '%s'", pattern->getName().c_str(), getName().c_str()) );
		}
		parser()->error_push_only( pattern->src_info(), "См. образец" );
		parser()->error_push_done();
	}
	RDOFUNFunctionParam* param = pattern->params.at( currParam );
	if ( !param->getType()->getDV().isExist() ) {
		parser()->error_push_only( _param_pos, rdo::format("Нет значения по-умолчанию для параметра '%s'", param->src_text().c_str()) );
		parser()->error_push_only( param->src_info(), rdo::format("См. параметр '%s', тип '%s'", param->src_text().c_str(), param->getType()->src_text().c_str()) );
		parser()->error_push_done();
	}
	rdoRuntime::RDOValue val = param->getType()->getDefaultValue( _param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(_param_pos, rdo::format("Параметр образца %s.%s = *", pattern->getName().c_str(), param->getName().c_str())) );
	activity->addParamCalc( calc );
	currParam++;
}

void RDODPTActivity::endParam( const YYLTYPE& _param_pos )
{
	if ( pattern->params.size() > currParam ) {
		RDOFUNFunctionParam* param = pattern->params.at( currParam );
		parser()->error_push_only( _param_pos, rdo::format("Указаны не все параметра образца '%s':", pattern->src_text().c_str()) );
		for ( int i = currParam; i < pattern->params.size(); i++ ) {
			param = pattern->params.at( i );
			parser()->error_push_only( param->src_info(), rdo::format("Ожидаемый параметр '%s' имеет тип '%s'", param->getName().c_str(), param->getType()->src_text().c_str()) );
		}
		parser()->error_push_done();
	}
	if ( pattern->getType() == RDOPATPattern::PT_Keyboard && !activity->hasHotKey() ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, "Для клавиатурной операции должна быть указана клавиша" );
		} else {
			parser()->error_push_only( _param_pos, "Для активности должна быть указана клавиша" );
		}
		parser()->error_push_only( pattern->src_info(), "См. образец" );
		parser()->error_push_done();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag _trace ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	trace( _trace )
{
	parser()->checkDPTName( src_info() );
	parser()->insertDPTSearch( this );
}

RDODPTSearchActivity* RDODPTSearch::addNewActivity( const RDOParserSrcInfo& _activity_src_info, const RDOParserSrcInfo& _pattern_src_info )
{
	RDODPTSearchActivity* activity = new RDODPTSearchActivity( this, _activity_src_info, _pattern_src_info );
	activities.push_back( activity );
	return activity;
}

void RDODPTSearch::end()
{
	rdoRuntime::RDOCalc* condCalc = conditon ? conditon->createCalc() : new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
	rdoRuntime::RDOCalc* termCalc = termConditon ? termConditon->createCalc() : new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );

	rdoRuntime::RDODPTSearchRuntime* dpt = new rdoRuntime::RDODPTSearchRuntime( parser()->runtime(),
		condCalc,
		termCalc,
		evalBy->createCalc(),
		compTops );

	dpt->traceFlag = trace;
	parser()->runtime()->addRuntimeDPT( dpt );

	int size = activities.size();
	for ( int i = 0; i < size; i++ ) {
		RDODPTSearchActivity* activity = activities.at(i);
		rdoRuntime::RDOSearchActivityRuntime* act = new rdoRuntime::RDOSearchActivityRuntime( parser()->runtime(),
			dynamic_cast<rdoRuntime::RDORule*>(activity->getActivity()),
			activity->getValue(),
			activity->getRuleCost()->createCalc());
		dpt->addActivity( act );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( _parent, _src_info, _pattern_src_info ),
	value( rdoRuntime::RDODPTSearch::Activity::vt_before ),
	ruleCost( NULL )
{
	if ( pattern->getType() != RDOPATPattern::PT_Rule ) {
		parser()->error_push_only( src_info(), "Только продукционные правила могут быть использованы в точке принятия решений типа search" );
		parser()->error_push_only( pattern->src_info(), "См. образец" );
		parser()->error_push_done();
	}
	for ( std::vector< RDORelevantResource* >::const_iterator i = pattern->rel_res_begin(); i != pattern->rel_res_end(); i++ ) {
		if ( ((*i)->begin == rdoRuntime::RDOResourceTrace::CS_Create) || ((*i)->begin == rdoRuntime::RDOResourceTrace::CS_Erase) ) {
			parser()->error_push_only( src_info(), rdo::format("В продукционном правиле '%s' нельзя создавать или удалять ресурсы, т.к. оно используется в точке типа search", src_text().c_str()) );
			parser()->error_push_only( pattern->src_info(), "См. образец" );
			parser()->error_push_only( (*i)->src_info(), "См. релевантный ресурс" );
			parser()->error_push_done();
//			parser()->error( "Rule: " + getName() + " Cannot be used in search activity because of bad converter status" );
		}
	}
	activity = new rdoRuntime::RDOActivityRule( parser()->runtime(), pattern->getPatRuntime(), true, getName() );
}

void RDODPTSearchActivity::setValue( rdoRuntime::RDODPTSearch::Activity::ValueTime _value, RDOFUNArithm* _ruleCost, const YYLTYPE& _param_pos )
{
	endParam( _param_pos );
	value    = _value;
	ruleCost = _ruleCost;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info )
{
	parser()->checkDPTName( src_info() );
	// TODO: а почему some делает инкремент тут, а search в другом мечте ?
	parser()->runtime()->dptCounter++;
	parser()->insertDPTSome( this );
}

RDODPTSomeActivity* RDODPTSome::addNewActivity( const RDOParserSrcInfo& _activity_src_info, const RDOParserSrcInfo& _pattern_src_info )
{
	RDODPTSomeActivity* activity = new RDODPTSomeActivity( this, _activity_src_info, _pattern_src_info );
	activities.push_back( activity );
	return activity;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity( const RDODPTSome* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( _parent, _src_info, _pattern_src_info )
{
	if ( pattern->getType() != RDOPATPattern::PT_Rule && pattern->getType() != RDOPATPattern::PT_Operation ) {
		parser()->error_push_only( src_info(), "Только продукционные правила и операции могут быть использованы в точке типа some" );
		parser()->error_push_only( pattern->src_info(), "См. образец" );
		parser()->error_push_done();
	}
	if ( !_parent->getConditon() ) {
		activity = pattern->getPatRuntime()->createActivity( parser()->runtime(), getName() );
	} else {
		activity = pattern->getPatRuntime()->createActivity( parser()->runtime(), _parent->getConditon()->createCalc(), getName() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( _parser, _src_info, _pattern_src_info )
{
}

void RDODPTActivityHotKey::addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos )
{
	switch ( activity->addHotKey( parser()->runtime(), hotKey ) ) {
		case rdoRuntime::RDOActivityRuntime::addhk_ok      : {
			break;
		}
		case rdoRuntime::RDOActivityRuntime::addhk_already : {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error( hotkey_pos, rdo::format("Для операции '%s' клавиша уже назначена", src_text().c_str()) );
			} else {
				parser()->error( hotkey_pos, rdo::format("Для активности '%s' клавиша уже назначена", src_text().c_str()) );
			}
			break;
		}
		case rdoRuntime::RDOActivityRuntime::addhk_notfound: {
			parser()->error( hotkey_pos, rdo::format("Неизвестная клавиша: %s", hotKey.c_str()) );
			break;
		}
		case rdoRuntime::RDOActivityRuntime::addhk_dont    : {
			parser()->error_push_only( src_info(), rdo::format("Операция '%s' не является клавиатурной", src_text().c_str()) );
			parser()->error_push_only( getType()->src_info(), "См. образец" );
			parser()->error_push_done();
			break;
		}
		default: {
			parser()->error( src_info(), "Внутренная ошибка: RDOOPROperation::addHotKey" );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
RDODPTFreeActivity::RDODPTFreeActivity( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( _parser, _src_info, _pattern_src_info )
{
	if ( pattern->getType() != RDOPATPattern::PT_IE && pattern->getType() != RDOPATPattern::PT_Keyboard ) {
		parser()->error_push_only( src_info(), "Только нерегулярные события и клавиатурные операции могут быть использованы в списке свободных активностей" );
		parser()->error_push_only( pattern->src_info(), "См. образец" );
		parser()->error_push_done();
	}
	activity = pattern->getPatRuntime()->createActivity( parser()->runtime(), getName() );
	parser()->insertDPTFreeActivity( this );
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
	parser()->insertDPTProcess( this );
	runtime = new rdoRuntime::RDOPROCProcess( name, parser()->runtime() );
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
	runtime = new rdoRuntime::RDOPROCGenerate( parser()->getLastDPTProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
RDOPROCSeize::RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name ):
	RDOPROCOperator( _process, _name )
{
	const RDORSSResource* rss = parser()->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCSeize( parser()->getLastDPTProcess()->getRunTime(), rss->getNumber() );
	} else {
		parser()->error( "Внутренняя ошибка RDOPROCSeize: не нашли parser-ресурс" );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name ):
	RDOPROCOperator( _process, _name )
{
	const RDORSSResource* rss = parser()->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCRelease( parser()->getLastDPTProcess()->getRunTime(), rss->getNumber() );
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
	runtime = new rdoRuntime::RDOPROCAdvance( parser()->getLastDPTProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name ):
	RDOPROCOperator( _process, _name )
{
	runtime = new rdoRuntime::RDOPROCTerminate( parser()->getLastDPTProcess()->getRunTime() );
}

} // namespace rdoParse
