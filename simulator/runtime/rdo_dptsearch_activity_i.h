#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_rule_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOSimulator;

CLOSE_RDO_RUNTIME_NAMESPACE

class IDPTSearchActivity: public virtual rdo::counter_reference
{
public:
    enum class CostTime
    {
        BEFORE,
        AFTER
    };

    virtual LPIRule& rule() = 0;
    virtual double cost(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual CostTime costTime() const = 0;
};
DECLARE_POINTER(IDPTSearchActivity)

#define DECLARE_IDPTSearchActivity                                   \
    virtual LPIRule& rule();                                         \
    virtual double cost(const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual CostTime costTime() const;
