// Scintilla source code edit control
/** @file WordListUtil.h
 ** Util for WordList.
 ** Written by Yaroslav Romanov.
 **/
// Copyright 1998-2012 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.
#include <string.h>
#include <vector>
#include "WordList.h"

class WordListUtil {

public:
	WordListUtil(const WordList wordlist);
	~WordListUtil();
	
	tstring *GetNearestWord (const char *wordStart) const;
	std::vector<tstring> *GetNearestWords(const char *wordStart) const;

private:
	const WordList wl;
}