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
#include <rdocalc.h>

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
	parser()->insertRSSResource( this );
	currParam = resType->getParams().begin();
}

int RDORSSResource::writeModelStructure()
{
	parser()->modelStructure << (number + 1) << " " << getName() << " " << getType()->getNumber() << std::endl;
	return 0;
}

rdoRuntime::RDOCalc* RDORSSResource::createCalc()
{
	rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcCreateNumberedResource( parser()->runtime(), getType()->getNumber(), getTrace(), getValues(), getNumber(), getType()->isPermanent() );
	calc->setSrcInfo( src_info() );
	calc->setSrcText( "Создание ресурса " + calc->src_text() );
	return calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPResType* const _resType ):
	RDORSSResource( _parser, _src_info, _resType )
{
}

rdoRuntime::RDOCalc* RDOPROCResource::createCalc()
{
	rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcCreateProcessResource( parser()->runtime(), getType()->getNumber(), getTrace(), getValues(), getNumber(), getType()->isPermanent() );
	calc->setSrcInfo( src_info() );
	calc->setSrcText( "Создание ресурса " + calc->src_text() );
	return calc;
}

} // namespace rdoParse 
