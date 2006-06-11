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
   RDOActivityTrace(RDOSimulatorTrace *i_sim, RDORule *r, bool vA): 
      RDOActivity(r, vA), RDOTraceableObject(i_sim) {}
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
   void addActivity(RDOActivityTrace *act) 
   { 
      act->id = activities.size() + 1;
      RDODecisionPoint::addActivity(act);
   }
   void onSearchBegin(RDOSimulator *sim);
   void onSearchDecisionHeader(RDOSimulator *sim);
   void onSearchDecision(RDOSimulator *sim, TreeNode *node);
   void onSearchResultSuccess(RDOSimulator *sim, TreeRoot *treeRoot);
   void onSearchResultNotFound(RDOSimulator *sim, TreeRoot *treeRoot);
   TreeRoot *createTreeRoot(RDOSimulator *sim);

   DPT_TraceFlag traceFlag;
   RDODecisionPointTrace(RDOSimulatorTrace *i_sim): 
      RDODecisionPoint(), RDOTraceableObject(i_sim) { traceFlag = DPT_no_trace; }
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
