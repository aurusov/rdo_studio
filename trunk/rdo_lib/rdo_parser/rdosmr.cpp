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
	showMode( rdoSimulator::SM_NoShow ),
	frameNumber( 1 ),
	terminateIf(NULL),
	showModeSet(false),
	frameNumberSet(false)
{
	setFile( "Model_name", _modelName );
	getParser()->setSMR( this );
}

void RDOSMR::setShowMode(rdoSimulator::ShowMode sm)
{
	if(showModeSet)
		getParser()->error("Second appearence of Show_mode descriptor");

	showModeSet = true;
	showMode = sm;
}

void RDOSMR::setFrameNumber(int fn)
{
	if(frameNumberSet)
		getParser()->error("Second appearence of Frame_number descriptor");

	frameNumberSet = true;
	frameNumber = fn;
}

void RDOSMR::setTerminateIf(RDOFUNLogic *logic)
{
	if(!getParser()->runtime->setTerminateIf(logic->calc))
		getParser()->error("Second Terminate_if entry");
}

void RDOSMR::setConstValue( const std::string& constName, RDOFUNArithm* arithm )
{
	const RDOFUNConstant *cons = getParser()->findFUNConst(constName);
	if(!cons)
		getParser()->error("Undefined constant: " + constName);

	rdoRuntime::RDOCalc *calc = arithm->createCalc(cons->getType());
	getParser()->runtime->addInitCalc(new rdoRuntime::RDOCalcSetConst( getParser()->runtime, cons->number, calc ));
}

void RDOSMR::setResParValue( const std::string& resName, const std::string& parName, RDOFUNArithm* arithm )
{
	const RDORSSResource *res = getParser()->findRSSResource(resName);
	if(!res)
		getParser()->error("Undefined resource name: " + resName);

	const RDORTPParam *descr = res->getType()->findRTPParam(parName);
	if(!descr)
		getParser()->error("Undefined resource parameter name: " + parName);

	int parNumb = res->getType()->getRTPParamNumber(parName);
	rdoRuntime::RDOCalc *calc = arithm->createCalc(descr->getType());
	getParser()->runtime->addInitCalc(new rdoRuntime::RDOSetResourceParamCalc( getParser()->runtime, res->getNumber(), parNumb, calc ));
}

void RDOSMR::setSeed( const std::string& seqName, int _base )
{
	const RDOFUNSequence* seq = getParser()->findSequence( seqName );
	if ( !seq ) {
		getParser()->error( "Undefined sequence: " + seqName );
	}
	seq->init_calc->setBase( _base );
}

} // namespace rdoParse 
