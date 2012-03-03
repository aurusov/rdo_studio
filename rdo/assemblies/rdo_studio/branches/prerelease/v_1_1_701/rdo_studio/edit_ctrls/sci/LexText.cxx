// Scintilla source code edit control
/** @file LexText.cxx
 ** Lexer for text.
 ** Written by Andrey Urusov.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"
#include "SString.h"

static void ColouriseTextDoc( unsigned int startPos, int length, int /*initStyle*/, WordList * /*keywordlists*/ [], Accessor &styler )
{
	styler.StartAt( startPos );
	styler.StartSegment( startPos );
	unsigned int lengthDoc = startPos + length;
	styler.ColourTo( lengthDoc - 1, SCE_TEXT_DEFAULT );
}

LexerModule lmText( SCLEX_TEXT, ColouriseTextDoc, "text" );
