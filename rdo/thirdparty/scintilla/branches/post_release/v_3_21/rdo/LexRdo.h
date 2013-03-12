#ifndef LEXRDO_H
#define LEXRDO_H

#include <locale>
#include "utils/rdotypes.h"
#include "thirdparty/scintilla/include/ILexer.h"
#include "thirdparty/scintilla/lexlib/LexerModule.h"

namespace rdo { namespace gui { namespace lexer {

bool isOperator  (char ch);
bool isIdentifier(char ch, const std::locale& locale);

}}} // namespace rdo::gui::lexer

extern LexerModule lexerRDOSyntax;

#endif // LEXRDO_H
