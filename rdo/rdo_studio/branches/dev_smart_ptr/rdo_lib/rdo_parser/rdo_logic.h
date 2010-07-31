/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_LOGIC_H_
#define _RDOPARSER_LOGIC_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_runtime/rdo_logic_interface.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLogicActivity
// ----------------------------------------------------------------------------
template<class RTLogic, class Activity>
class RDOLogicActivity: public RDOParserObject, public RDOParserSrcInfo
{
public:
	typedef std::vector<PTR(Activity)> ActivityList;

	RDOLogicActivity(PTR(RDOParser) parser, CREF(RDOParserSrcInfo) src_info)
		: RDOParserObject (parser  )
		, RDOParserSrcInfo(src_info)
	{}

	CREF(tstring) name() const { return src_info().src_text(); }

	PTR(Activity) addNewActivity(CREF(RDOParserSrcInfo) activity_src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	{
		PTR(Activity) activity = new Activity(this->m_rt_logic, this, activity_src_info, pattern_src_info);
		m_activities.push_back(activity);
		return activity;
	}

	PTR(Activity) getLastActivity() const
	{
		return !m_activities.empty() ? m_activities.back() : NULL;
	}
	CREF(ActivityList) getActivities() const { return m_activities; }

	rbool setPrior(REF(LPRDOFUNArithm) prior)
	{
		LPIPriority priority = m_rt_logic;
		if (priority)
		{
			return priority->setPrior(prior->createCalc());
		}
		return false;
	}

protected:
	LPILogic     m_rt_logic;

private:
	ActivityList m_activities;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_LOGIC_H_
