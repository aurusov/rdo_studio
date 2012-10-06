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

std::vector<tstring> WordListUtil::GetNearestWords(const tstring& userPattern) const
{
	struct PriorityResultItem
	{
		tstring  value;
		float    priority;

		PriorityResultItem()
			: priority(0.0)
		{}
		PriorityResultItem(const tstring& value, float priority)
			: value   (value   )
			, priority(priority)
		{}

		rbool operator< (const PriorityResultItem& item) const
		{
			return priority < item.priority;
		}
	};

	typedef  std::vector<tstring>  result_type;
	result_type result;

	if (wl.words == 0)
		return result;

	if (userPattern.empty())
	{
		for (int i = 0; i < wl.len; ++i)
		{
			result.push_back(wl.words[i]);
		}
		return result;
	}

	std::vector<PriorityResultItem> priorityResult;
	for (int i = 0; i < wl.len; ++i)
	{
		if (boost::ifind_first(wl.words[i], userPattern))
		{
			priorityResult.push_back(PriorityResultItem(wl.words[i], tstring(wl.words[i]).length()));
		}
	}
	std::sort(priorityResult.begin(), priorityResult.end());

	BOOST_FOREACH(const PriorityResultItem& item, priorityResult)
	{
		result.push_back(item.value);
	}

	return result;
}
