/*
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic.h
  \author    Барс Александр, Урусов Андрей
  \date      
  \brief     
  \indent    4T
 */

#ifndef _CONVERTOR_RDOCONVERTER_LOGIC_H_
#define _CONVERTOR_RDOCONVERTER_LOGIC_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_object.h"
#include "rdo_lib/rdo_converter/rdofun.h"
#include "rdo_lib/rdo_runtime/rdo_logic_i.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLogicActivity
// ----------------------------------------------------------------------------
template<class RTLogic, class Activity>
class RDOLogicActivity: public RDOParserSrcInfo
{
public:
	typedef  rdo::intrusive_ptr<Activity> LPActivity;
	typedef  std::vector<LPActivity>      ActivityList;

	RDOLogicActivity(CREF(RDOParserSrcInfo) src_info)
		: RDOParserSrcInfo(src_info)
	{}
	virtual ~RDOLogicActivity()
	{}

	CREF(tstring) name() const { return src_info().src_text(); }

	LPActivity addNewActivity(CREF(RDOParserSrcInfo) activity_src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	{
		LPActivity pAactivity = rdo::Factory<Activity>::create(m_pRuntimeLogic, activity_src_info, pattern_src_info);
		ASSERT(pAactivity);
		m_activityList.push_back(pAactivity);
		return pAactivity;
	}

	LPActivity getLastActivity() const
	{
		return !m_activityList.empty() ? m_activityList.back() : NULL;
	}
	CREF(ActivityList) getActivities() const { return m_activityList; }

	rbool setPrior(REF(LPRDOFUNArithm) pPrior)
	{
		LPIPriority pPriority = m_pRuntimeLogic;
		if (pPriority)
		{
			return pPriority->setPrior(pPrior->createCalc());
		}
		return false;
	}

protected:
	LPILogic     m_pRuntimeLogic;

private:
	ActivityList m_activityList;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDOCONVERTER_LOGIC_H_
