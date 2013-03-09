/*!
  \copyright (c) RDO-Team, 2011
  \file      searchtrace.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Трассировка DPTSearch
  \indent    4T
*/

#ifndef _LIB_RUNTIME_SEARCH_TRACE_H_
#define _LIB_RUNTIME_SEARCH_TRACE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/searchtree.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/simtrace.h"
#include "simulator/runtime/searchtrace_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDODPTSearchTrace
  \brief   Трассировка точки принятия решений DPTSearch
*/
class RDODPTSearchTrace: public RDODPTSearch, public RDOTraceableObject, public IDPTSearchTraceStatistics
{
DEFINE_IFACTORY(RDODPTSearchTrace);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDODPTSearch)
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IDPTSearchTraceStatistics)
QUERY_INTERFACE_END

public:
	/*!
	  \enum  DPT_TraceFlag
	  \brief статусы трассировки для точек DPTSearch
	*/
	enum DPT_TraceFlag
	{
	   DPT_no_trace,
	   DPT_trace_stat,
	   DPT_trace_tops,
	   DPT_trace_all
	};

	void onSearchBegin         (CREF(LPRDORuntime) pRuntime);
	void onSearchDecisionHeader(CREF(LPRDORuntime) pRuntime);
	void onSearchDecision      (CREF(LPRDORuntime) pRuntime, TreeNode* node);
	void onSearchResultSuccess (CREF(LPRDORuntime) pRuntime, TreeRoot* treeRoot);
	void onSearchResultNotFound(CREF(LPRDORuntime) pRuntime, TreeRoot* treeRoot);
	TreeRoot* createTreeRoot   (CREF(LPRDORuntime) pRuntime);

	ruint calc_cnt; // Количество запусков
	ruint calc_res_found_cnt;
	std::list<double> calc_times;
	std::list<double> calc_cost;
	std::list<ruint > calc_mems;
	std::list<ruint > calc_nodes;
	std::list<ruint > calc_nodes_expended;
	std::list<ruint > calc_nodes_full;
	std::list<ruint > calc_nodes_in_graph;

	DPT_TraceFlag traceFlag;

protected:
	RDODPTSearchTrace(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent);

private:
	DECLARE_IDPTSearchTraceStatistics;
};

/*!
  \class   TreeRootTrace
  \brief   Трассировка корня графа состояний точки принятия решений DPTSearch
*/
class TreeRootTrace: public TreeRoot
{
private:
	virtual void createRootTreeNode(CREF(LPRDORuntime) pRuntime);

public:
	TreeRootTrace(CREF(LPRDORuntime) pRuntime, PTR(RDODPTSearch) pDP);
};

/*!
  \class   TreeNodeTrace
  \brief   Трассировка узла графа состояний точки принятия решений DPTSearch
*/
class TreeNodeTrace: public TreeNode
{
friend class RDOTrace;

private:
	void          onSearchOpenNode        (CREF(LPRDORuntime) pRuntime);
	void          onSearchNodeInfoDeleted (CREF(LPRDORuntime) pRuntime);
	void          onSearchNodeInfoReplaced(CREF(LPRDORuntime) pRuntime);
	void          onSearchNodeInfoNew     (CREF(LPRDORuntime) pRuntime);
	PTR(TreeNode) createChildTreeNode     ();

public:
	TreeNodeTrace(CREF(LPRDORuntime) pRuntime, TreeNode* i_parent, TreeRoot* i_root, LPIDPTSearchActivity i_activity, double cost, int cnt);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/searchtrace.inl"

#endif // _LIB_RUNTIME_SEARCH_TRACE_H_
