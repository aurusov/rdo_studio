#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

struct IAsString: public virtual rdo::counter_reference
{
DECLARE_FACTORY(IAsString);
public:
    virtual std::string asString() const = 0;

protected:
    IAsString()          {}
    virtual ~IAsString() {}
};
DECLARE_POINTER(IAsString)

#define DECLARE_IAsString \
    std::string asString() const;

CLOSE_RDO_RUNTIME_NAMESPACE
