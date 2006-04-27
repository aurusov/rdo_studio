#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser_base.h"
#include "rdoparser_lexer.h"
#include "rdoparselex.h"

#include <rdobinarystream.h>
#include <rdokernel.h>
#include <rdorepository.h>

rdoParserBase::~rdoParserBase()
{
	if ( m_lexer ) {
		delete m_lexer;
		m_lexer = NULL;
	}
}

void rdoParserBase::parse()
{
	if ( m_lexer ) delete m_lexer;
	rdo::binarystream in_stream;
	kernel.getRepository()->load( type, in_stream );
	if ( in_stream.good() ) {
		std::ostringstream out_stream;
		m_lexer = new rdoFlexLexer( &in_stream, &out_stream );
		parser_fun( m_lexer );
	}
}
