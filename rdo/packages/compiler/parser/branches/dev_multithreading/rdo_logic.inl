/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/rdo_logic.inl
  \author    Клеванец Игорь (rdo@rk9.bmstu.ru)
  \date      31.01.2012
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

template<class RTLogic, class Activity>
inline CREF(tstring) RDOLogic<RTLogic,Activity>::name() const
{
	return src_info().src_text();
}

template<class RTLogic, class Activity>
inline typename RDOLogic<RTLogic, Activity>::LPActivity RDOLogic<RTLogic,Activity>::addNewActivity (CREF(RDOParserSrcInfo) activity_src_info, CREF(RDOParserSrcInfo) pattern_src_info)
{
	LPActivity pAactivity = rdo::Factory<Activity>::create(m_pRuntimeLogic, activity_src_info, pattern_src_info);
	ASSERT(pAactivity);
	m_activityList.push_back(pAactivity);
	return pAactivity;
}

template<class RTLogic, class Activity>
inline typename RDOLogic<RTLogic, Activity>::LPActivity RDOLogic<RTLogic,Activity>::getLastActivity() const
{
	return !m_activityList.empty() ? m_activityList.back() : LPActivity(NULL);
}

template<class RTLogic, class Activity>
inline CREF(typename RDOLogic<RTLogic, Activity>::ActivityList) RDOLogic<RTLogic,Activity>::getActivities() const
{
	return m_activityList;
}

template<class RTLogic, class Activity>
inline rbool RDOLogic<RTLogic,Activity>::setPrior(REF(LPRDOFUNArithm) pPrior)
{
	LPIPriority pPriority = m_pRuntimeLogic;
	if (pPriority)
	{
		return pPriority->setPrior(pPrior->createCalc());
	}
	return false;
}

template<class RTLogic, class Activity>
inline rbool RDOLogic<RTLogic,Activity>::getMultithreading () const
{
	return m_multithreading;
}

template<class RTLogic, class Activity>
inline void RDOLogic<RTLogic,Activity>::setMultithreading (rbool multithreading)
{
	m_multithreading = multithreading;
}

CLOSE_RDO_PARSER_NAMESPACE
