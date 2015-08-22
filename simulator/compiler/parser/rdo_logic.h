#pragma once

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

#include "simulator/compiler/parser/rdo_logic-inl.h"
