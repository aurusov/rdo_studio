// Scintilla source code edit control
/** @file LexFind.cxx
 ** Lexer for find results.
 ** Written by Andrey Urusov.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "LexFind.h"
#include "LexRdo.h"

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"
#include "SString.h"

static void ColouriseFindDoc( unsigned int startPos, int length, int initStyle, WordList *keywordlists[], Accessor &styler )
{
	WordList& keywords  = *keywordlists[0];

	styler.StartAt( startPos );

	StyleContext sc( startPos, length, initStyle, styler );

	bool flag = sc.More();
	for ( ; flag; sc.Forward() ) {

		if ( sc.state == SCE_FIND_IDENTIFIER ) {
			if ( !isRDOLexerIdentifier(sc.ch) ) {
				char s[100];
				sc.GetCurrent( s, sizeof(s) );
				if ( keywords.InList(s) ) {
					sc.ChangeState( SCE_FIND_KEYWORD );
				}
				sc.SetState( SCE_FIND_DEFAULT );
			} else {
				char s[100];
				sc.GetCurrent( s, sizeof(s) );
			}
		} else {
			char s[100];
			sc.GetCurrent( s, sizeof(s) );
		}

		if ( sc.state == SCE_FIND_DEFAULT ) {
			if ( isRDOLexerIdentifier(sc.ch) )
				sc.SetState( SCE_FIND_IDENTIFIER );
		}

		flag = sc.More();
	}
	sc.Complete();
}

LexerModule lmFind( SCLEX_FIND, ColouriseFindDoc, "find" );
