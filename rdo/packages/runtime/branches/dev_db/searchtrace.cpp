/*!
  \copyright (c) RDO-Team, 2011
  \file      searchtrace.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Трассировка DPTSearch
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/headers/db/interface_db.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchTrace
// --------------------------------------------------------------------------------
void RDODPTSearchTrace::onSearchBegin(CREF(LPRDORuntime) pRuntime)
{
	if (traceFlag != DPT_no_trace)
	{
		pRuntime->getTracer()->writeSearchBegin(pRuntime->getCurrentTime(), traceId());
	}
	if (traceFlag == DPT_trace_tops || traceFlag == DPT_trace_all)
	{
		pRuntime->getTracer()->getTrcDB()->queryListPushBack(QString("INSERT INTO trc_st (DEFAULT,%1,'STN',1,0,0,0,-1,-1,0);")
			.arg(pRuntime->getTracer()->getSBid()));

		pRuntime->getTracer()->writeString("STN 1 0 0 0 -1 -1 0 0");
	}
	++calc_cnt;
}

void RDODPTSearchTrace::onSearchDecisionHeader(CREF(LPRDORuntime) pRuntime)
{
	if (traceFlag != DPT_no_trace)
	{
		pRuntime->getTracer()->writeSearchDecisionHeader();
	}
}

void RDODPTSearchTrace::onSearchDecision(CREF(LPRDORuntime) pRuntime, TreeNode* node)
{
	if (traceFlag != DPT_no_trace)
	{
		pRuntime->getTracer()->writeSearchDecision(pRuntime, node);
	}
}

void RDODPTSearchTrace::onSearchResultSuccess(CREF(LPRDORuntime) pRuntime, TreeRoot* treeRoot)
{
	if (traceFlag != DPT_no_trace)
	{
		pRuntime->getTracer()->writeSearchResult('S', pRuntime, treeRoot);
	}
	++calc_res_found_cnt;
	calc_mems.push_back(treeRoot->m_sizeof_dpt);
	pRuntime->memory_insert(treeRoot->m_sizeof_dpt);
}

void RDODPTSearchTrace::onSearchResultNotFound(CREF(LPRDORuntime) pRuntime, TreeRoot *treeRoot)
{
	if (traceFlag != DPT_no_trace)
	{
		pRuntime->getTracer()->writeSearchResult('N', pRuntime, treeRoot);
	}
}

// --------------------------------------------------------------------------------
// -------------------- TreeNodeTrace
// --------------------------------------------------------------------------------
void TreeNodeTrace::onSearchOpenNode(CREF(LPRDORuntime) pRuntime)
{
	/// @todo использовать явный cast
	RDODPTSearchTrace* dpTrace = (RDODPTSearchTrace *)m_root->m_dp;
	if (dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_tops || dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all)
	{
		pRuntime->getTracer()->writeSearchOpenNode(m_number
												 , m_parent ? m_parent->m_number : 0
												 , m_costPath
												 , m_costRest);
	}
}

void TreeNodeTrace::onSearchNodeInfoDeleted(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getTracer()->writeSearchNodeInfo('D', this);
}

void TreeNodeTrace::onSearchNodeInfoReplaced(CREF(LPRDORuntime) pRuntime)
{
   pRuntime->getTracer()->writeSearchNodeInfo('R', this);
}

void TreeNodeTrace::onSearchNodeInfoNew(CREF(LPRDORuntime) pRuntime)
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
void TreeRootTrace::createRootTreeNode(CREF(LPRDORuntime) pRuntime)
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
TreeRoot* RDODPTSearchTrace::createTreeRoot(CREF(LPRDORuntime) pRuntime)
{
	TreeRootTrace* root = new TreeRootTrace(pRuntime, this);
	root->m_sizeof_dpt = sizeof(TreeRootTrace) + pRuntime->getSizeofSim();
	return root;
}

ruint RDODPTSearchTrace::getCalcCnt() const
{
	return calc_cnt;
}

ruint RDODPTSearchTrace::getCalcResFoundCnt() const
{
	return calc_res_found_cnt;
}

template <typename T>
void __getStats(CREF(std::list<T>) list, REF(T) min, REF(T) max, REF(double) med)
{
	T      sum = 0;
	ruint  cnt = 0;
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

void RDODPTSearchTrace::getStatsDOUBLE(Type type, REF(double) min, REF(double) max, REF(double) med) const
{
	switch (type)
	{
	case IDPTSearchTraceStatistics::ST_TIMES:
		return __getStats<double>(calc_times, min, max, med);
	case IDPTSearchTraceStatistics::ST_COST:
		return __getStats<double>(calc_cost , min, max, med);
	default:
		break;
	}
	NEVER_REACH_HERE;
}

void RDODPTSearchTrace::getStatsRUINT(Type type, REF(ruint) min, REF(ruint) max, REF(double) med) const
{
	switch (type)
	{
	case IDPTSearchTraceStatistics::ST_MEMORY:
		return __getStats<ruint>(calc_mems, min, max, med);
	case IDPTSearchTraceStatistics::ST_NODES:
		return __getStats<ruint>(calc_nodes_in_graph, min, max, med);
	case IDPTSearchTraceStatistics::ST_NODES_FULL:
		return __getStats<ruint>(calc_nodes_full, min, max, med);
	case IDPTSearchTraceStatistics::ST_NODES_EXPENDED:
		return __getStats<ruint>(calc_nodes_expended, min, max, med);
	case IDPTSearchTraceStatistics::ST_NODES_IN_GRAPH:
		return __getStats<ruint>(calc_mems, min, max, med);
	default:
		break;
	}
	NEVER_REACH_HERE;
}

CLOSE_RDO_RUNTIME_NAMESPACE
