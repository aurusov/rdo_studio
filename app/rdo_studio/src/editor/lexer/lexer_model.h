#ifndef _RDO_STUDIO_EDITOR_LEXER_MODEL_H_
#define _RDO_STUDIO_EDITOR_LEXER_MODEL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <locale>
// ----------------------------------------------------------------------- SYNOPSIS
#include "thirdparty/scintilla/include/ILexer.h"
#include "thirdparty/scintilla/lexlib/LexerModule.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace lexer {

bool isOperator  (int ch);
bool isIdentifier(int ch);
bool isDigit     (int ch);

}}} // namespace rdo::gui::lexer

extern LexerModule lexerRDOSyntax;

#endif // _RDO_STUDIO_EDITOR_LEXER_MODEL_H_
