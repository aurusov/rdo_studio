#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdosmr.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdoparser_rdo.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"

namespace rdoParse 
{

int smr_file_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void smr_file_error(PTR(char) mes)
{}

int smr_sim_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void smr_sim_error(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOSMR
// ----------------------------------------------------------------------------
RDOSMR::RDOSMR( RDOParser* _parser, const std::string& _modelName ):
	RDOParserObject( _parser ),
	frameNumber( 1 ),
	showRate( 60 ),
	runStartTime( 0 ),
	traceStartTime( rdoRuntime::RDOSimulatorTrace::UNDEFINE_TIME ),
	traceEndTime  ( rdoRuntime::RDOSimulatorTrace::UNDEFINE_TIME ),
	showMode( rdoSimulator::SM_NoShow ),
	terminateIf( NULL )
{
	setFile( "Model_name", _modelName );
	parser()->setSMR( this );
}

void RDOSMR::setShowMode( rdoSimulator::ShowMode _showMode )
{
	showMode = _showMode;
}

void RDOSMR::setFrameNumber( int value, const YYLTYPE& pos )
{
	if ( value <= 0 ) {
		parser()->error().error( pos, "Номер кадра должен быть больше нуля" );
	}
	if ( parser()->runtime()->allFrames.size() + 1 <= (unsigned int)value ) {
		parser()->error().error( pos, rdo::format("Несуществующий кадр: %d", value) );
	}
	frameNumber = value;
}

void RDOSMR::setShowRate( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		parser()->error().error( pos, "Масштаб должен быть больше нуля" );
	}
	showRate = value;
}

void RDOSMR::setRunStartTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		parser()->error().error( pos, "Начальное модельное время должно быть больше нуля" );
	}
	runStartTime = value;
}

void RDOSMR::setTraceStartTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		parser()->error().error( pos, "Начальное время трассировки должно быть больше нуля" );
	}
	if ( getTraceEndTime() != rdoRuntime::RDOSimulatorTrace::UNDEFINE_TIME && getTraceEndTime() <= value ){
		parser()->error().push_only( pos, "Начальное время трассировки должно быть меньше конечного" );
		parser()->error().push_only( traceEndTime_pos, "См. конечное время трассировки" );
		parser()->error().push_done();
	}
	traceStartTime     = value;
	traceStartTime_pos = pos;
}

void RDOSMR::setTraceEndTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		parser()->error().error( pos, "Конечное время трассировки должно быть больше нуля" );
	}
	if ( getTraceStartTime() != rdoRuntime::RDOSimulatorTrace::UNDEFINE_TIME && getTraceStartTime() >= value ) {
		parser()->error().push_only( pos, "Конечное время трассировки должно быть больше начального" );
		parser()->error().push_only( traceStartTime_pos, "См. начальное время трассировки" );
		parser()->error().push_done();
	}
	traceEndTime     = value;
	traceEndTime_pos = pos;
}

void RDOSMR::setTerminateIf( RDOFUNLogic* logic )
{
	if ( terminateIf ) {
		parser()->error().push_only( logic->src_info(), "Terminate_if уже определен" );
	//	parser()->error().push_only( logic->src_info(), "Second Terminate_if entry" );
		parser()->error().push_only( terminateIf->src_info(), "См. первое определение" );
		parser()->error().push_done();
	}
	terminateIf = logic;
	parser()->runtime()->setTerminateIf( logic->getCalc() );
}

void RDOSMR::setConstValue( const RDOParserSrcInfo& const_info, RDOFUNArithm* arithm )
{
	const RDOFUNConstant* const cons = parser()->findFUNConstant( const_info.src_text() );
	if ( !cons ) {
		parser()->error().error( const_info, rdo::format("Константа '%s' не найдена", const_info.src_text().c_str()) );
	}
	ASSERT(arithm);
	arithm->checkParamType(cons->getType());
	rdoRuntime::LPRDOCalc calc = arithm->createCalc( cons->getType() );
	parser()->runtime()->addInitCalc(rdo::Factory<rdoRuntime::RDOCalcSetConst>::create(cons->getNumber(), calc).object_cast<rdoRuntime::RDOCalc>());
	parser()->insertChanges( cons->src_text(), arithm->src_text() );
}

void RDOSMR::setResParValue( const RDOParserSrcInfo& res_info, const RDOParserSrcInfo& par_info, RDOFUNArithm* arithm )
{
	const RDORSSResource* res = parser()->findRSSResource( res_info.src_text() );
	if ( !res ) {
		parser()->error().error( res_info.src_info(), rdo::format("Ресурс '%s' не найден", res_info.src_text().c_str()) );
//		parser()->error().error( res_info.src_info(), "Undefined resource name: " + resName );
	}
	LPRDORTPParam param = res->getType()->findRTPParam( par_info.src_text() );
	if ( !param ) {
		parser()->error().push_only( par_info.src_info(), rdo::format("Параметр '%s' не найден", par_info.src_text().c_str()) );
		parser()->error().push_only( res->src_info(), "См. ресурс" );
		parser()->error().push_only( res->getType()->src_info(), "См. тип ресурса" );
		parser()->error().push_done();
//		parser()->error().error( par_info.src_info(), "Undefined resource parameter name: " + parName);
	}
	ASSERT(arithm);
	arithm->checkParamType(param->getParamType());
	unsigned int parNumb = res->getType()->getRTPParamNumber( par_info.src_text() );
	rdoRuntime::LPRDOCalc calc = arithm->createCalc( param->getParamType() );
	parser()->runtime()->addInitCalc(rdo::Factory<rdoRuntime::RDOSetResourceParamCalc>::create(res->getID(), parNumb, calc).object_cast<rdoRuntime::RDOCalc>());
	parser()->insertChanges( res_info.src_text() + "." + par_info.src_text(), arithm->src_text() );
}

void RDOSMR::setSeed( const RDOParserSrcInfo& seq_info, int base )
{
	CPTR(RDOFUNSequence) seq = parser()->findFUNSequence( seq_info.src_text() );
	if ( !seq ) {
		parser()->error().error( seq_info, rdo::format("Последовательность '%s' не найдена", seq_info.src_text().c_str()) );
//		parser()->error().error( "Undefined sequence: " + seqName );
	}
	const_cast<PTR(RDOFUNSequence)>(seq)->init_calc->setBase( base );
	parser()->insertChanges( seq->src_text() + ".Seed", rdo::format("%d", base) );
}

void RDOSMR::insertBreakPoint( const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic )
{
	std::vector< BreakPoint* >::const_iterator it = breakPoints.begin();
	while ( it != breakPoints.end() ) {
		if ( (*it)->src_text() == _src_info.src_text() ) {
			parser()->error().push_only( _src_info, rdo::format("Точка останова с именем '%s' уже существует", _src_info.src_text().c_str()) );
			parser()->error().push_only( (*it)->src_info(), "См. первое определение" );
			parser()->error().push_done();
		}
		it++;
	}
	breakPoints.push_back( new BreakPoint( this, _src_info, _logic ) );
}

RDOSMR::BreakPoint::BreakPoint( RDOSMR* smr, const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic ):
	RDOParserObject( smr ),
	RDOParserSrcInfo( _src_info )
{
	parser()->runtime()->insertBreakPoint( src_text(), _logic->getCalc() );
}

} // namespace rdoParse 
