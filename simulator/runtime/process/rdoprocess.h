#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
#include "simulator/runtime/rdo_res_type.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOPROCBlock: public IPROCBlock, public IBaseOperation
{
friend class RDOPROCTransact;
friend class RDOPROCProcess;
friend class RDOPROCResource;

protected:
    LPIPROCProcess  m_process;
    TransactList    m_transacts;

    RDOPROCBlock(LPIPROCProcess process);
    virtual ~RDOPROCBlock();

    DECLARE_IPROCBlock;
};

class RDOPROCProcess: public RDOLogicSimple, public IPROCProcess, public RDOPatternPrior
{
DECLARE_FACTORY(RDOPROCProcess)
friend class RDOPROCBlock;

public:
    DECLARE_IPROCProcess;

protected:
    std::string m_name;
    LPIPROCProcess m_parent;
    std::list<LPIPROCProcess> m_child;

private:
    RDOPROCProcess(const std::string& _name, const LPRDORuntime& pRuntime);
    LPIResourceType m_pTransactType;
};

PREDECLARE_POINTER(RDOPROCResource);

class RDOPROCTransact: public RDOResource
{
DECLARE_FACTORY(RDOPROCTransact);
public:
    LPRDOPROCResource getRes();
    void setRes(const LPRDOPROCResource& pResource);
    LPIPROCBlock& getBlock();
    void setBlock(const LPIPROCBlock& block);

    void next();
    virtual LPRDOResource clone(const LPRDORuntime& pRuntime) const;

private:
    RDOPROCTransact(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& paramsCalcs, LPIResourceType pResType, std::size_t resID, std::size_t typeID, bool trace, bool permanentFlag);
    virtual ~RDOPROCTransact();

    LPIPROCBlock       m_block;
    LPRDOPROCResource  m_res;
};

class RDOPROCResource: public RDOResource
{
DECLARE_FACTORY(RDOPROCResource);
friend class RDOPROCSeize;
friend class RDOPROCRelease;

public:
    std::string whoAreYou();
    virtual LPRDOResource clone(const LPRDORuntime& pRuntime) const;

protected:
    std::list<LPRDOPROCTransact> transacts;

private:
    RDOPROCResource(const LPRDORuntime& pRuntime, const std::vector<RDOValue>& paramsCalcs, LPIResourceType pResType, std::size_t resID, std::size_t typeID, bool trace, bool permanentFlag);
    virtual ~RDOPROCResource();
};

CLOSE_RDO_RUNTIME_NAMESPACE
