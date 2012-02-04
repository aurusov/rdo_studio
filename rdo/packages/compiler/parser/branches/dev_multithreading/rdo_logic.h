/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/rdo_logic.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_LOGIC_H_
#define _RDOPARSER_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/runtime/rdo_logic_i.h"
#include "simulator/runtime/rdo_priority_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogic
// --------------------------------------------------------------------------------
template<class RTLogic, class Activity>
class RDOLogic: public RDOParserSrcInfo
{
public:
	typedef  rdo::intrusive_ptr<Activity> LPActivity;
	typedef  std::vector<LPActivity>      ActivityList;

	RDOLogic(CREF(RDOParserSrcInfo) src_info)
		: RDOParserSrcInfo(src_info)
	{}
	virtual ~RDOLogic()
	{}

	CREF(tstring)      name              () const;

	LPActivity         addNewActivity    (CREF(RDOParserSrcInfo) activity_src_info, CREF(RDOParserSrcInfo) pattern_src_info);
	LPActivity         getLastActivity   () const;
	CREF(ActivityList) getActivities     () const;

	rbool              setPrior          (REF(LPRDOFUNArithm) pPrior);
	rbool              getMultithreading () const;
	void               setMultithreading (rbool multithreading);

protected:
	LPILogic     m_pRuntimeLogic;
	rbool        m_multithreading;

private:
	ActivityList m_activityList;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/rdo_logic.inl"

#endif // _RDOPARSER_LOGIC_H_
