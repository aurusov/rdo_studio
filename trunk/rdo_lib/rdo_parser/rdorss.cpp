#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdorss.h"
#include "rdortp.h"
#include "rdoparser.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

int rsslex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void rsserror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDORSSResource
// ----------------------------------------------------------------------------
RDORSSResource::RDORSSResource( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPResType* const _resType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	resType( _resType ),
	number( _parser->getRSS_id() ),
	trace( false )
{
	getParser()->insertRSSResource( this );
	currParam = resType->getParams().begin();
}

int RDORSSResource::writeModelStructure()
{
	getParser()->modelStructure << (number + 1) << " " << getName() << " " << getType()->getNumber() << std::endl;
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDORSSTransact
// ----------------------------------------------------------------------------
RDORSSTransact::RDORSSTransact( RDOParser* _parser, const std::string& _name, const RDORTPResType* const _resType ):
	RDORSSResource( _parser, _name, _resType )
{
}

} // namespace rdoParse 
