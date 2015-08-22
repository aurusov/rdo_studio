#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/searchtree.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/searchtrace_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDODPTSearchTrace: public RDODPTSearch, public RDOTraceableObject, public IDPTSearchTraceStatistics
{
DECLARE_FACTORY(RDODPTSearchTrace);
public:
    enum class TraceFlag
    {
       NO_TRACE,
       TRACE_STAT,
       TRACE_TOPS,
       TRACE_ALL
    };

    void onSearchBegin         (const LPRDORuntime& pRuntime);
    void onSearchDecisionHeader(const LPRDORuntime& pRuntime);
    void onSearchDecision      (const LPRDORuntime& pRuntime, TreeNode* node);
    void onSearchResultSuccess (const LPRDORuntime& pRuntime, TreeRoot* treeRoot);
    void onSearchResultNotFound(const LPRDORuntime& pRuntime, TreeRoot* treeRoot);
    TreeRoot* createTreeRoot   (const LPRDORuntime& pRuntime);

    std::size_t calc_cnt; // Количество запусков
    std::size_t calc_res_found_cnt;
    std::list<double> calc_times;
    std::list<double> calc_cost;
    std::list<std::size_t> calc_mems;
    std::list<std::size_t> calc_nodes;
    std::list<std::size_t> calc_nodes_expended;
    std::list<std::size_t> calc_nodes_full;
    std::list<std::size_t> calc_nodes_in_graph;

    TraceFlag traceFlag;

protected:
    RDODPTSearchTrace(const LPRDORuntime& pRuntime, LPIBaseOperationContainer parent);

private:
    DECLARE_IDPTSearchTraceStatistics;
};

class TreeRootTrace: public TreeRoot
{
private:
    virtual void createRootTreeNode(const LPRDORuntime& pRuntime);

public:
    TreeRootTrace(const LPRDORuntime& pRuntime, RDODPTSearch* pDP);
};

class TreeNodeTrace: public TreeNode
{
friend class RDOTrace;

private:
    void      onSearchOpenNode        (const LPRDORuntime& pRuntime);
    void      onSearchNodeInfoDeleted (const LPRDORuntime& pRuntime);
    void      onSearchNodeInfoReplaced(const LPRDORuntime& pRuntime);
    void      onSearchNodeInfoNew     (const LPRDORuntime& pRuntime);
    TreeNode* createChildTreeNode     ();

public:
    TreeNodeTrace(const LPRDORuntime& pRuntime, TreeNode* i_parent, TreeRoot* i_root, LPIDPTSearchActivity i_activity, double cost, int cnt);
};

CLOSE_RDO_RUNTIME_NAMESPACE
