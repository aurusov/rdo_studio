#ifndef RDO_SEARCH_TRACE_H_
#define RDO_SEARCH_TRACE_H_

#include "rdo.h"
#include "rdotrace.h"

class RDOActivityTrace: public RDOActivity, RDOTraceableObject
{
friend TreeNodeTrace;
friend RDODecisionPointTrace;
friend RDOTrace;

public:
	RDOActivityTrace( RDOSimulatorTrace* i_sim, RDORule* r, bool vA ):
		RDOActivity( r, vA ),
		RDOTraceableObject( i_sim )
	{
		// Т.е. тут можно было написать id = r->id, тольуо r динамически
		// приведен к типу RDORule, в котром нет id.
		// Присвоение и увеличение i_sim->activityCounter на один было сделано
		// в конструкторе r. Текущее значение i_sim->activityCounter на 1 больше.
		id = i_sim->activityCounter - 1;
	}
};

enum DPT_TraceFlag
{
   DPT_no_trace,
   DPT_trace_stat,
   DPT_trace_tops,
   DPT_trace_all
};

class RDODecisionPointTrace: public RDODecisionPoint, RDOTraceableObject
{
public:
	void onSearchBegin(RDOSimulator *sim);
	void onSearchDecisionHeader(RDOSimulator *sim);
	void onSearchDecision(RDOSimulator *sim, TreeNode *node);
	void onSearchResultSuccess(RDOSimulator *sim, TreeRoot *treeRoot);
	void onSearchResultNotFound(RDOSimulator *sim, TreeRoot *treeRoot);
	TreeRoot *createTreeRoot(RDOSimulator *sim);

	int calc_cnt;
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
	RDODecisionPointTrace( RDOSimulatorTrace* i_sim ):
		RDODecisionPoint(),
		RDOTraceableObject( i_sim ),
		calc_cnt( 0 ),
		calc_res_found_cnt( 0 )
	{
		traceFlag = DPT_no_trace;
	}
};

class TreeRootTrace: public TreeRoot
{
   void createRootTreeNode(RDOSimulator *sim);
public:
   TreeRootTrace(RDOSimulator *sim, RDODecisionPoint *_dp): TreeRoot(sim, _dp) {}
};

class TreeNodeTrace: public TreeNode
{
friend RDOTrace;
   void onSearchOpenNode(RDOSimulator *sim);
   void onSearchNodeInfoDeleted(RDOSimulator *sim);
   void onSearchNodeInfoReplaced(RDOSimulator *sim);
   void onSearchNodeInfoNew(RDOSimulator *sim);
   TreeNode *createChildTreeNode();

public:
   TreeNodeTrace(RDOSimulator *i_sim, TreeNode *i_parent, TreeRoot *i_root, RDOActivity *i_activity, double cost, int cnt):
      TreeNode(i_sim, i_parent, i_root, i_activity, cost, cnt)
	{
	}
};


#endif //RDO_SEARCH_TRACE_H_
