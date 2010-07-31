#ifndef RDOFRM_H
#define RDOFRM_H

#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_runtime/rdoframe.h"

namespace rdoParse
{

int  frmparse(PTR(void) lexer);
int  frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void frmerror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
class RDOFRMFrame: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOFRMFrame( RDOParser* _parser, const RDOParserSrcInfo& _src_info, LPRDOFUNLogic logic = NULL );

	const std::string&       name() const  { return src_info().src_text(); }
	rdoRuntime::RDOFRMFrame* frame()       { return m_frame;               }

private:
	rdoRuntime::RDOFRMFrame* m_frame;
};

} // namespace rdoParse

#endif // RDOFRM_H
