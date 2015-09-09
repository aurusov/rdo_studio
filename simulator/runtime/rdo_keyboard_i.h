#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime)

CLOSE_RDO_RUNTIME_NAMESPACE

class IKeyboard: public virtual rdo::counter_reference
{
public:
    enum class AddHotKeyResult
    {
        OK,
        ALREADY,
        NOTFOUND,
        DONT
    };
    virtual bool hasHotKey() const = 0;
    virtual AddHotKeyResult addHotKey(const rdo::runtime::LPRDORuntime& pRuntime, const std::string& hotKey) = 0;
};
DECLARE_POINTER(IKeyboard)

#define DECLARE_IKeyboard \
    virtual bool hasHotKey() const; \
    virtual AddHotKeyResult addHotKey(const rdo::runtime::LPRDORuntime& pRuntime, const std::string& hotKey);
