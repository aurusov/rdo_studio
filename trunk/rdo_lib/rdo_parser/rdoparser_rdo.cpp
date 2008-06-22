#include "pch.h"
#include "rdoparser_rdo.h"
#include "rdoparser_lexer.h"
#include "rdoparser.h"
#include "rdosmr.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdosmr.h"

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
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem( RDOParser* _parser, rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun, StreamFrom from ):
	RDOParserItem( _parser, _type, _parser_fun, _error_fun, _lexer_fun, from ),
	m_lexer( NULL )
{
};

RDOParserRDOItem::~RDOParserRDOItem()
{
	if ( m_lexer ) {
		delete m_lexer;
		m_lexer = NULL;
	}
}

void RDOParserRDOItem::parse()
{
	rdo::binarystream in_stream;
	switch ( m_from )
	{
		case sf_repository:
			kernel->sendMessage( kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &rdoRepository::RDOThreadRepository::FileData( m_type, in_stream ) );
			break;
		case sf_editor:
			kernel->sendMessage( kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &rdoRepository::RDOThreadRepository::FileData( m_type, in_stream ) );
			break;
	}
	if ( in_stream.good() ) {
		parse( in_stream );
	}
}

void RDOParserRDOItem::parse( std::istream& in_stream )
{
	if ( m_lexer ) delete m_lexer;
	std::ostringstream out_stream;
	m_lexer = getLexer( &in_stream, &out_stream );
	if ( m_lexer && m_parser_fun ) m_parser_fun( m_lexer );
}

RDOLexer* RDOParserRDOItem::getLexer( std::istream* in_stream, std::ostream* out_stream )
{
	return new RDOLexer( m_parser, in_stream, out_stream );
}

int RDOParserRDOItem::lexer_loc_line()
{
	if ( m_lexer )
	{
		return m_lexer->m_lploc ? m_lexer->m_lploc->first_line : m_lexer->lineno();
	}
	else
	{
		return rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE;
	}
}

int RDOParserRDOItem::lexer_loc_pos()
{
	return m_lexer && m_lexer->m_lploc ? m_lexer->m_lploc->first_column : 0;
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
RDOParserRSS::RDOParserRSS( RDOParser* _parser, StreamFrom from ): RDOParserRDOItem( _parser, rdoModelObjects::RSS, rssparse, rsserror, rsslex, from )
{
}

void RDOParserRSS::parse()
{
	m_parser->setHaveKWResources( false );
	m_parser->setHaveKWResourcesEnd( false );
	RDOParserRDOItem::parse();
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
void RDOParserRSSPost::parse()
{
	// В режиме совместимости со старым РДО создаем ресурсы по номерам их типов, а не по номерам самих ресурсов из RSS
#ifdef RDOSIM_COMPATIBLE
	std::vector< RDORTPResType* >::const_iterator rtp_it = m_parser->getRTPResType().begin();
	while ( rtp_it != m_parser->getRTPResType().end() ) {
#endif
		std::vector< RDORSSResource* >::const_iterator rss_it = m_parser->getRSSResources().begin();
		while ( rss_it != m_parser->getRSSResources().end() ) {
#ifdef RDOSIM_COMPATIBLE
			if ( (*rss_it)->getType() == *rtp_it ) {
#endif
				rdoRuntime::RDOCalc* calc = (*rss_it)->createCalc();
				m_parser->runtime()->addInitCalc( calc );
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
	RDORTPIntParamType* intType   = new RDORTPIntParamType( m_parser, new RDORTPIntDiap(m_parser), new RDORTPDefVal(m_parser) );
	RDORTPRealParamType* realType = new RDORTPRealParamType( m_parser, new RDORTPRealDiap(m_parser), new RDORTPDefVal(m_parser) );

	RDOFUNFunction* fun = new RDOFUNFunction( m_parser, "Abs", realType );
	RDOFUNFunctionParam* param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcAbs( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "ArcCos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcCos( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "ArcSin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcSin( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "ArcTan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcTan( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Cos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcCos( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Cotan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcCotan( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Exp", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcExp( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Floor", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcFloor( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Frac", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcFrac( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "IAbs", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIAbs( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "IMax", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIMax( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "IMin", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIMin( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Int", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcInt( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "IntPower", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIntPower( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Ln", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLn( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Log10", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLog10( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Log2", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLog2( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "LogN", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLogN( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Max", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcMax( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Min", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcMin( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Power", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcPower( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Round", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcRound( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Sin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcSin( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Sqrt", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcSqrt( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "Tan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcTan( m_parser->runtime() ) );

	// -----------------------------------------
	// И для маленьких букв
	// -----------------------------------------
	fun = new RDOFUNFunction( m_parser, "abs", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcAbs( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "arccos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcCos( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "arcsin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcSin( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "arctan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcArcTan( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "cos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcCos( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "cotan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcCotan( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "exp", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcExp( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "floor", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcFloor( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "frac", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcFrac( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "iabs", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIAbs( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "imax", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIMax( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "imin", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIMin( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "int", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcInt( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "intpower", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcIntPower( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "ln", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLn( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "log10", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLog10( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "log2", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLog2( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "logn", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcLogN( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "max", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcMax( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "min", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcMin( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "power", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcPower( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "round", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcRound( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "sin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcSin( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "sqrt", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcSqrt( m_parser->runtime() ) );

	fun = new RDOFUNFunction( m_parser, "tan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->setFunctionCalc( new rdoRuntime::RDOFunCalcTan( m_parser->runtime() ) );
}

} // namespace rdoParse
