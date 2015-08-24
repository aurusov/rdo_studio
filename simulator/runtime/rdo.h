#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

// TODO Надо бы сделать IBaseOperation базовым для всех возможных логик

class IBaseOperation: public virtual rdo::counter_reference
{
public:
    enum class ResultCode
    {
        CANNOT_RUN,
        PLANNED_AND_RUN,
        MUST_CONTINUE,
        DONE
    };

    virtual void onStart(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual void onStop(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual bool onCheckCondition(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual ResultCode onDoOperation(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual ResultCode onContinue(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
};

#define DECLARE_IBaseOperation                                                     \
    virtual void     onStart         (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual void     onStop          (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual bool     onCheckCondition(const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual ResultCode onDoOperation (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual ResultCode onContinue    (const rdo::runtime::LPRDORuntime& pRuntime);

DECLARE_POINTER(IBaseOperation);

typedef std::vector<LPIBaseOperation> BaseOperationList;
