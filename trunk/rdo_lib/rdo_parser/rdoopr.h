#ifndef RDOOPR_H
#define RDOOPR_H

#include "rdodpt.h"

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
public:
	RDOOPROperation( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info );
};

} // namespace rdoParse 

#endif // RDOOPR_H
