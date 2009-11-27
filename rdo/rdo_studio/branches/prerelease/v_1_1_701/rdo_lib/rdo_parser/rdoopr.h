#ifndef RDOOPR_H
#define RDOOPR_H

#include "rdodpt.h"

namespace rdoRuntime
{
class RDOOperations;
}

namespace rdoParse 
{

int oprparse( void* lexer );
int oprlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void oprerror( char* mes );

// ----------------------------------------------------------------------------
// ---------- RDOOPROperation
// ----------------------------------------------------------------------------
class RDOOPROperation: public RDODPTActivityHotKey
{
friend class RDOLogicActivity<rdoRuntime::RDOOperations, RDOOPROperation>;
private:
	RDOOPROperation( const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
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
