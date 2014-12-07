#ifndef _RDO_STUDIO_EDITOR_LEXER_WORD_LIST_UTIL_H_
#define _RDO_STUDIO_EDITOR_LEXER_WORD_LIST_UTIL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <string>
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "thirdparty/scintilla/lexlib/WordList.h"
// --------------------------------------------------------------------------------

class WordListUtil: public boost::noncopyable
{
public:
	WordListUtil(const WordList& wordlist);
	std::vector<std::string> getNearestWords(const std::string& userPattern) const;

private:
	const WordList& wl;
};

#endif // _RDO_STUDIO_EDITOR_LEXER_WORD_LIST_UTIL_H_
