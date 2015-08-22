#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <string>
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "thirdparty/scintilla/lexlib/WordList.h"
// --------------------------------------------------------------------------------

class WordListUtil: private boost::noncopyable
{
public:
    WordListUtil(const WordList& wordlist);
    std::vector<std::string> getNearestWords(const std::string& userPattern) const;

private:
    const WordList& wl;
};
