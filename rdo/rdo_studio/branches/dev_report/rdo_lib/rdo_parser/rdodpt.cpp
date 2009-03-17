#include "pch.h"
#include "rdodpt.h"
#include "rdoopr.h"
#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdorss.h"
#include <rdo_ie.h>
#include <rdo_rule.h>
#include <rdo_operation.h>
#include <rdo_keyboard.h>
#include <rdoprocess.h>
#include <rdo_logic_dptfree.h>
#include <rdo_logic_dptsome.h>
#include <rdodptrtime.h>
#include <rdo_resources.h>

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
RDODPTActivity::RDODPTActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	m_pattern( NULL ),
	m_currParam( 0 ),
	m_activity( NULL )
{
	parser()->checkActivityName( src_info() );
	m_pattern = parser()->findPATPattern( _pattern_src_info.src_text() );
	if ( !m_pattern ) {
		parser()->error( _pattern_src_info, rdo::format("Не найден образец: %s", _pattern_src_info.src_text().c_str()) );
	}
}

void RDODPTActivity::addParam( const RDOValue& param )
{
	if ( m_pattern->params.size() <= m_currParam ) {
		if ( param.src_pos().m_first_line == src_pos().m_first_line ) {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error_push_only( param, rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", m_pattern->name().c_str(), name().c_str()) );
			} else {
				parser()->error_push_only( param, rdo::format("Слишком много параметров для образца '%s' при описании активности '%s'", m_pattern->name().c_str(), name().c_str()) );
			}
			parser()->error_push_only( m_pattern->src_info(), "См. образец" );
			parser()->error_push_done();
		} else {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error( param, "Имя операции должно заканчиваться двоеточием" );
			} else {
				parser()->error( param, "Имя активности должно заканчиваться двоеточием" );
			}
		}
	}
	rdoRuntime::RDOValue val;
	RDOFUNFunctionParam* pat_param = m_pattern->params.at( m_currParam );
	if ( param->getAsString() == "*" )
	{
		if ( !pat_param->getType()->getDV().isExist() ) {
			parser()->error_push_only( param, rdo::format("Нет значения по-умолчанию для параметра '%s'", pat_param->src_text().c_str()) );
			parser()->error_push_only( pat_param->src_info(), rdo::format("См. параметр '%s', тип '%s'", pat_param->src_text().c_str(), pat_param->getType()->src_text().c_str()) );
			parser()->error_push_done();
		}
		val = pat_param->getType()->getDefaultValue( param );
	}
	else
	{
		val = pat_param->getType()->getValue( param );
	}
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), m_currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(param.getPosAsYY(), rdo::format("Параметр образца %s.%s = %s", m_pattern->name().c_str(), pat_param->name().c_str(), param->getAsString().c_str())) );
	m_activity->addParamCalc( calc );
	m_currParam++;
}

