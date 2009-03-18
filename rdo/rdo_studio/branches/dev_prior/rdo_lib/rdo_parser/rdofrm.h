#ifndef RDOFRM_H
#define RDOFRM_H

#include "rdoparser_object.h"
#include <rdoframe.h>

namespace rdoParse
{

int frmparse( void* lexer );
int frmlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void frmerror( char* mes );

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
class RDOFUNLogic;

class RDOFRMFrame: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFRMFrame( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDOFUNLogic* logic = NULL );

	const std::string&       name() const  { return src_info().src_text(); }
	rdoRuntime::RDOFRMFrame* frame()       { return m_frame;               }

private:
	rdoRuntime::RDOFRMFrame* m_frame;
};

} // namespace rdoParse

#endif // RDOFRM_H
