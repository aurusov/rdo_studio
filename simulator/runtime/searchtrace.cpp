// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/rdo_rule.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchTrace
// --------------------------------------------------------------------------------
RDODPTSearchTrace::RDODPTSearchTrace(const LPRDORuntime& pRuntime, LPIBaseOperationContainer parent)
    : RDODPTSearch      (pRuntime, parent)
    , RDOTraceableObject(false           )
    , calc_cnt          (0               )
    , calc_res_found_cnt(0               )
{
    traceFlag = TraceFlag::NO_TRACE;
}

void RDODPTSearchTrace::onSearchBegin(const LPRDORuntime& pRuntime)
{
    if (traceFlag != TraceFlag::NO_TRACE)
    {
        pRuntime->getTracer()->writeSearchBegin(pRuntime->getCurrentTime(), traceId());
    }
    if (traceFlag == TraceFlag::TRACE_TOPS || traceFlag == TraceFlag::TRACE_ALL)
    {
        pRuntime->getTracer()->writeString("STN 1 0 0 0 -1 -1 0 0");
    }
    ++calc_cnt;
}

void RDODPTSearchTrace::onSearchDecisionHeader(const LPRDORuntime& pRuntime)
{
    if (traceFlag != TraceFlag::NO_TRACE)
    {
        pRuntime->getTracer()->writeSearchDecisionHeader();
    }
}

void RDODPTSearchTrace::onSearchDecision(const LPRDORuntime& pRuntime, TreeNode* node)
{
    if (traceFlag != TraceFlag::NO_TRACE)
    {
        pRuntime->getTracer()->writeSearchDecision(pRuntime, node);
    }
}

void RDODPTSearchTrace::onSearchResultSuccess(const LPRDORuntime& pRuntime, TreeRoot* treeRoot)
{
    if (traceFlag != TraceFlag::NO_TRACE)
    {
        pRuntime->getTracer()->writeSearchResult('S', pRuntime, treeRoot);
    }
    ++calc_res_found_cnt;
    calc_mems.push_back(treeRoot->m_sizeof_dpt);
    pRuntime->memory_insert(treeRoot->m_sizeof_dpt);
}

void RDODPTSearchTrace::onSearchResultNotFound(const LPRDORuntime& pRuntime, TreeRoot *treeRoot)
{
    if (traceFlag != TraceFlag::NO_TRACE)
    {
        pRuntime->getTracer()->writeSearchResult('N', pRuntime, treeRoot);
    }
}

// --------------------------------------------------------------------------------
// -------------------- TreeNodeTrace
// --------------------------------------------------------------------------------
TreeNodeTrace::TreeNodeTrace(const LPRDORuntime& pRuntime, TreeNode* i_parent, TreeRoot* i_root, LPIDPTSearchActivity i_activity, double cost, int cnt)
    : TreeNode(pRuntime, i_parent, i_root, i_activity, cost, cnt)
{}

void TreeNodeTrace::onSearchOpenNode(const LPRDORuntime& pRuntime)
{
    // TODO использовать явный cast
    RDODPTSearchTrace* dpTrace = (RDODPTSearchTrace *)m_root->m_dp;
    if (dpTrace->traceFlag == RDODPTSearchTrace::TraceFlag::TRACE_TOPS || dpTrace->traceFlag == RDODPTSearchTrace::TraceFlag::TRACE_ALL)
    {
        pRuntime->getTracer()->writeSearchOpenNode(m_number
                                                 , m_parent ? m_parent->m_number : 0
                                                 , m_costPath
                                                 , m_costRest);
    }
}

void TreeNodeTrace::onSearchNodeInfoDeleted(const LPRDORuntime& pRuntime)
{
    pRuntime->getTracer()->writeSearchNodeInfo('D', this);
}

void TreeNodeTrace::onSearchNodeInfoReplaced(const LPRDORuntime& pRuntime)
{
   pRuntime->getTracer()->writeSearchNodeInfo('R', this);
}

