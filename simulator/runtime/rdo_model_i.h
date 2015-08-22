#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

class IModelStructure: public virtual rdo::counter_reference
{
public:
        virtual void writeModelStructure(std::ostream& stream) const = 0;
};
DECLARE_POINTER(IModelStructure)

#define DECLARE_IModelStructure \
    virtual void writeModelStructure(std::ostream& stream) const;

class IName: public virtual rdo::counter_reference
{
public:
        virtual const std::string& name() const = 0;
};
DECLARE_POINTER(IName)

#define DECLARE_IName \
    virtual const std::string& name() const;
