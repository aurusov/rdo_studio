/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      WordListUtil.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      29.09.2012
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "thirdparty/sci/rdo/WordListUtil.h"
// --------------------------------------------------------------------------------

WordListUtil::WordListUtil(const WordList& wordlist)
	: wl(wordlist)
{}

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
		boost::iterator_range<char*> findPatternIt = boost::ifind_first(wl.words[i], userPattern);
		if (!findPatternIt.empty())
		{
			boost::iterator_range<char*> fullKeywordIt(wl.words[i], wl.words[i] + strlen(wl.words[i]));
			ruint position = findPatternIt.begin() - fullKeywordIt.begin();
			float priority = tstring(wl.words[i]).length();
			priorityResult.push_back(PriorityResultItem(wl.words[i], priority));
		}
	}
	std::sort(priorityResult.begin(), priorityResult.end());

	BOOST_FOREACH(const PriorityResultItem& item, priorityResult)
	{
		result.push_back(item.value);
	}

	return result;
}
