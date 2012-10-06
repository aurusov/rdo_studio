/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      WordListUtil.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      29.09.2012
  \brief     
  \indent    4T
*/

#include <boost/noncopyable.hpp>
#include "thirdparty/sci/lexlib/WordList.h"

class WordListUtil: public boost::noncopyable
{
public:
	WordListUtil(const WordList& wordlist);
	//tstring GetNearestWord(const char *wordStart, int searchLen) const;
	std::vector<tstring> GetNearestWords(const tstring& userPattern) const;

private:
	const WordList& wl;
};
