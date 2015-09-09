#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOResourceTypeList;
typedef  rdo::intrusive_ptr<RDOResourceTypeList>  LPRDOResourceTypeList;

class RDOResourceTypeList
    : public RDOType
    , public IResourceType
    , public RDORuntimeObject
    , public RDOTraceableObject
{
private:
    friend class rdo::Factory<RDOResourceTypeList>;

public:
    typedef  boost::function<LPRDOResource (const LPRDORuntime&, const std::vector<RDOValue>&, const LPIResourceType&, std::size_t, std::size_t, bool, bool, bool)>  Create;
    void setFactoryMethod(const Create& create);

    virtual ResCIterator res_begin() const;
    virtual ResCIterator res_end  () const;

    virtual void          eraseRes(const LPRDOResource& pResource);
    LPRDOResourceTypeList clone   (const LPRDORuntime& pRuntime) const;

protected:
    RDOResourceTypeList(std::size_t number, const LPRDORuntime& pRuntime);
    virtual ~RDOResourceTypeList();

private:
    void insertNewResource(const LPRDORuntime& pRuntime, const LPRDOResource& pResource);

    typedef  std::list<LPRDOResource> ResourceList;
    ResourceList m_resourceList;
    Create       m_create;

    virtual LPRDOResource createRes(const LPRDORuntime& pRuntime, std::size_t resID, const std::vector<RDOValue>& paramsCalcs, bool traceFlag, bool permanentFlag, bool isNested = 0);
};

CLOSE_RDO_RUNTIME_NAMESPACE
