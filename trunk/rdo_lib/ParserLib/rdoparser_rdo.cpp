#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser_rdo.h"
#include "rdoparser_lexer.h"
#include "rdoparser.h"
#include "rdosmr.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdoruntime.h"

#include <rdobinarystream.h>
#include <rdokernel.h>
#include <rdorepository.h>

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
	kernel.getRepository()->load( type, in_stream );
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
	return new RDOLexer( this, &in_stream, &out_stream );
}

void RDOParserRDO::lexer_setvalue( int value )
{
	if ( m_lexer ) {
		*m_lexer->m_lpval = value;
	}
}

void RDOParserRDO::lexer_loc_init()
{
	if ( m_lexer && m_lexer->m_lploc ) {
		m_lexer->m_lploc->first_line   = 0;
		m_lexer->m_lploc->first_column = 0;
		m_lexer->m_lploc->last_line    = 0;
		m_lexer->m_lploc->last_column  = 0;
	}
}

void RDOParserRDO::lexer_loc_action()
{
	if ( m_lexer && m_lexer->m_lploc ) {
		m_lexer->m_lploc->first_line   = m_lexer->m_lploc->last_line;
		m_lexer->m_lploc->first_column = m_lexer->m_lploc->last_column;
		for ( int i = 0; i < m_lexer->YYLeng(); i++ ) {
			if ( m_lexer->YYText()[i] == '\n' ) {
				m_lexer->m_lploc->last_line++;
				m_lexer->m_lploc->last_column = 0;
			} else if ( m_lexer->YYText()[i] == '\r' ) {
				m_lexer->m_lploc->last_column = 0;
			} else {
				m_lexer->m_lploc->last_column++;
			}
		}
	}
}

void RDOParserRDO::lexer_loc_set( int error_line, int error_pos )
{
	if ( m_lexer && m_lexer->m_lploc ) {
		m_lexer->m_lploc->first_line   = error_line;
		m_lexer->m_lploc->first_column = error_pos;
		m_lexer->m_lploc->last_line    = error_line;
		m_lexer->m_lploc->last_column  = error_pos;
	}
}

void RDOParserRDO::lexer_loc_set( void* data )
{
	if ( m_lexer && m_lexer->m_lploc ) {
		if ( reinterpret_cast<YYLTYPE*>(data)->first_line == reinterpret_cast<YYLTYPE*>(data)->last_line ) {
			m_lexer->m_lploc->first_line   = reinterpret_cast<YYLTYPE*>(data)->last_line;
			m_lexer->m_lploc->first_column = reinterpret_cast<YYLTYPE*>(data)->last_column;
			m_lexer->m_lploc->last_line    = reinterpret_cast<YYLTYPE*>(data)->last_line;
			m_lexer->m_lploc->last_column  = reinterpret_cast<YYLTYPE*>(data)->last_column;
		} else {
			m_lexer->m_lploc->first_line   = reinterpret_cast<YYLTYPE*>(data)->first_line;
			m_lexer->m_lploc->first_column = reinterpret_cast<YYLTYPE*>(data)->first_column;
			m_lexer->m_lploc->last_line    = reinterpret_cast<YYLTYPE*>(data)->last_line;
			m_lexer->m_lploc->last_column  = reinterpret_cast<YYLTYPE*>(data)->last_column;
		}
	}
}

void RDOParserRDO::lexer_loc_delta_pos( int value )
{
	if ( m_lexer && m_lexer->m_lploc ) {
		m_lexer->m_lploc->first_column += value;
		m_lexer->m_lploc->last_column  += value;
	}
}

void RDOParserRDO::lexer_loc_backup( void* data )
{
	if ( data ) {
		m_loc.first_line   = reinterpret_cast<YYLTYPE*>(data)->first_line;
		m_loc.first_column = reinterpret_cast<YYLTYPE*>(data)->first_column;
		m_loc.last_line    = reinterpret_cast<YYLTYPE*>(data)->last_line;
		m_loc.last_column  = reinterpret_cast<YYLTYPE*>(data)->last_column;
	} else if ( m_lexer && m_lexer->m_lploc ) {
		m_loc = *m_lexer->m_lploc;
	}
}

void RDOParserRDO::lexer_loc_restore()
{
	if ( m_lexer && m_lexer->m_lploc ) {
		*m_lexer->m_lploc = m_loc;
	}
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
// ---------- RDOParserRTP
// ----------------------------------------------------------------------------
RDOLexer* RDOParserRTP::getLexer( std::istream& in_stream, std::ostream& out_stream )
{
	return new RDOLexerRTP( this, &in_stream, &out_stream );
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
void RDOParserRSSPost::parse()
{
	std::vector< RDORSSResource* >::const_iterator it = currParser->getRSSResources().begin();
	while ( it != currParser->getRSSResources().end() ) {
		RDOCalcCreateNumberedResource* createResource = new RDOCalcCreateNumberedResource( (*it)->getType()->getNumber(), (*it)->getTrace(), (*it)->getValues(), (*it)->getNumber(), (*it)->getType()->isPermanent() );
		currParser->runTime->addInitCalc( createResource );
		it++;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
void RDOParserSTDFUN::parse()
{
	if ( rdoParse::currParser ) rdoParse::currParser->LoadStdFunctions();
}

// ----------------------------------------------------------------------------
// ---------- RDOParserSMR1
// ----------------------------------------------------------------------------
void RDOParserSMR1::parse( std::istream& in_stream )
{
	try {
		RDOParserRDO::parse( in_stream );
	} catch ( RDOSMR1OkException& ) {
		// Everithing ok, just end of first part parsing
	}
	if ( rdoParse::currParser && rdoParse::currParser->smr && !rdoParse::currParser->smr->showRate ) {
		rdoParse::currParser->smr->showRate = rdoParse::currParser->addDouble(new double(60.0));
	}
}

} // namespace rdoParse
