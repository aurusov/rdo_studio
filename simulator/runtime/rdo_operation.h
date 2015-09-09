#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_event_i.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_priority.h"
#include "simulator/runtime/rdo_operation_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOOperation
    : public IBaseOperation
    , public IOperation
    , public IEvent
    , public RDOActivityPattern<RDOPatternOperation>
    , public RDOPatternPrior
    , public IOperationTrace
{
typedef RDOActivityPattern<RDOPatternOperation> pattern_type;
DECLARE_FACTORY(RDOOperation);
friend class RDOTrace;
friend class RDOSimulatorTrace;
friend class RDOSimulator;

protected:
    RDOOperation(const LPRDORuntime& pRuntime, const LPRDOPatternOperation& pPattern, bool trace, const std::string& name);
    RDOOperation(const LPRDORuntime& pRuntime, const LPRDOPatternOperation& pPattern, bool trace, const LPRDOCalc& pCondition, const std::string& name);
    virtual ~RDOOperation();
    DECLARE_IOperation;

private:
    RDOOperation(const LPRDORuntime& pRuntime, const RDOOperation& originForClone);

    typedef std::vector<LPIOperation> CloneList;

    CloneList m_cloneList;
    bool m_haveAdditionalCondition;
    LPRDOCalc m_pAdditionalCondition;
    int m_operId;

    virtual void onMakePlaned(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& params);

    void incrementRelevantResourceReference(const LPRDORuntime& pRuntime);
    void decrementRelevantResourceReference(const LPRDORuntime& pRuntime);

    DECLARE_IBaseOperation;
    DECLARE_IOperationTrace;
};

CLOSE_RDO_RUNTIME_NAMESPACE
