#include "pch.h"
#include "rdopmd.h"
#include "rdoparser.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdoparser_lexer.h"
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
RDOPMDPokaz::RDOPMDPokaz( RDOParser* _parser ):
	RDOParserObject( _parser ),
	pokaz_runtime( NULL )
{
}

void RDOPMDPokaz::endOfCreation( rdoRuntime::RDOPMDPokaz* _pokaz_runtime )
{
	pokaz_runtime     = _pokaz_runtime;
	pokaz_runtime->setID( getParser()->getPMD_id() );
	getParser()->insertPMDPokaz( this );
	//todo: перенести в конструктор rdoRuntime::RDOPMDPokaz
	getParser()->runtime->addRuntimePokaz( pokaz_runtime );
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar( RDOParser* _parser, std::string* _name, bool _trace, const std::string& _resName, const std::string& _parName ):
	RDOPMDPokaz( _parser )
{
	const RDORSSResource *const res = getParser()->findRSSResource(_resName);
	if(!res)
		getParser()->error("Undefined resource name: " + _resName);

	if( !res->getType()->isPermanent() )
		getParser()->error("Resource must be of permanent type: " + _resName);

	const RDORTPParam *const par = res->getType()->findRTPParam(_parName);
	if(!par)
		getParser()->error("Undefined parameter name: " + _parName + " for resource " + _resName);

	int type = par->getType()->getType();
	if ( type != RDORTPParamType::pt_int && type != RDORTPParamType::pt_real ) {
		getParser()->error("Enumerative parameter: " + _resName + "." + _parName + " not allowed in watch_par statement");
	}

	rdoRuntime::RDOPMDWatchPar* pokaz = new rdoRuntime::RDOPMDWatchPar( getParser()->runtime, _name, _trace, _resName, _parName, res->getNumber(), res->getType()->getRTPParamNumber(_parName) );
	endOfCreation( pokaz );
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState( RDOParser* _parser, std::string* _name, bool _trace, RDOFUNLogic* _logic ):
	RDOPMDPokaz( _parser )
{
	endOfCreation( new rdoRuntime::RDOPMDWatchState( getParser()->runtime, _name, _trace, _logic->calc ) );
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant( RDOParser* _parser, std::string* _name, bool _trace, const std::string& _resTypeName ):
	RDOPMDPokaz( _parser )
{
	RDOFUNGroupLogic* fgl = new RDOFUNGroupLogic( this, 5, _resTypeName );
	rdoRuntime::RDOPMDWatchQuant* pokaz = new rdoRuntime::RDOPMDWatchQuant( getParser()->runtime, _name, _trace, _resTypeName, fgl->resType->getNumber() );
	endOfCreation( pokaz );
}

void RDOPMDWatchQuant::setLogic( RDOFUNLogic* _logic )
{
	static_cast<rdoRuntime::RDOPMDWatchQuant*>(pokaz_runtime)->setLogicCalc( _logic->calc );
	getParser()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	static_cast<rdoRuntime::RDOPMDWatchQuant*>(pokaz_runtime)->setLogicCalc( new rdoRuntime::RDOCalcConst( getParser()->runtime, 1 ) );
	getParser()->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue( RDOParser* _parser, std::string* _name, bool _trace, const std::string& _resTypeName ):
	RDOPMDPokaz( _parser )
{
	RDOFUNGroupLogic* fgl = new RDOFUNGroupLogic( this, 5, _resTypeName );
	rdoRuntime::RDOPMDWatchValue* pokaz = new rdoRuntime::RDOPMDWatchValue( getParser()->runtime, _name, _trace, _resTypeName, fgl->resType->getNumber() );
	endOfCreation( pokaz );
}

void RDOPMDWatchValue::setLogic( RDOFUNLogic* _logic, RDOFUNArithm* _arithm )
{
	static_cast<rdoRuntime::RDOPMDWatchValue*>(pokaz_runtime)->setLogicCalc( _logic->calc );
	static_cast<rdoRuntime::RDOPMDWatchValue*>(pokaz_runtime)->setArithmCalc( _arithm->createCalc() );
	getParser()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck( RDOFUNArithm* _arithm )
{
	static_cast<rdoRuntime::RDOPMDWatchValue*>(pokaz_runtime)->setLogicCalc( new rdoRuntime::RDOCalcConst( getParser()->runtime, 1 ) );
	static_cast<rdoRuntime::RDOPMDWatchValue*>(pokaz_runtime)->setArithmCalc( _arithm->createCalc() );
	getParser()->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue( RDOParser* _parser, std::string* _name, RDOFUNArithm* _arithm ):
	RDOPMDPokaz( _parser )
{
	rdoRuntime::RDOPMDGetValue* pokaz = new rdoRuntime::RDOPMDGetValue( getParser()->runtime, _name, _arithm->createCalc() );
	endOfCreation( pokaz );
}

} // namespace rdoParse
