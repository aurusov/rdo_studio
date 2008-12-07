// Scintilla source code edit control
/** @file KeyWords.cxx
 ** Colourise for particular languages.
 **/
// Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
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

const LexerModule *LexerModule::base = 0;
int LexerModule::nextLanguage = SCLEX_AUTOMATIC+1;

LexerModule::LexerModule(int language_, LexerFunction fnLexer_,
	const char *languageName_, LexerFunction fnFolder_) :
	language(language_), 
	fnLexer(fnLexer_), 
	fnFolder(fnFolder_), 
	languageName(languageName_) {
	next = base;
	base = this;
	if (language == SCLEX_AUTOMATIC) {
		language = nextLanguage;
		nextLanguage++;
	}
}

const LexerModule *LexerModule::Find(int language) {
	const LexerModule *lm = base;
	while (lm) {
		if (lm->language == language) {
			return lm;
		}
		lm = lm->next;
	}
	return 0;
}

const LexerModule *LexerModule::Find(const char *languageName) {
	if (languageName) {
		const LexerModule *lm = base;
		while (lm) {
			if (lm->languageName && 0 == strcmp(lm->languageName, languageName)) {
				return lm;
			}
			lm = lm->next;
		}
	}
	return 0;
}

void LexerModule::Lex(unsigned int startPos, int lengthDoc, int initStyle,
	  WordList *keywordlists[], Accessor &styler) const {
	if (fnLexer)
		fnLexer(startPos, lengthDoc, initStyle, keywordlists, styler);
}

void LexerModule::Fold(unsigned int startPos, int lengthDoc, int initStyle,
	  WordList *keywordlists[], Accessor &styler) const {
	if (fnFolder) {
		int lineCurrent = styler.GetLine(startPos);
		// Move back one line in case deletion wrecked current line fold state
		if (lineCurrent > 0) {
			lineCurrent--;
			int newStartPos = styler.LineStart(lineCurrent);
			lengthDoc += startPos - newStartPos;
			startPos = newStartPos;
			initStyle = 0;
			if (startPos > 0) {
				initStyle = styler.StyleAt(startPos - 1);
			}
		}
		fnFolder(startPos, lengthDoc, initStyle, keywordlists, styler);
	}
}

// The following code forces a reference to all of the Scintilla lexers.
// If we don't do something like this, then the linker tends to "optimize"
// them away. (eric@sourcegear.com)

// Taken from wxWindow's stc.cpp. Walter.

int wxForceScintillaLexers(void) {
	return Scintilla_LinkLexers();
}

int Scintilla_LinkLexers() {
  extern LexerModule lmRDO;

  if ( &lmRDO )
    {
      return 1;
    }
  else
    {
      return 0;
    }
}
