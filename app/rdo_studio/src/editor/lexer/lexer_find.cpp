/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      lexer_find.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <boost/locale.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/lexer/lexer_find.h"
#include "app/rdo_studio/src/editor/lexer/lexer_model.h"
#include "thirdparty/scintilla/include/Platform.h"
#include "thirdparty/scintilla/include/Scintilla.h"
#include "thirdparty/scintilla/include/SciLexer.h"
#include "thirdparty/scintilla/lexlib/WordList.h"
#include "thirdparty/scintilla/lexlib/LexAccessor.h"
#include "thirdparty/scintilla/lexlib/Accessor.h"
// --------------------------------------------------------------------------------

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
			if ( (matchCase && ch == findKeyword[0]) || (!matchCase && boost::locale::to_lower(std::string(1, ch)) == boost::locale::to_lower(std::string(1, findKeyword[0]))) ) {
				bool flag = true;
				for ( int j = 0; j < findKeywordLen; j++ ) {
					char c1 = styler.SafeGetCharAt( i + j );
					char c2 = findKeyword[j];
					if ( (matchCase && c1 != c2) || (!matchCase && boost::locale::to_lower(std::string(1, c1)) != boost::locale::to_lower(std::string(1, c2))) ) {
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
