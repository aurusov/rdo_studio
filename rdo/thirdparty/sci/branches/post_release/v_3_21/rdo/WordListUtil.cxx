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

#include "WordListUtil.h"

WordListUtil::WordListUtil(const WordList& wordlist)
	: wl(wordlist)
{}

//tstring WordListUtil::GetNearestWord (const char *wordStart, int searchLen) const
//{
//}

std::vector<tstring> WordListUtil::GetNearestWords(const char *wordStart) const
{
	struct keyword
	{
		tstring value;
		int     priority;

		keyword()
			: priority(0)
		{}
		keyword(const tstring& value, int priority)
			: value   (value   )
			, priority(priority)
		{}

		rbool operator< (const keyword& other) const
		{
			return priority < other.priority;
		}
	};

	int start = 0;
	int end = wl.len - 1;
	std::vector<tstring> res;
	std::vector<keyword> pres;
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
		if (boost::ifind_first(wl.words[i], wordStart))
		{
			pres.push_back(keyword(wl.words[i], tstring(wl.words[i]).length()));
		}
	}

	std::sort(pres.begin(), pres.end());
	std::vector<keyword>::iterator it;

	for (it = pres.begin(); it != pres.end(); ++it)
	{
		res.push_back(it->value);
	}
	return res;
}