#include "pch.h"
#include "rdosmr.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdoparser_lexer.h"
#include "rdoparser_rdo.h"
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int smr1lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void smr1error( char* mes )
{
//	throw RDOSMR1OkException("");
//	rdoParse::parser->error( mes );
}

int smr2lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void smr2error( char* mes )
{
//	rdoParse::parser->error( mes );
}

// ----------------------------------------------------------------------------
// ---------- RDOSMR
// ----------------------------------------------------------------------------
RDOSMR::RDOSMR( RDOParser* _parser, const std::string& _modelName ):
	RDOParserObject( _parser ),
	frameNumber( 1 ),
	showRate( 60 ),
	runStartTime( 0 ),
	traceStartTime( -1 ),
	traceEndTime( -1 ),
	showMode( rdoSimulator::SM_NoShow ),
	terminateIf( NULL )
{
	setFile( "Model_name", _modelName );
	getParser()->setSMR( this );
}

void RDOSMR::setShowMode( rdoSimulator::ShowMode _showMode )
{
	showMode = _showMode;
}

void RDOSMR::setFrameNumber( int value, const YYLTYPE& pos )
{
	if ( value <= 0 ) {
		getParser()->error( pos, "Номер кадра должен быть больше нуля" );
	}
	if ( getParser()->runtime->allFrames.size() + 1 <= value ) {
		getParser()->error( pos, rdo::format("Несуществующий кадр: %d", value) );
	}
	frameNumber = value;
}

void RDOSMR::setShowRate( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		getParser()->error( pos, "Масштаб должен быть больше нуля" );
	}
	showRate = value;
}

void RDOSMR::setRunStartTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		getParser()->error( pos, "Начальное модельное время должно быть больше нуля" );
	}
	runStartTime = value;
}

void RDOSMR::setTraceStartTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		getParser()->error( pos, "Начальное время трассировки должно быть больше нуля" );
	}
	if ( getTraceEndTime() != -1 && getTraceEndTime() <= value ) {
		getParser()->error_push_only( pos, "Начальное время трассировки должно быть меньше конечного" );
		getParser()->error_push_only( traceEndTime_pos, "См. конечное время трассировки" );
		getParser()->error_push_done();
	}
	traceStartTime     = value;
	traceStartTime_pos = pos;
}

void RDOSMR::setTraceEndTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		getParser()->error( pos, "Конечное время трассировки должно быть больше нуля" );
	}
	if ( getTraceStartTime() != -1 && getTraceStartTime() >= value ) {
		getParser()->error_push_only( pos, "Конечное время трассировки должно быть больше начального" );
		getParser()->error_push_only( traceStartTime_pos, "См. начальное время трассировки" );
		getParser()->error_push_done();
	}
	traceEndTime     = value;
	traceEndTime_pos = pos;
}

void RDOSMR::setTerminateIf( RDOFUNLogic* logic )
{
	if ( terminateIf ) {
		getParser()->error_push_only( logic->src_info(), "Terminate_if уже определен" );
	//	getParser()->error_push_only( logic->src_info(), "Second Terminate_if entry" );
		getParser()->error_push_only( terminateIf->src_info(), "См. первое определение" );
		getParser()->error_push_done();
	}
	terminateIf = logic;
	getParser()->runtime->setTerminateIf( logic->createCalc() );
}

void RDOSMR::setConstValue( const RDOParserSrcInfo& const_info, RDOFUNArithm* arithm )
{
	const RDOFUNConstant* const cons = getParser()->findFUNConst( const_info.src_text() );
	if ( !cons ) {
		getParser()->error( const_info, rdo::format("Константа '%s' не найдена", const_info.src_text().c_str()) );
	}
	cons->getType()->checkParamType( arithm, false );
	rdoRuntime::RDOCalc* calc = arithm->createCalc( cons->getType() );
	getParser()->runtime->addInitCalc( new rdoRuntime::RDOCalcSetConst( getParser()->runtime, cons->number, calc ) );
	getParser()->insertChanges( cons->src_text(), arithm->src_text() );
}

void RDOSMR::setResParValue( const RDOParserSrcInfo& res_info, const RDOParserSrcInfo& par_info, RDOFUNArithm* arithm )
{
	const RDORSSResource* res = getParser()->findRSSResource( res_info.src_text() );
	if ( !res ) {
		getParser()->error( res_info.src_info(), rdo::format("Ресурс '%s' не найден", res_info.src_text().c_str()) );
//		getParser()->error( res_info.src_info(), "Undefined resource name: " + resName );
	}
	const RDORTPParam* param = res->getType()->findRTPParam( par_info.src_text() );
	if ( !param ) {
		getParser()->error_push_only( par_info.src_info(), rdo::format("Параметр '%s' не найден", par_info.src_text().c_str()) );
		getParser()->error_push_only( res->src_info(), "См. ресурс" );
		getParser()->error_push_only( res->getType()->src_info(), "См. тип ресурса" );
		getParser()->error_push_done();
//		getParser()->error( par_info.src_info(), "Undefined resource parameter name: " + parName);
	}
	param->getType()->checkParamType( arithm, false );
	int parNumb = res->getType()->getRTPParamNumber( par_info.src_text() );
	rdoRuntime::RDOCalc* calc = arithm->createCalc( param->getType() );
	getParser()->runtime->addInitCalc( new rdoRuntime::RDOSetResourceParamCalc( getParser()->runtime, res->getNumber(), parNumb, calc ) );
	getParser()->insertChanges( res_info.src_text() + "." + par_info.src_text(), arithm->src_text() );
}

void RDOSMR::setSeed( const RDOParserSrcInfo& seq_info, int base )
{
	const RDOFUNSequence* seq = getParser()->findSequence( seq_info.src_text() );
	if ( !seq ) {
		getParser()->error( seq_info, rdo::format("Последовательность '%s' не найдена", seq_info.src_text().c_str()) );
//		getParser()->error( "Undefined sequence: " + seqName );
	}
	seq->init_calc->setBase( base );
	getParser()->insertChanges( seq->src_text() + ".Seed", rdo::format("%d", base) );
}

void RDOSMR::insertBreakPoint( const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic )
{
	std::vector< BreakPoint* >::const_iterator it = breakPoints.begin();
	while ( it != breakPoints.end() ) {
		if ( (*it)->src_text() == _src_info.src_text() ) {
			getParser()->error_push_only( _src_info, rdo::format("Точка останова с именем '%s' уже существует", _src_info.src_text().c_str()) );
			getParser()->error_push_only( (*it)->src_info(), "См. первое определение" );
			getParser()->error_push_done();
		}
		it++;
	}
	breakPoints.push_back( new BreakPoint( this, _src_info, _logic ) );
}

RDOSMR::BreakPoint::BreakPoint( RDOSMR* smr, const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic ):
	RDOParserObject( smr ),
	RDOParserSrcInfo( _src_info )
{
	getParser()->runtime->insertBreakPoint( src_text(), _logic->createCalc() );
}

} // namespace rdoParse 
