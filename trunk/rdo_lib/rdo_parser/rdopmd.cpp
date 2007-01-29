#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdopmd.h"
#include "rdoparser.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdocalcconst.h"
#include "rdoruntime.h"
#include "rdoparser_lexer.h"

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
//	rdoParse::parser->error( mes );
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokaz::RDOPMDPokaz( RDOParser* _parser ):
	RDOParserObject( _parser ),
	pokaz_runtime( NULL )
{
}

void RDOPMDPokaz::endOfCreation( rdoRuntime::RDOPMDPokaz* _pokaz_runtime )
{
	pokaz_runtime     = _pokaz_runtime;
	pokaz_runtime->id = parser->getPMD_id();
	parser->insertPMDPokaz( this );
	parser->runTime->addRuntimePokaz( pokaz_runtime );
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resName, std::string* _parName ):
	RDOPMDPokaz( _parser )
{
	const RDORSSResource *const res = parser->findRSSResource(_resName);
	if(!res)
		parser->error("Undefined resource name: " + *_resName);

	if( !res->getType()->isPermanent() )
		parser->error("Resource must be of permanent type: " + *_resName);

	const RDORTPParamDesc *const par = res->getType()->findRTPParam(_parName);
	if(!par)
		parser->error("Undefined parameter name: " + *_parName + " for resource " + *_resName);

	int type = par->getType()->getType();
	if ( type != RDORTPResParam::pt_int && type != RDORTPResParam::pt_real ) {
		parser->error("Enumerative parameter: " + *_resName + "." + *_parName + " not allowed in watch_par statement");
	}

	rdoRuntime::RDOPMDWatchPar* pokaz = new rdoRuntime::RDOPMDWatchPar( _name, _trace, _parName, _parName );
	pokaz->resNumber = res->getNumber();
	pokaz->parNumber = res->getType()->getRTPParamNumber(_parName);
	endOfCreation( pokaz );
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState( RDOParser* _parser, std::string* _name, bool _trace, RDOFUNLogic* _logic ):
	RDOPMDPokaz( _parser )
{
	endOfCreation( new rdoRuntime::RDOPMDWatchState( _name, _trace, _logic ) );
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resTypeName ):
	RDOPMDPokaz( _parser )
{
	rdoRuntime::RDOPMDWatchQuant* pokaz = new rdoRuntime::RDOPMDWatchQuant( _name, _trace, _resTypeName );
	pokaz->funGroup = new RDOFUNGroupLogic( this, 5, _resTypeName );
	endOfCreation( pokaz );
}

void RDOPMDWatchQuant::setLogic( RDOFUNLogic* _logic )
{
	static_cast<rdoRuntime::RDOPMDWatchQuant*>(pokaz_runtime)->logicCalc = _logic->calc;
	parser->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	static_cast<rdoRuntime::RDOPMDWatchQuant*>(pokaz_runtime)->logicCalc = new rdoRuntime::RDOCalcConst(1);
	parser->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resTypeName ):
	RDOPMDPokaz( _parser )
{
	rdoRuntime::RDOPMDWatchValue* pokaz = new rdoRuntime::RDOPMDWatchValue( _name, _trace, _resTypeName );
	pokaz->funGroup   = new RDOFUNGroupLogic( this, 5, _resTypeName );
	pokaz->wasChanged = false;
	endOfCreation( pokaz );
}

void RDOPMDWatchValue::setLogic( RDOFUNLogic* _logic, RDOFUNArithm* _arithm )
{
	static_cast<rdoRuntime::RDOPMDWatchValue*>(pokaz_runtime)->logicCalc  = _logic->calc;
	static_cast<rdoRuntime::RDOPMDWatchValue*>(pokaz_runtime)->arithmCalc = _arithm->createCalc();
	parser->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck( RDOFUNArithm* _arithm )
{
	static_cast<rdoRuntime::RDOPMDWatchValue*>(pokaz_runtime)->logicCalc  = new rdoRuntime::RDOCalcConst(1);
	static_cast<rdoRuntime::RDOPMDWatchValue*>(pokaz_runtime)->arithmCalc = _arithm->createCalc();
	parser->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue( RDOParser* _parser, std::string* _name, RDOFUNArithm* _arithm ):
	RDOPMDPokaz( _parser )
{
	rdoRuntime::RDOPMDGetValue* pokaz = new rdoRuntime::RDOPMDGetValue( _name, _arithm );
	endOfCreation( pokaz );
}

} // namespace rdoParse
