#ifndef RDOFRM_H
#define RDOFRM_H

#include "rdortp.h"

namespace rdoParse
{

int frmparse( void* lexer );
int frmlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void frmerror( char* mes );

} // namespace rdoParse

#endif // RDOFRM_H
