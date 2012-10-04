/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      WordListUtil.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      29.09.2012
  \brief     
  \indent    4T
*/

#include <vector>
#include <map>
#include <boost\algorithm\string.hpp>

#include "utils\rdotypes.h"

#include "WordList.h"
#include "WordListUtil.h"

WordListUtil::WordListUtil(const WordList& wordlist)
	:wl(wordlist)
{
}
//tstring WordListUtil::GetNearestWord (const char *wordStart, int searchLen) const
//{
//}

std::vector<tstring> WordListUtil::GetNearestWords(const char *wordStart) const
{
	struct keywords
	{
		tstring keyword;
		int priority;

		bool operator<(const keywords& k)
		{
			return priority < k.priority;
		}
	};
	keywords key;
	int start = 0;
	int end = wl.len - 1;
	std::vector<tstring> res;
	std::vector<keywords> pres;
	if (0 == wl.words)
	{
		res.push_back("");
		return res;
	}
	if( boost::iequals(wordStart, ""))
	{
		for(int i = start; i < end; i++)
		{
			res.push_back(wl.words[i]);
		}
		return res;
	}
	for (int i = start; i < end; i++)
	{
		if(boost::ifind_first(wl.words[i], wordStart))
		{
			key.keyword = wl.words[i];
			key.priority = tstring(wl.words[i]).length();
			pres.push_back(key);
		}
	}
	std::sort(pres.begin(), pres.end());
	std::vector<keywords>::iterator it;
	for(it = pres.begin(); it != pres.end(); ++it)
	{
		res.push_back((*it).keyword);
	}
	return res;
}