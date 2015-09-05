#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

struct runtime_for_Queue
{
    LPRDOResource rss;
    int           Id_param;
    RDOValue      defaultValue;
};

struct parser_for_Queue
{
    int Id_res;
    int Id_param;
};

class RDOPROCBlockForQueue: public RDOPROCBlock
{
protected:
    RDOPROCBlockForQueue(LPIPROCProcess process, parser_for_Queue From_Par);

    parser_for_Queue  fromParser;
    runtime_for_Queue forRes;
    void _onStart(const LPRDORuntime& pRuntime);
};

class RDOPROCQueue: public RDOPROCBlockForQueue
{
DECLARE_FACTORY(RDOPROCQueue);
public:
    static std::size_t getDefaultValue();
    static std::string getQueueParamName();

private:
    RDOPROCQueue(LPIPROCProcess process, parser_for_Queue From_Par);

    DECLARE_IBaseOperation;
};

class RDOPROCDepart: public RDOPROCBlockForQueue
{
DECLARE_FACTORY(RDOPROCDepart);
public:
    static std::size_t getDefaultValue();
    static std::string getDepartParamName();

private:
    RDOPROCDepart(LPIPROCProcess process, parser_for_Queue From_Par);

    DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE
