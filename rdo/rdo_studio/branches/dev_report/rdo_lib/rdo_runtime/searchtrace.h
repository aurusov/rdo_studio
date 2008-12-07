#ifndef SEARCHTRACE_H
#define SEARCHTRACE_H

#include "rdo.h"
#include "searchtree.h"
#include "rdotrace.h"
#include "simtrace.h"

namespace rdoRuntime
{

class RDOActivityTrace: public RDODPTSearch::Activity, public RDOTraceableObject
{
friend class TreeNodeTrace;
friend class RDODPTSearchTrace;
friend class RDOTrace;

public:
	RDOActivityTrace( RDORule* r, ValueTime valueTime );
};

class RDODPTSearchTrace: public RDODPTSearch, public RDOTraceableObject
{
public:
	enum DPT_TraceFlag {
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

	int calc_cnt; // Количество запусков
	int calc_res_found_cnt;
	std::list< double >       calc_times;
	std::list< unsigned int > calc_mems;
	std::list< double >       calc_cost;
	std::list< unsigned int > calc_nodes;
	std::list< unsigned int > calc_nodes_expended;
	std::list< unsigned int > calc_nodes_full;
	std::list< unsigned int > calc_nodes_in_graph;
	void getStats( std::list< double >& list, double& min, double& max, double& med ) const;
	void getStats( std::list< unsigned int >& list, unsigned int& min, unsigned int& max, double& med ) const;

	DPT_TraceFlag traceFlag;
	RDODPTSearchTrace( RDOSimulatorTrace* sim ):
		RDODPTSearch( sim ),
		RDOTraceableObject( false ),
		calc_cnt( 0 ),
		calc_res_found_cnt( 0 )
	{
		traceFlag = DPT_no_trace;
	}
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
	TreeNodeTrace( RDOSimulator* i_sim, TreeNode* i_parent, TreeRoot* i_root, RDODPTSearch::Activity* i_activity, double cost, int cnt ):
		TreeNode( i_sim, i_parent, i_root, i_activity, cost, cnt )
	{
	}
};

} // namespace rdoRuntime

#endif // SEARCHTRACE_H
