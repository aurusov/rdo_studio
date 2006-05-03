#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser_rdo.h"
#include "rdoparser_lexer.h"
#include "rdoparselex.h"
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
	m_lexer = new RDOFlexLexer( this, &in_stream, &out_stream );
	parser_fun( m_lexer );
}

void RDOParserRDO::set_lexer_value( int value )
{
	if ( m_lexer ) {
		*m_lexer->m_lpval = value;
	}
}

int RDOParserRDO::lineno()
{
	return m_lexer ? m_lexer->lineno() : -1;
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
void RDOParserRSSPost::parse()
{
	std::vector< RDORSSResource* >::const_iterator it = currParser->allRSSResource.begin();
	while ( it != currParser->allRSSResource.end() ) {
		RDOCalcCreateNumberedResource* createResource = new RDOCalcCreateNumberedResource( (*it)->getType()->getType(), (*it)->getTrace(), (*it)->getValues(), (*it)->getNumber(), (*it)->getType()->isPerm() );
		currParser->runTime->addInitCalc( createResource );
		it++;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOParserFUN
// ----------------------------------------------------------------------------
void RDOParserFUN::parse( std::istream& in_stream )
{
	if ( rdoParse::currParser ) rdoParse::currParser->LoadStdFunctions();
	RDOParserRDO::parse( in_stream );
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
