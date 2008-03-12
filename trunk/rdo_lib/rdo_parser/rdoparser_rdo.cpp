#include "pch.h"
#include "rdoparser_rdo.h"
#include "rdoparser_lexer.h"
#include "rdoparser.h"
#include "rdosmr.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdosmr.h"

#include "../rdo_simulator/rdosimwin.h"
#include <rdobinarystream.h>
#include <rdokernel.h>
#include <rdorepository.h>
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserRDO
// ----------------------------------------------------------------------------
RDOParserRDO::~RDOParserRDO()
{
	if ( m_lexer ) {
		delete m_lexer;
		m_lexer = NULL;
	}
}

void RDOParserRDO::parse()
{
	rdo::binarystream in_stream;
	kernel->simulator()->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &rdoRepository::RDOThreadRepository::FileData( type, in_stream ) );
	if ( in_stream.good() ) {
		parse( in_stream );
	}
}

void RDOParserRDO::parse( std::istream& in_stream )
{
	if ( m_lexer ) delete m_lexer;
	std::ostringstream out_stream;
	m_lexer = getLexer( in_stream, out_stream );
	if ( m_lexer && parser_fun ) parser_fun( m_lexer );
}

RDOLexer* RDOParserRDO::getLexer( std::istream& in_stream, std::ostream& out_stream )
{
	return new RDOLexer( parser, &in_stream, &out_stream );
}

int RDOParserRDO::lexer_loc_line()
{
	if ( m_lexer ) {
		return m_lexer->m_lploc ? m_lexer->m_lploc->first_line : m_lexer->lineno();
	} else return -1;
}

int RDOParserRDO::lexer_loc_pos()
{
	return m_lexer && m_lexer->m_lploc ? m_lexer->m_lploc->first_column : 0;
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
RDOParserRSS::RDOParserRSS( RDOParser* _parser ): RDOParserRDO( _parser, rdoModelObjects::RSS, rssparse, rsserror, rsslex )
{
}

void RDOParserRSS::parse( std::istream& in_stream )
{
	parser->setHaveKWResources( false );
	parser->setHaveKWResourcesEnd( false );
	RDOParserRDO::parse( in_stream );
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
void RDOParserRSSPost::parse()
{
	// В режиме совместимости со старым РДО создаем ресурсы по номерам их типов, а не по номерам самих ресурсов из RSS
#ifdef RDOSIM_COMPATIBLE
	std::vector< RDORTPResType* >::const_iterator rtp_it = parser->getRTPResType().begin();
	while ( rtp_it != parser->getRTPResType().end() ) {
#endif
		std::vector< RDORSSResource* >::const_iterator rss_it = parser->getRSSResources().begin();
		while ( rss_it != parser->getRSSResources().end() ) {
#ifdef RDOSIM_COMPATIBLE
			if ( (*rss_it)->getType() == *rtp_it ) {
#endif
				rdoRuntime::RDOCalc* calc = (*rss_it)->createCalc();
				parser->runtime->addInitCalc( calc );
#ifdef RDOSIM_COMPATIBLE
			}
#endif
			rss_it++;
		}
#ifdef RDOSIM_COMPATIBLE
		rtp_it++;
	}
#endif
}

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
void RDOParserSTDFUN::parse()
{
	RDORTPIntParamType* intType   = new RDORTPIntParamType( parser, new RDORTPIntDiap(parser), new RDORTPIntDefVal(parser) );
	RDORTPRealParamType* realType = new RDORTPRealParamType( parser, new RDORTPRealDiap(parser), new RDORTPRealDefVal(parser) );

	RDOFUNFunction* fun = new RDOFUNFunction( parser, "Abs", realType );
	RDOFUNFunctionParam* param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcAbs( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "ArcCos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcCos( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "ArcSin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcSin( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "ArcTan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcTan( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Cos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcCos( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Cotan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcCotan( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Exp", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcExp( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Floor", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcFloor( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Frac", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcFrac( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "IAbs", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIAbs( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "IMax", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIMax( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "IMin", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIMin( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Int", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcInt( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "IntPower", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIntPower( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Ln", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLn( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Log10", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLog10( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Log2", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLog2( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "LogN", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLogN( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Max", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcMax( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Min", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcMin( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Power", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcPower( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Round", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcRound( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Sin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcSin( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Sqrt", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcSqrt( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "Tan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcTan( parser->runtime ) );

	// -----------------------------------------
	// И для маленьких букв
	// -----------------------------------------
	fun = new RDOFUNFunction( parser, "abs", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcAbs( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "arccos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcCos( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "arcsin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcSin( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "arctan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcTan( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "cos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcCos( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "cotan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcCotan( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "exp", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcExp( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "floor", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcFloor( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "frac", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcFrac( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "iabs", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIAbs( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "imax", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIMax( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "imin", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIMin( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "int", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcInt( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "intpower", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIntPower( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "ln", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLn( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "log10", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLog10( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "log2", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLog2( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "logn", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLogN( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "max", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcMax( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "min", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcMin( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "power", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcPower( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "round", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcRound( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "sin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcSin( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "sqrt", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcSqrt( parser->runtime ) );

	fun = new RDOFUNFunction( parser, "tan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcTan( parser->runtime ) );
}

// ----------------------------------------------------------------------------
// ---------- RDOParserSMR1
// ----------------------------------------------------------------------------
RDOParserSMR1::RDOParserSMR1( RDOParser* _parser ): RDOParserRDO( _parser, rdoModelObjects::SMR, smr1parse, smr1error, smr1lex )
{
}

void RDOParserSMR1::parse( std::istream& in_stream )
{
	try {
		RDOParserRDO::parse( in_stream );
	} catch ( RDOSMR1OkException& ) {
		// Everithing ok, just end of first part parsing
	}
}

} // namespace rdoParse
