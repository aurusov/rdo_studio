#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RdoTrace.h"

void RDODecisionPointTrace::onSearchBegin(RDOSimulator *sim) 
{
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   if(traceFlag != DPT_no_trace)
      simTr->getTracer()->writeSearchBegin(simTr->getCurrentTime(), traceId());

   if(traceFlag == DPT_trace_tops || traceFlag == DPT_trace_all)
      simTr->getTracer()->writeString("STN 1 0 0 0 -1 -1 0 0");
}

void RDODecisionPointTrace::onSearchDecisionHeader(RDOSimulator *sim)
{
   if(traceFlag != DPT_no_trace)
   {
      RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
      simTr->getTracer()->writeSearchDecisionHeader();
   }
}

void RDODecisionPointTrace::onSearchDecision(RDOSimulator *sim, TreeNode *node)
{
   if(traceFlag != DPT_no_trace)
   {
      RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
      simTr->getTracer()->writeSearchDecision(sim, node); 
   }
}

void RDODecisionPointTrace::onSearchResultSuccess(RDOSimulator *sim, TreeRoot *treeRoot)
{
   if(traceFlag != DPT_no_trace)
   {
      RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
      simTr->getTracer()->writeSearchResult('S', simTr, treeRoot);
   }
}

void RDODecisionPointTrace::onSearchResultNotFound(RDOSimulator *sim, TreeRoot *treeRoot)
{
   if(traceFlag != DPT_no_trace)
   {
      RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
      simTr->getTracer()->writeSearchResult('N', simTr, treeRoot);
   }
}

void TreeNodeTrace::onSearchOpenNode(RDOSimulator *sim)
{
   RDODecisionPointTrace *dpTrace = (RDODecisionPointTrace *)root->dp;
   if(dpTrace->traceFlag == DPT_trace_tops || dpTrace->traceFlag == DPT_trace_all)
   {
      RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
      simTr->getTracer()->writeSearchOpenNode(count, 
         (parent?parent->count:0),
         costPath,
         costRest);
   }
}

void TreeNodeTrace::onSearchNodeInfoDeleted(RDOSimulator *sim)
{
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   simTr->getTracer()->writeSearchNodeInfo('D', this);
}

void TreeNodeTrace::onSearchNodeInfoReplaced(RDOSimulator *sim)
{
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   simTr->getTracer()->writeSearchNodeInfo('R', this); // must be 'R'
}

void TreeNodeTrace::onSearchNodeInfoNew(RDOSimulator *sim)
{
   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   simTr->getTracer()->writeSearchNodeInfo('N', this);
}


TreeNode *TreeNodeTrace::createChildTreeNode()
{
   return new TreeNodeTrace(childSim, this, root, currAct, costPath, root->nodeCount++);
}

void TreeRootTrace::createRootTreeNode(RDOSimulator *sim)
{
   rootNode = new TreeNodeTrace(sim, NULL, this, NULL, 0, nodeCount++);
	rootNode->costRule = rootNode->costPath = rootNode->costRest = 0;
   allLeafs.push_back(rootNode);
}

TreeRoot *RDODecisionPointTrace::createTreeRoot(RDOSimulator *sim)
{
   return new TreeRootTrace(sim, this);
}

