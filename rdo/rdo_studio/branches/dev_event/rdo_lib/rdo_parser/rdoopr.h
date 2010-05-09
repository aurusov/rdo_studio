#ifndef RDOOPR_H
#define RDOOPR_H

#include "rdo_lib/rdo_parser/rdodpt.h"

namespace rdoRuntime
{
class RDOOperations;
}

namespace rdoParse 
{

int  oprparse(PTR(void) lexer);
int  oprlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void oprerror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDOOPROperation
// ----------------------------------------------------------------------------
class RDOOPROperation: public RDODPTActivityHotKey
{
friend class RDOLogicActivity<rdoRuntime::RDOOperations, RDOOPROperation>;
private:
	RDOOPROperation( LPIBaseOperationContainer dpt, const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDOOperations
// ----------------------------------------------------------------------------
class RDOOperations: public RDOLogicActivity<rdoRuntime::RDOOperations, RDOOPROperation>
{
public:
	RDOOperations( RDOParser* _parser, const RDOParserSrcInfo& _src_info );
};

} // namespace rdoParse 

#endif // RDOOPR_H
