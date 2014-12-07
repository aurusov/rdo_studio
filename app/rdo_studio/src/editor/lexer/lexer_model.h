#pragma once

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
