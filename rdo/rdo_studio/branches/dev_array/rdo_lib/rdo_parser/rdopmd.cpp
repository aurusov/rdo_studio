#include "pch.h"
#include "rdopmd.h"
#include "rdoparser.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdoparser_lexer.h"
#include <rdocalc.h>

namespace rdoParse 
{

int pmdlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void pmderror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokaz::RDOPMDPokaz(PTR(RDOParser) _parser, CREF(RDOParserSrcInfo) _src_info)
	: RDOParserObject (_parser  )
	, RDOParserSrcInfo(_src_info)
{
	const RDOPMDPokaz* pokaz = parser()->findPMDPokaz( src_text() );
	if ( pokaz ) {
		parser()->error_push_only( src_info(), rdo::format("Показатель '%s' уже существует", src_text().c_str()) );
		parser()->error_push_only( pokaz->src_info(), "См. первое определение" );
		parser()->error_push_done();
	}
}

void RDOPMDPokaz::endOfCreation(CREF(LPIPokaz) pokaz)
{
	m_pokaz = pokaz;
	LPITrace trace = m_pokaz;
	if (trace)
		trace->setTraceID(parser()->getPMD_id());
	parser()->insertPMDPokaz( this );
	//todo: перенести в конструктор rdoRuntime::RDOPMDPokaz
	parser()->runtime()->addRuntimePokaz(m_pokaz);
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, const RDOParserSrcInfo& _res_src_info, const RDOParserSrcInfo& _par_src_info ):
	RDOPMDPokaz( _parser, _src_info )
{
	const RDORSSResource* const res = parser()->findRSSResource( _res_src_info.src_text() );
	if ( !res ) {
		parser()->error( _res_src_info, rdo::format("Ресурс '%s' не найден", _res_src_info.src_text().c_str()) );
	}
/*
	if ( !res->getType()->isPermanent() ) {
		parser()->error_push_only( _res_src_info, "Наблюдать (watch_par) можно только за параметром постоянного ресурса" );
		parser()->error_push_only( res->getType()->src_info(), "См. тип ресурса" );
		parser()->error_push_done();
//		parser()->error("Resource must be of permanent type: " + _resName);
	}
*/
	const RDORTPParam* const par = res->getType()->findRTPParam( _par_src_info.src_text() );
	if ( !par ) {
		parser()->error_push_only( _par_src_info, rdo::format("Параметр '%s' не найден", _par_src_info.src_text().c_str()) );
		parser()->error_push_only( res->src_info(), "См. ресурс" );
		parser()->error_push_only( res->getType()->src_info(), "См. тип ресурса" );
		parser()->error_push_done();
	}
	rdoRuntime::RDOType::TypeID typeID = par->getType()->typeID();
	if ( typeID != rdoRuntime::RDOType::t_int && typeID != rdoRuntime::RDOType::t_real ) {
		parser()->error_push_only( _par_src_info, "Наблюдать можно только за параметром целого или вещественного типа" );
		parser()->error_push_only( par->getType()->src_info(), "См. тип параметра" );
		parser()->error_push_done();
	}
	endOfCreation(F(rdoRuntime::RDOPMDWatchPar)::create(parser()->runtime(), src_text(), _trace, _res_src_info.src_text(), _par_src_info.src_text(), res->getID(), res->getType()->getRTPParamNumber(_par_src_info.src_text())));
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, RDOFUNLogic* _logic ):
	RDOPMDPokaz( _parser, _src_info )
{
	endOfCreation(F(rdoRuntime::RDOPMDWatchState)::create(parser()->runtime(), src_text(), _trace, _logic->getCalc()));
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchTemp
// ----------------------------------------------------------------------------
RDOPMDWatchTemp::RDOPMDWatchTemp( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _res_type_src_info ):
	RDOPMDPokaz( _parser, _src_info )
{
	const RDORTPResType* const res_type = parser()->findRTPResType( _res_type_src_info.src_text() );
	if ( !res_type ) {
		parser()->error( _res_type_src_info, rdo::format("Тип ресурса '%s' не найден", _res_type_src_info.src_text().c_str()) );
	}
	if ( !res_type->isTemporary() ) {
		parser()->error_push_only( _res_type_src_info, "Показатель собирает информацию по временным ресурсам (temporary)" );
		parser()->error_push_only( res_type->src_info(), "См. тип ресурса" );
		parser()->error_push_done();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, const RDOParserSrcInfo& _res_type_src_info ):
	RDOPMDWatchTemp( _parser, _src_info, _res_type_src_info )
{
	RDOFUNGroupLogic* fgl = new RDOFUNGroupLogic( this, RDOFUNGroupLogic::fgt_unknow, _res_type_src_info.src_text() );
	endOfCreation(F(rdoRuntime::RDOPMDWatchQuant)::create(parser()->runtime(), src_text(), _trace, _res_type_src_info.src_text(), fgl->resType->getNumber()));
}

void RDOPMDWatchQuant::setLogic( RDOFUNLogic* _logic )
{
	LPIPokazWatchQuant quant = m_pokaz;
	ASSERT(quant);
	quant->setLogicCalc(_logic->getCalc());
	parser()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	LPIPokazWatchQuant quant = m_pokaz;
	ASSERT(quant);
	quant->setLogicCalc(new rdoRuntime::RDOCalcConst(parser()->runtime(), 1));
	parser()->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, const RDOParserSrcInfo& _res_type_src_info ):
	RDOPMDWatchTemp( _parser, _src_info, _res_type_src_info )
{
	RDOFUNGroupLogic* fgl = new RDOFUNGroupLogic( this, RDOFUNGroupLogic::fgt_unknow, _res_type_src_info.src_text() );
	endOfCreation(F(rdoRuntime::RDOPMDWatchValue)::create(parser()->runtime(), src_text(), _trace, _res_type_src_info.src_text(), fgl->resType->getNumber()));
}

void RDOPMDWatchValue::setLogic( RDOFUNLogic* _logic, RDOFUNArithm* _arithm )
{
	LPIPokazWatchValue watch = m_pokaz;
	ASSERT(watch);
	watch->setLogicCalc (_logic->getCalc()    );
	watch->setArithmCalc(_arithm->createCalc());
	parser()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck( RDOFUNArithm* _arithm )
{
	LPIPokazWatchValue watch = m_pokaz;
	ASSERT(watch);
	watch->setLogicCalc (new rdoRuntime::RDOCalcConst(parser()->runtime(), 1));
	watch->setArithmCalc(_arithm->createCalc());
	parser()->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDOFUNArithm* _arithm ):
	RDOPMDPokaz( _parser, _src_info )
{
	endOfCreation(F(rdoRuntime::RDOPMDGetValue)::create(parser()->runtime(), src_text(), _arithm->createCalc()));
}

} // namespace rdoParse
