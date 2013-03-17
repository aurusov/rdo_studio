/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      LexFind.cxx
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/lexer/LexFind.h"
#include "app/rdo_studio/src/editor/lexer/LexRdo.h"
#include "thirdparty/scintilla/include/Platform.h"
#include "thirdparty/scintilla/include/Scintilla.h"
#include "thirdparty/scintilla/include/SciLexer.h"
#include "thirdparty/scintilla/lexlib/WordList.h"
#include "thirdparty/scintilla/lexlib/LexAccessor.h"
#include "thirdparty/scintilla/lexlib/Accessor.h"
// --------------------------------------------------------------------------------

static inline char RDOMakeUpperCase( char ch )
{
	if ( ch >= 'a' && ch <= 'z' ) return static_cast<char>( ch - 'a' + 'A' );
	if ( ch >= 'а' && ch <= 'я' ) return static_cast<char>( ch - 'а' + 'А' );
	return ch;
}

static inline char RDOMakeLowerCase( char ch )
{
	if ( ch >= 'A' && ch <= 'Z' ) return static_cast<char>( ch - 'A' + 'a' );
	if ( ch >= 'А' && ch <= 'Я' ) return static_cast<char>( ch - 'А' + 'а' );
	return ch;
}

static void lexerRDOFindColor( unsigned int startPos, int length, int initStyle, WordList *keywordlists[], Accessor &styler )
{
	WordList& keywords = *keywordlists[ SCI_RDO_ENDOFLINEONLY_KEYWORDSINDEX ];
	if ( !keywords ) return;
	const char* findKeyword = *keywords.words;
	const int findKeywordLen = strlen( findKeyword );
	bool matchCase = styler.GetPropertyInt( "find_matchcase", 0 ) ? true : false;

	styler.StartAt( startPos );
	styler.StartSegment( startPos );
	int state = initStyle;
	unsigned int lengthDoc = startPos + length;
	for ( unsigned int i = startPos; i < lengthDoc; i++ ) {

		char ch = styler.SafeGetCharAt( i );

		if ( styler.IsLeadByte( ch ) ) {
			i += 1;
			continue;
		}

		if ( state == SCE_FIND_DEFAULT ) {
			if ( (matchCase && ch == findKeyword[0]) || (!matchCase && RDOMakeLowerCase(ch) == RDOMakeLowerCase(findKeyword[0])) ) {
				bool flag = true;
				for ( int j = 0; j < findKeywordLen; j++ ) {
					char c1 = styler.SafeGetCharAt( i + j );
					char c2 = findKeyword[j];
					if ( (matchCase && c1 != c2) || (!matchCase && RDOMakeLowerCase(c1) != RDOMakeLowerCase(c2)) ) {
						flag = false;
						break;
					}
				}
				if ( flag ) {
					int lineCurrent = styler.GetLine( i );
					int posInLine   = i - styler.LineStart( lineCurrent );
					if ( posInLine < 3 ) {
						flag = false;
					}
				}

				styler.ColourTo( i - 1, state );
				if ( flag ) {
					styler.ColourTo( i + findKeywordLen - 1, SCE_FIND_KEYWORD );
					i += findKeywordLen - 1;
				}
				state = SCE_FIND_DEFAULT;
			} else {
				styler.ColourTo( i - 1, state );
				state = SCE_FIND_DEFAULT;
			}
		}

	}
	styler.ColourTo( lengthDoc - 1, state );
}

LexerModule lexerRDOFind(SCLEX_FIND, lexerRDOFindColor, "find");
