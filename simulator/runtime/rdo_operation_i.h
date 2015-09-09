#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

class IOperation: public virtual rdo::counter_reference
{
public:
    virtual void   onBeforeChoiceFrom    (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual bool   choiceFrom            (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual void   onBeforeOperationBegin(const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual void   convertBegin          (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual void   onAfterOperationBegin (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual void   onBeforeOperationEnd  (const rdo::runtime::LPRDORuntime& pRuntime, const std::vector<rdo::runtime::RDOValue>& params) = 0;
    virtual void   convertEnd            (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual void   onAfterOperationEnd   (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
    virtual double getNextTimeInterval   (const rdo::runtime::LPRDORuntime& pRuntime) = 0;
};
DECLARE_POINTER(IOperation)

#define DECLARE_IOperation \
    virtual void   onBeforeChoiceFrom    (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual bool   choiceFrom            (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual void   onBeforeOperationBegin(const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual void   convertBegin          (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual void   onAfterOperationBegin (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual void   onBeforeOperationEnd  (const rdo::runtime::LPRDORuntime& pRuntime, const std::vector<rdo::runtime::RDOValue>& params); \
    virtual void   convertEnd            (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual void   onAfterOperationEnd   (const rdo::runtime::LPRDORuntime& pRuntime); \
    virtual double getNextTimeInterval   (const rdo::runtime::LPRDORuntime& pRuntime)

class IOperationTrace: public virtual rdo::counter_reference
{
public:
    virtual std::string traceOperId() const = 0;
};
DECLARE_POINTER(IOperationTrace)

#define DECLARE_IOperationTrace \
    virtual std::string traceOperId() const;
