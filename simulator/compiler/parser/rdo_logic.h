/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/rdo_logic.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Клеванец Игорь (impus@hotbox.ru)
  \date      31.01.2012
  \brief     Логика парсера
  \indent    4T
*/

#ifndef _RDOPARSER_LOGIC_H_
#define _RDOPARSER_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_logic_base.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/runtime/rdo_logic_i.h"
#include "simulator/runtime/rdo_priority_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogic
// --------------------------------------------------------------------------------
template<class RTLogic, class Activity>
class RDOLogic: public RDOLogicBase
{
public:
	typedef rdo::intrusive_ptr<Activity> LPActivity;
	typedef std::vector<LPActivity>      ActivityList;

	LPActivity          addNewActivity (const RDOParserSrcInfo& activity_src_info, const RDOParserSrcInfo& pattern_src_info);
	LPActivity          getLastActivity() const;
	const ActivityList& getActivities  () const;

protected:
	RDOLogic(const RDOParserSrcInfo& src_info);
	virtual ~RDOLogic();

private:
	ActivityList m_activityList;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/rdo_logic.inl"

#endif // _RDOPARSER_LOGIC_H_
