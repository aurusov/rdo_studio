#ifndef LEXRDO_H
#define LEXRDO_H

#include <ctype.h>
#include "utils/rdotypes.h"
#include "utils/rdolocale.h"
#include "thirdparty/scintilla/include/ILexer.h"
#include "thirdparty/scintilla/lexlib/LexerModule.h"

namespace rdo { namespace gui { namespace lexer {

bool isOperator  (char ch);
bool isIdentifier(char ch);

}}} // namespace rdo::gui::lexer

extern LexerModule lexerRDOSyntax;

#endif // LEXRDO_H
