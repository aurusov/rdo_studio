#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

struct runtime_for_Seize
{
    LPRDOPROCResource  rss;
    int                Id_param;
    RDOValue           enum_free;
    RDOValue           enum_buzy;
    RDOValue           enum_break;
};

struct parser_for_Seize
{
    int Id_res;
    int Id_param;
};

class RDOPROCBlockForSeize: public RDOPROCBlock
{
public:
    static std::string getStateParamName();
    static std::string getStateEnumFree();
    static std::string getStateEnumBuzy();

protected:
    RDOPROCBlockForSeize(LPIPROCProcess process, std::vector<parser_for_Seize> From_Par);

    std::vector<runtime_for_Seize> forRes;
    std::vector<parser_for_Seize>  fromParser;

    void _onStart(const LPRDORuntime& pRuntime);
};

class RDOPROCSeize: public RDOPROCBlockForSeize
{
DECLARE_FACTORY(RDOPROCSeize);
private:
    RDOPROCSeize(LPIPROCProcess process, std::vector <parser_for_Seize> From_Par);

    std::size_t index;

    virtual void transactGoIn (const LPTransact& pTransact);
    virtual void transactGoOut(const LPTransact& pTransact);

    DECLARE_IBaseOperation;
};

class RDOPROCRelease: public RDOPROCBlockForSeize
{
DECLARE_FACTORY(RDOPROCRelease);
private:
    RDOPROCRelease(LPIPROCProcess process, std::vector <parser_for_Seize> From_Par);

    std::size_t index;

    DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE
