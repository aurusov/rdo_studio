#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <fstream>
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace_i.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class TreeNodeTrace;
class RDOEvent;
class RDORule;
class RDOOperation;
class RDOTraceableObject;
class TreeRootTrace;
class RDOResultTrace;
class TreeNode;
class TreeRoot;
PREDECLARE_POINTER(RDOResource);
PREDECLARE_POINTER(RDORuntime);

class RDOEndL
{
public:
    virtual void onEndl();
};

std::ostream& operator << (std::ostream& stream, RDOEndL& rdoEndL);

class RDOTrace
{
friend class RDOSimulatorTrace;
friend class RDOResource;

public:
    RDOTrace();
    virtual ~RDOTrace();

    bool canTrace() const;

    void startWriting();
    void stopWriting();
    bool canWrite() const;
    bool isNull() const;

    // Search in tree
    virtual void writeSearchBegin(double currentTime, std::string decisionPointId);
    virtual void writeSearchDecisionHeader();
    virtual void writeSearchDecision(const LPRDORuntime& pRuntime, TreeNode *node);
    virtual void writeString(std::string);
    virtual void writeSearchOpenNode(int nodeCount, int parentCount, double pathCost, double restCost);
    virtual void writeSearchNodeInfo(char sign, TreeNodeTrace *node);
    virtual void writeSearchResult(char letter, const LPRDORuntime& pRuntime, TreeRoot *treeRoot);

    virtual void writeEvent              (const LPIBaseOperation& opr, const LPRDORuntime& pRuntime);
    virtual void writeRule               (const LPIBaseOperation& opr, const LPRDORuntime& pRuntime);
    virtual void writeAfterOperationBegin(const LPIBaseOperation& opr, const LPRDORuntime& pRuntime);
    virtual void writeAfterOperationEnd  (const LPIBaseOperation& opr, const LPRDORuntime& pRuntime);

    virtual void writeTraceBegin(const LPRDORuntime& pRuntime);
    virtual void writeModelBegin(const LPRDORuntime& pRuntime);
    virtual void writeTraceEnd  (const LPRDORuntime& pRuntime);
    virtual void writeStatus    (const LPRDORuntime& pRuntime, const std::string& status);

    virtual void writePermanentResources(const LPRDORuntime& pRuntime, const std::list<LPRDOResource>& res_perm);

    std::string traceResourcesList( char prefix, const LPRDORuntime& pRuntime, const std::list<LPRDOResource>& rel_res_list);

    virtual void writeResult(const LPRDORuntime& pRuntime, RDOResultTrace* pok);

public:
    virtual std::ostream& getOStream();
    virtual RDOEndL& getEOL();

protected:
    bool m_isNullTracer;

private:
    bool m_canWriteToStream;
    std::ofstream m_emptyOut;
    RDOEndL m_emptyEndL;
};

class RDOTraceableObject: public ITrace
{
public:
    enum { NONE = 0xFFFFFFFF };

    bool traceable() const;
    void setTrace(bool trace);

    std::size_t getTraceID() const;
    void setTraceID(std::size_t id);
    void setTraceID(std::size_t id, std::size_t str_id);

    std::string& traceId() const;

protected:
    RDOTraceableObject(bool trace, std::size_t id = NONE, std::string str = "");
    virtual ~RDOTraceableObject();

private:
    bool m_trace;
    std::size_t m_id;
    mutable std::string m_str_id;
};

class RDOResultTrace: public RDOTraceableObject, public IResultTrace, public IResultTraceValue
{
public:
    RDOResultTrace(const LPRDORuntime& pRuntime, bool trace);

protected:
    LPRDORuntime m_pRuntime;
    bool m_wasChanged;

    DECLARE_IResultTrace;
};

CLOSE_RDO_RUNTIME_NAMESPACE
