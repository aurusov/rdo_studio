#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdofun.h"
#include "simulator/runtime/rdo_logic_i.h"
#include "simulator/runtime/rdo_priority_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogicActivity
// --------------------------------------------------------------------------------
template<class RTLogic, class Activity>
class RDOLogicActivity: public RDOParserSrcInfo
{
public:
    typedef  rdo::intrusive_ptr<Activity> LPActivity;
    typedef  std::vector<LPActivity>      ActivityList;

    RDOLogicActivity(const RDOParserSrcInfo& src_info)
        : RDOParserSrcInfo(src_info)
    {}
    virtual ~RDOLogicActivity()
    {}

    const std::string& name() const { return src_info().src_text(); }

    LPActivity addNewActivity(const RDOParserSrcInfo& activity_src_info, const RDOParserSrcInfo& pattern_src_info)
    {
        LPActivity pAactivity = rdo::Factory<Activity>::create(m_pRuntimeLogic.object_dynamic_cast<IBaseOperationContainer>(), activity_src_info, pattern_src_info);
        ASSERT(pAactivity);
        m_activityList.push_back(pAactivity);
        return pAactivity;
    }

    LPActivity getLastActivity() const
    {
        return !m_activityList.empty() ? m_activityList.back() : LPActivity(NULL);
    }
    const ActivityList& getActivities() const { return m_activityList; }

    bool setPrior(LPRDOFUNArithm& pPrior)
    {
        LPIPriority pPriority = m_pRuntimeLogic.object_dynamic_cast<IPriority>();
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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
