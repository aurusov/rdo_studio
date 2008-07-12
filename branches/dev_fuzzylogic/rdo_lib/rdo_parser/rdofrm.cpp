#include "pch.h"
#include "rdofrm.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdoparser_lexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int frmlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void frmerror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDOFUNLogic* logic ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	m_frame( NULL )
{
	m_frame = new rdoRuntime::RDOFRMFrame( parser()->runtime(), _src_info, logic ? logic->getCalc() : NULL );
	parser()->insertFRMFrame( this );
}

} // namespace rdoParse
