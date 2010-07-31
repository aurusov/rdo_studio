#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"

namespace rdoParse 
{

int frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void frmerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame( RDOParser* _parser, const RDOParserSrcInfo& _src_info, LPRDOFUNLogic logic ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	m_frame( NULL )
{
	m_frame = new rdoRuntime::RDOFRMFrame( parser()->runtime(), _src_info, logic ? logic->getCalc() : NULL );
	parser()->insertFRMFrame( this );
}

} // namespace rdoParse
