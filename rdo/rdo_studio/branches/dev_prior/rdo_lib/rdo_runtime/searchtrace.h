#ifndef SEARCHTRACE_H
#define SEARCHTRACE_H

#include "rdo.h"
#include "searchtree.h"
#include "rdotrace.h"
#include "simtrace.h"
#include "searchtrace_interface.h"

namespace rdoRuntime
{

class RDODPTSearchTrace: public RDODPTSearch, public RDOTraceableObject, public IDPTSearchTraceStatistics
{
DEFINE_FACTORY(RDODPTSearchTrace);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDODPTSearch)
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IDPTSearchTraceStatistics)
QUERY_INTERFACE_END

public:
	enum DPT_TraceFlag
	{
	   DPT_no_trace,
	   DPT_trace_stat,
	   DPT_trace_tops,
	   DPT_trace_all
	};

	void onSearchBegin( RDOSimulator* sim );
	void onSearchDecisionHeader( RDOSimulator* sim );
	void onSearchDecision( RDOSimulator* sim, TreeNode* node );
	void onSearchResultSuccess( RDOSimulator* sim, TreeRoot* treeRoot );
	void onSearchResultNotFound( RDOSimulator* sim, TreeRoot* treeRoot );
	TreeRoot* createTreeRoot( RDOSimulator* sim );

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
	RDODPTSearchTrace( RDOSimulatorTrace* sim ):
		RDODPTSearch( sim ),
		RDOTraceableObject( false ),
		calc_cnt( 0 ),
		calc_res_found_cnt( 0 )
	{
		traceFlag = DPT_no_trace;
	}

private:
	DECLARE_IDPTSearchTraceStatistics;
};

class TreeRootTrace: public TreeRoot
{
private:
	virtual void createRootTreeNode( RDOSimulator* sim );

public:
	TreeRootTrace( RDOSimulator* sim, RDODPTSearch* _dp ): TreeRoot( sim, _dp ) {}
};

class TreeNodeTrace: public TreeNode
{
friend class RDOTrace;

private:
	void onSearchOpenNode( RDOSimulator* sim );
	void onSearchNodeInfoDeleted( RDOSimulator* sim );
	void onSearchNodeInfoReplaced( RDOSimulator* sim );
	void onSearchNodeInfoNew( RDOSimulator* sim );
	TreeNode* createChildTreeNode();

public:
	TreeNodeTrace( RDOSimulator* i_sim, TreeNode* i_parent, TreeRoot* i_root, LPIDPTSearchActivity i_activity, double cost, int cnt ):
		TreeNode( i_sim, i_parent, i_root, i_activity, cost, cnt )
	{
	}
};

} // namespace rdoRuntime

#endif // SEARCHTRACE_H
