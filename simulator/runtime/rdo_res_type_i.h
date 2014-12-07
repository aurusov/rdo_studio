#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime );
PREDECLARE_POINTER(RDOResource);

PREDECLARE_OBJECT_INTERFACE(IResourceType)
struct IResourceType: public rdo::RefCounter<IResourceType>
{
DECLARE_FACTORY(IResourceType);
public:
    virtual rdo::runtime::LPRDOResource createRes(const LPRDORuntime& pRuntime, std::size_t resID, const std::vector<RDOValue>& paramsCalcs, bool traceFlag, bool permanentFlag, bool isNested = 0) = 0;

    typedef  std::list<rdo::runtime::LPRDOResource>  ResList;
    typedef  ResList::const_iterator  ResCIterator;

    virtual ResCIterator res_begin() const = 0;
    virtual ResCIterator res_end() const = 0;

    virtual void eraseRes(const rdo::runtime::LPRDOResource& pResource) = 0;

    typedef  RDOResource  value_type;

protected:
    IResourceType()          {}
    virtual ~IResourceType() {}

};

#define DECLARE_IResourceType \
    rdo::runtime::LPRDOResource createRes(const LPRDORuntime& pRuntime, std::size_t resID, const std::vector<RDOValue>& paramsCalcs, bool traceFlag, bool permanentFlag, bool isNested = 0);

CLOSE_RDO_RUNTIME_NAMESPACE
