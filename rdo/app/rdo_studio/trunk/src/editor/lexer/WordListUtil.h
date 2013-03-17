/*!
  \copyright (c) RDO-Team, 2012
  \file      WordListUtil.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      29.09.2012
  \brief     
  \indent    4T
*/

#ifndef _THIRDPARTY_SCINTILLA_WORDLISTUTIL_H_
#define _THIRDPARTY_SCINTILLA_WORDLISTUTIL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "thirdparty/scintilla/lexlib/WordList.h"
// --------------------------------------------------------------------------------

class WordListUtil: public boost::noncopyable
{
public:
	WordListUtil(const WordList& wordlist);
	std::vector<tstring> getNearestWords(const tstring& userPattern) const;

private:
	const WordList& wl;
};

#endif //_THIRDPARTY_SCINTILLA_WORDLISTUTIL_H_
