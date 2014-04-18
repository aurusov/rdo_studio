/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/rdo_logic.inl
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Клеванец Игорь (impus@hotbox.ru)
  \date      31.01.2012
  \brief     Логика парсера
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

template<class RTLogic, class Activity>
inline RDOLogic<RTLogic, Activity>::RDOLogic(const RDOParserSrcInfo& src_info)
	: RDOLogicBase(src_info)
{}

template<class RTLogic, class Activity>
inline RDOLogic<RTLogic, Activity>::~RDOLogic()
{}

template<class RTLogic, class Activity>
inline typename RDOLogic<RTLogic, Activity>::LPActivity RDOLogic<RTLogic,Activity>::addNewActivity(const RDOParserSrcInfo& activity_src_info, const RDOParserSrcInfo& pattern_src_info)
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
inline const typename RDOLogic<RTLogic, Activity>::ActivityList& RDOLogic<RTLogic,Activity>::getActivities() const
{
	return m_activityList;
}

CLOSE_RDO_PARSER_NAMESPACE