void RDODPTActivity::endParam( const YYLTYPE& _param_pos )
{
	if ( m_pattern->params.size() > m_currParam ) {
		RDOFUNFunctionParam* param = m_pattern->params.at( m_currParam );
		parser()->error_push_only( _param_pos, rdo::format("Указаны не все параметра образца '%s':", m_pattern->src_text().c_str()) );
		for ( unsigned int i = m_currParam; i < m_pattern->params.size(); i++ ) {
			param = m_pattern->params.at( i );
			parser()->error_push_only( param->src_info(), rdo::format("Ожидаемый параметр '%s' имеет тип '%s'", param->name().c_str(), param->getType()->src_text().c_str()) );
		}
		parser()->error_push_done();
	}
	if ( m_pattern->getType() == RDOPATPattern::PT_Keyboard && !static_cast<rdoRuntime::RDOKeyboard*>(m_activity)->hasHotKey() ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, "Для клавиатурной операции должна быть указана клавиша" );
		} else {
			parser()->error_push_only( _param_pos, "Для активности должна быть указана клавиша" );
		}
		parser()->error_push_only( m_pattern->src_info(), "См. образец" );
		parser()->error_push_done();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey( const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( parent, _src_info, _pattern_src_info )
{
	switch ( pattern()->getType() )
	{
		case RDOPATPattern::PT_IE:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternIrregEvent*>(pattern()->getPatRuntime())->createActivity( parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Rule:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternRule*>(pattern()->getPatRuntime())->createActivity( parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Operation:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternOperation*>(pattern()->getPatRuntime())->createActivity( parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Keyboard:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternKeyboard*>(pattern()->getPatRuntime())->createActivity( parser()->runtime(), name() );
			break;
		}
		default:
		{
			parser()->error_push_only( src_info(), "Неизвестный тип образца" );
			parser()->error_push_only( pattern()->src_info(), "См. образец" );
			parser()->error_push_done();
		}
	}
}

void RDODPTActivityHotKey::addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos )
{
	if ( pattern()->getType() != RDOPATPattern::PT_Keyboard )
	{
		parser()->error_push_only( hotkey_pos, "Горячие клавиши используются только в клавиатурных операциях" );
		parser()->error_push_only( pattern()->src_info(), "См. образец" );
		parser()->error_push_done();
	}
	switch ( static_cast<rdoRuntime::RDOKeyboard*>(m_activity)->addHotKey( parser()->runtime(), hotKey ) ) {
		case rdoRuntime::RDOKeyboard::addhk_ok      : {
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_already : {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error( hotkey_pos, rdo::format("Для операции '%s' клавиша уже назначена", src_text().c_str()) );
			} else {
				parser()->error( hotkey_pos, rdo::format("Для активности '%s' клавиша уже назначена", src_text().c_str()) );
			}
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_notfound: {
			parser()->error( hotkey_pos, rdo::format("Неизвестная клавиша: %s", hotKey.c_str()) );
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_dont    : {
			parser()->error_push_only( src_info(), rdo::format("Операция '%s' не является клавиатурной", src_text().c_str()) );
			parser()->error_push_only( pattern()->src_info(), "См. образец" );
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
RDODPTFreeActivity::RDODPTFreeActivity( const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( parent, _src_info, _pattern_src_info )
{
	parser()->insertDPTFreeActivity( this );
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
RDODPTFree::RDODPTFree( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>( _parser, _src_info )
{
	parser()->checkDPTName( src_info() );
	m_rt_logic = new rdoRuntime::RDODPTFree( parser()->runtime() );
	parser()->insertDPTFree( this );
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( _parent, _src_info, _pattern_src_info )
{
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>( _parser, _src_info )
{
	parser()->checkDPTName( src_info() );
	m_rt_logic = new rdoRuntime::RDODPTSome( parser()->runtime() );
	parser()->insertDPTSome( this );
}

void RDODPTSome::end()
{
	if ( getConditon() )
	{
		m_rt_logic->setCondition( getConditon()->getCalc() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( _parent, _src_info, _pattern_src_info ),
	m_value( rdoRuntime::RDODPTSearch::Activity::vt_before ),
	m_ruleCost( NULL )
{
	if ( pattern()->getType() != RDOPATPattern::PT_Rule ) {
		parser()->error_push_only( src_info(), "Только продукционные правила могут быть использованы в точке принятия решений типа search" );
		parser()->error_push_only( pattern()->src_info(), "См. образец" );
		parser()->error_push_done();
	}
	for ( std::vector< RDORelevantResource* >::const_iterator i = pattern()->rel_res_begin(); i != pattern()->rel_res_end(); i++ ) {
		if ( ((*i)->begin == rdoRuntime::RDOResource::CS_Create) || ((*i)->begin == rdoRuntime::RDOResource::CS_Erase) ) {
			parser()->error_push_only( src_info(), rdo::format("В продукционном правиле '%s' нельзя создавать или удалять ресурсы, т.к. оно используется в точке типа search", src_text().c_str()) );
			parser()->error_push_only( pattern()->src_info(), "См. образец" );
			parser()->error_push_only( (*i)->src_info(), "См. релевантный ресурс" );
			parser()->error_push_done();
//			parser()->error( "Rule: " + name() + " Cannot be used in search activity because of bad converter status" );
		}
	}
	m_activity = new rdoRuntime::RDORule( parser()->runtime(), static_cast<rdoRuntime::RDOPatternRule*>(pattern()->getPatRuntime()), true, name() );
}

void RDODPTSearchActivity::setValue( rdoRuntime::RDODPTSearch::Activity::ValueTime value, RDOFUNArithm* ruleCost, const YYLTYPE& _param_pos )
{
	endParam( _param_pos );
	m_value    = value;
	m_ruleCost = ruleCost;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace ):
	RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>( _parser, _src_info ),
	m_trace( trace ),
	m_closed( false )
{
	parser()->checkDPTName( src_info() );
	parser()->insertDPTSearch( this );
}

void RDODPTSearch::end()
{
	rdoRuntime::RDOCalc* condCalc = m_conditon ? m_conditon->getCalc() : new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
	rdoRuntime::RDOCalc* termCalc = m_termConditon ? m_termConditon->getCalc() : new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );

	m_rt_logic = new rdoRuntime::RDODPTSearchRuntime( parser()->runtime(),
		condCalc,
		termCalc,
		m_evalBy->createCalc(),
		m_compTops );

	m_rt_logic->traceFlag = m_trace;

	int size = getActivities().size();
	for ( int i = 0; i < size; i++ ) {
		const RDODPTSearchActivity* activity = getActivities().at(i);
		rdoRuntime::RDOSearchActivityRuntime* act = new rdoRuntime::RDOSearchActivityRuntime(
			dynamic_cast<rdoRuntime::RDORule*>(activity->activity()),
			activity->getValue(),
			activity->getRuleCost()->createCalc());
		m_rt_logic->addActivity( act );
	}
	m_closed = true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
std::string RDOPROCProcess::s_name_prefix = "";
std::string RDOPROCProcess::s_name_sufix  = "s";

RDOPROCProcess::RDOPROCProcess( RDOParser* _parser, const std::string& name ):
	RDOParserObject( _parser ),
	m_name( name ),
	m_closed( false ),
	m_parent( NULL ),
	m_runtime( NULL )
{
	parser()->insertPROCProcess( this );
	m_runtime = new rdoRuntime::RDOPROCProcess( m_name, parser()->runtime() );
}

void RDOPROCProcess::end()
{
	m_closed = true;
}

void RDOPROCProcess::insertChild( RDOPROCProcess* value )
{
	if ( value ) {
		m_child.push_back( value );
		value->m_parent = this;
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
	process->m_operations.push_back( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time ):
	RDOPROCOperator( _process, _name ),
	runtime( NULL )
{
	runtime = new rdoRuntime::RDOPROCGenerate( parser()->getLastPROCProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
bool RDOPROCBlockForSeize::checkType( RDOParser *parser, rdoMBuilder::RDOResType rtp, const RDOParserSrcInfo& info )
{
// "Состояние"
std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
// "Свободен"
std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
// "Занят"
std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

	// Тип найден, проверим его на наличие перечислимого параметра
	if ( !rtp.m_params[rtp_param_name].exist() )
	parser->error( rdo::format( "У типа ресурса '%s' нет параметра перечислимого типа '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
	
	const rdoMBuilder::RDOResType::Param& param = rtp.m_params[rtp_param_name];
	// Параметр Состояние есть, надо проверить, чтобы в нем были значения Свободен и Занят
	// Для начала проверим тип параметра
	if ( param.typeID() != rdoRuntime::RDOType::t_enum ) 
	parser->error( rdo::format( "У типа ресурса '%s' параметр '%s' не является параметром перечислимого типа", rtp.name().c_str(), rtp_param_name.c_str() ) );

	// Теперь проверим сами значения
	if ( !param.getEnum().exist(rtp_state_free) || !param.getEnum().exist(rtp_state_buzy) )
	parser->error( rdo::format( "У типа ресурса '%s' перечислимый параметр '%s' должен иметь как минимум два обязательных значения: %s и %s", rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );

	return true;
}

void RDOPROCBlockForSeize::createRes( RDOParser *parser, rdoMBuilder::RDOResType rtp, const std::string& res_name )
{
	// Получили список всех ресурсов
	rdoMBuilder::RDOResourceList rssList(parser);
	// Создадим ресурс
	rdoMBuilder::RDOResource rss(rtp, res_name);
	// Добавим его в систему
	rssList.append<rdoParse::RDOPROCResource>(rss);
}

void RDOPROCBlockForSeize::reobjectRes( RDOParser *parser, rdoMBuilder::RDOResType rtp, const std::string& res_name )
{
	// Получили список всех ресурсов
	rdoMBuilder::RDOResourceList rssList(parser);
	// Создадим ресурс
	rdoMBuilder::RDOResource rssNew(rtp, res_name);
	// Добавим его в систему
	rssList.replace<rdoParse::RDOPROCResource>(rssNew);
}

rdoMBuilder::RDOResType RDOPROCBlockForSeize::createType( RDOParser *parser, const std::string& rtp_name, const RDOParserSrcInfo& info )
{
	// "Состояние"
	std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "Свободен"
	std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	// "Занят"
	std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

	// Получили список всех типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList( parser );
	// Создадим тип ресурса
	rdoMBuilder::RDOResType rtp(rtp_name);
	// Создадим параметр перечислимого типа
	rtp.m_params.append( rdoMBuilder::RDOResType::Param(rtp_param_name, new rdoRuntime::RDOEnumType(parser->runtime(), rdoRuntime::RDOEnumType::Enums(rtp_state_free)(rtp_state_buzy)), rtp_state_free) );
	// Добавим тип ресурса
	if ( !rtpList.append( rtp ) )
	{
		parser->error( info, rdo::format("Ошибка создания типа ресурса: %s", rtp_name.c_str()) );
	}
	return rtp;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
void RDOPROCSeize::create_runtime_Seize( RDOParser *parser )
{
const RDORSSResource* rss = parser->findRSSResource(Res);
	if( rss ){
		const std::string res_name = rss->name();
		// Получили список всех ресурсов
		rdoMBuilder::RDOResourceList rssList( parser );
		// Создадим тип ресурса
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		// "Состояние"
		std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	
		parser_for_runtime.Id_res = rss->getID();
		parser_for_runtime.Id_param = rtp.m_params[rtp_param_name].id(); 
	}	
	else {
	parser->error( "Внутренняя ошибка RDOPROCSeize: не нашли parser-ресурс" );
	}
runtime = new rdoRuntime::RDOPROCSeize( parser->getLastPROCProcess()->getRunTime(), parser_for_runtime );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
void RDOPROCRelease::create_runtime_Release( RDOParser *parser )
{
const RDORSSResource* rss = parser->findRSSResource(Res);
	if( rss ){
		const std::string res_name = rss->name();
		// Получили список всех ресурсов
		rdoMBuilder::RDOResourceList rssList( parser );
		// Создадим тип ресурса
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		// "Состояние"
		std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	
		parser_for_runtime.Id_res = rss->getID();
		parser_for_runtime.Id_param = rtp.m_params[rtp_param_name].id(); 
	}
	else {
	parser->error( "Внутренняя ошибка RDOPROCRelease: не нашли parser-ресурс" );
	}
runtime = new rdoRuntime::RDOPROCRelease( parser->getLastPROCProcess()->getRunTime(), parser_for_runtime );
}


// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time ):
	RDOPROCOperator( _process, _name )
{
	runtime = new rdoRuntime::RDOPROCAdvance( parser()->getLastPROCProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name ):
	RDOPROCOperator( _process, _name )
{
	runtime = new rdoRuntime::RDOPROCTerminate( parser()->getLastPROCProcess()->getRunTime() );
}

} // namespace rdoParse
