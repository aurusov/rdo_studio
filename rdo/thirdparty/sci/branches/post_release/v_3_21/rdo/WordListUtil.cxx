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
#include <boost\foreach.hpp>

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
	struct Keyword
	{
		tstring value;
		float   priority;

		Keyword()
			: priority(0.0)
		{}
		Keyword(const tstring& value, float priority)
			: value   (value   )
			, priority(priority)
		{}

		rbool operator< (const Keyword& keyword) const
		{
			return priority < keyword.priority;
		}
	};

	int start = 0;
	int end = wl.len - 1;
	std::vector<tstring> res;
	std::vector<Keyword> pres;
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
			pres.push_back(Keyword(wl.words[i], tstring(wl.words[i]).length()));
		}
	}

	std::sort(pres.begin(), pres.end());

	BOOST_FOREACH(const Keyword& keyword, pres)
	{
		res.push_back(keyword.value);
	}
	return res;
}