void TreeNodeTrace::onSearchNodeInfoNew(const LPRDORuntime& pRuntime)
{
   pRuntime->getTracer()->writeSearchNodeInfo('N', this);
}

TreeNode* TreeNodeTrace::createChildTreeNode()
{
    m_root->m_sizeof_dpt += sizeof(TreeNode);
    return new TreeNodeTrace(m_pChildRuntime, this, m_root, m_currAct, m_costPath, m_root->getNewNodeNumber());
}

// --------------------------------------------------------------------------------
// -------------------- TreeRootTrace
// --------------------------------------------------------------------------------
TreeRootTrace::TreeRootTrace(const LPRDORuntime& pRuntime, RDODPTSearch* pDP)
    : TreeRoot(pRuntime, pDP)
{}

void TreeRootTrace::createRootTreeNode(const LPRDORuntime& pRuntime)
{
    m_rootNode = new TreeNodeTrace(pRuntime, NULL, this, NULL, 0, getNewNodeNumber());
    m_rootNode->m_costRule = 0;
    m_rootNode->m_costPath = 0;
    m_rootNode->m_costRest = 0;
    m_OPEN.push_back(m_rootNode);
    m_sizeof_dpt += sizeof(TreeNodeTrace) + sizeof(void*);
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchTrace
// --------------------------------------------------------------------------------
TreeRoot* RDODPTSearchTrace::createTreeRoot(const LPRDORuntime& pRuntime)
{
    TreeRootTrace* root = new TreeRootTrace(pRuntime, this);
    root->m_sizeof_dpt = sizeof(TreeRootTrace) + pRuntime->getSizeofSim();
    return root;
}

std::size_t RDODPTSearchTrace::getCalcCnt() const
{
    return calc_cnt;
}

std::size_t RDODPTSearchTrace::getCalcResFoundCnt() const
{
    return calc_res_found_cnt;
}

template <typename T>
void __getStats(const std::list<T>& list, T& min, T& max, double& med)
{
    T sum = 0;
    std::size_t cnt = 0;
    typename std::list<T>::const_iterator it = list.begin();
    while (it != list.end())
    {
        sum += *it;
        if (!cnt)
        {
            min = *it;
            max = *it;
        }
        ++cnt;
        if (min > *it)
            min = *it;
        if (max < *it)
            max = *it;
        ++it;
    }
    if (cnt)
    {
        med = (double)sum / cnt;
    }
}

void RDODPTSearchTrace::getStatsDOUBLE(Type type, double& min, double& max, double& med) const
{
    switch (type)
    {
    case IDPTSearchTraceStatistics::Type::TIMES:
        return __getStats<double>(calc_times, min, max, med);
    case IDPTSearchTraceStatistics::Type::COST:
        return __getStats<double>(calc_cost , min, max, med);
    default:
        break;
    }
    NEVER_REACH_HERE;
}

void RDODPTSearchTrace::getStatsRUINT(Type type, std::size_t& min, std::size_t& max, double& med) const
{
    switch (type)
    {
    case IDPTSearchTraceStatistics::Type::MEMORY:
        return __getStats<std::size_t>(calc_mems, min, max, med);
    case IDPTSearchTraceStatistics::Type::NODES:
        return __getStats<std::size_t>(calc_nodes, min, max, med);
    case IDPTSearchTraceStatistics::Type::NODES_FULL:
        return __getStats<std::size_t>(calc_nodes_full, min, max, med);
    case IDPTSearchTraceStatistics::Type::NODES_EXPENDED:
        return __getStats<std::size_t>(calc_nodes_expended, min, max, med);
    case IDPTSearchTraceStatistics::Type::NODES_IN_GRAPH:
        return __getStats<std::size_t>(calc_nodes_in_graph, min, max, med);
    default:
        break;
    }
    NEVER_REACH_HERE;
}

CLOSE_RDO_RUNTIME_NAMESPACE
