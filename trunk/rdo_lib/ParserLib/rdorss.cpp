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
	rdoParse::currParser->error( mes );
}

RDORSSResource::RDORSSResource( const std::string* const _name, const RDORTPResType* const _resType ):
	name( _name ),
	resType( _resType ),
	number( currParser->getRSS_id() ),
	trace( false )
{
	currParser->insertRSSResource( this );
	currParam = resType->getParams().begin();
}

int RDORSSResource::writeModelStructure()
{
	currParser->modelStructure << (number + 1) << " " << *getName() << " " << getType()->getNumber() << std::endl;
	return 0;
}

RDORSSTransact::RDORSSTransact( const std::string* const _name, const RDORTPResType* const _resType ):
	RDORSSResource( _name, _resType )
{
}

} // namespace rdoParse 
