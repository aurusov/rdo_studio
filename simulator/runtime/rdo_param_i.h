#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDOCalc)

CLOSE_RDO_RUNTIME_NAMESPACE

class IParam
{
public:
    virtual bool setParam(const rdo::runtime::LPRDOCalc& pParam) = 0;
    virtual bool endParam() = 0;
};

#define DECLARE_IParam \
    virtual bool setParam(const rdo::runtime::LPRDOCalc& pParam); \
    virtual bool endParam();
